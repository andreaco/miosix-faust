#include "audio.h"
#include "dac.h"
#include "miosix.h"
#include <functional>
#include <algorithm>

using namespace miosix;
using namespace std;

static AudioDriver& audioDriver = AudioDriver::getInstance();

void refillDMA_IRQ(AudioBuffer buffer, unsigned int bufferSize) {
    DMA1_Stream5->CR = 0;
    DMA1_Stream5->PAR = reinterpret_cast<unsigned int>(&SPI3->DR);
    DMA1_Stream5->M0AR = reinterpret_cast<unsigned int>(buffer);
    DMA1_Stream5->NDTR = bufferSize;
    DMA1_Stream5->CR = DMA_SxCR_PL_1 | //High priority DMA stream
                       DMA_SxCR_MSIZE_0 | //Read  16bit at a time from RAM
                       DMA_SxCR_PSIZE_0 | //Write 16bit at a time to SPI
                       DMA_SxCR_MINC | //Increment RAM pointer
                       DMA_SxCR_DIR_0 | //Memory to peripheral direction
                       DMA_SxCR_TCIE | //Interrupt on completion
                       DMA_SxCR_EN;       //Start the DMA
}

void refillDMA(AudioBuffer buffer, unsigned int bufferSize) {
    FastInterruptDisableLock lock;
    refillDMA_IRQ(buffer, bufferSize);
}

AudioDriver &AudioDriver::getInstance() {
    static AudioDriver instance;
    return instance;
}

AudioDriver::AudioDriver()
        : bufferSize(256) {
    auto lambda = [](unsigned short *, unsigned int) {};
    callback = lambda;
    buffer = new unsigned short[bufferSize];
    fill(buffer, buffer + bufferSize, 0);


    {
        FastInterruptDisableLock dLock;
        //Enable DMA1 and SPI3/I2S3
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
        RCC_SYNC();
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        RCC_SYNC();
               
        //Enable audio PLL (settings for 44100Hz audio)
        RCC->PLLI2SCFGR = (2 << 28) | (271 << 6);
        RCC->CR |= RCC_CR_PLLI2SON;
    }
    audioDac.init();


    //Wait for PLL to lock
    while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0);

    

    SPI3->CR2 = SPI_CR2_TXDMAEN;
    SPI3->I2SPR = SPI_I2SPR_MCKOE | 6;
    SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD    //I2S mode selected
                    | SPI_I2SCFGR_I2SE      //I2S Enabled
                    | SPI_I2SCFGR_I2SCFG_1; //Master transmit

    NVIC_SetPriority(DMA1_Stream5_IRQn, 2);//High priority for DMA
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    refillDMA(buffer, bufferSize);
    audioDac.send(0x02, 0x9e);
}



void AudioDriver::setDMACallback(CallbackFunction newCallback) {
    callback = newCallback;
}

/**
 * DMA end of transfer interrupt
 */
void __attribute__((naked)) DMA1_Stream5_IRQHandler() {
    saveContext();
    asm volatile("bl _Z17I2SdmaHandlerImplv");
    restoreContext();
}

/**
 * DMA end of transfer interrupt actual implementation
 */
void __attribute__((used)) I2SdmaHandlerImpl() {
    DMA1->HIFCR = DMA_HIFCR_CTCIF5 |
                  DMA_HIFCR_CTEIF5 |
                  DMA_HIFCR_CDMEIF5 |
                  DMA_HIFCR_CFEIF5;

    AudioBuffer buffer = audioDriver.getBuffer();
    unsigned int bufferSize = audioDriver.getBufferSize();
    CallbackFunction callback = audioDriver.getCallbackFunction();

    refillDMA_IRQ(buffer, bufferSize);
    callback(buffer, bufferSize);
}

