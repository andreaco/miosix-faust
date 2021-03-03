#include "audio.h"
#include "cs43l22dac.h"
#include "miosix.h"
#include <functional>
#include <algorithm>
#include <memory>


// TODO: Remove using declarations
using namespace miosix;
using namespace std;

// singleton instance of the AudioDriver
static AudioDriver &audioDriver = AudioDriver::getInstance();

// instance of an AudioProcessable with an empty processor
static AudioProcessableDummy audioProcessableDummy;


/**
 * This function is used to fill the DMA with a buffer.
 *
 * @param buffer AudioBuffer to move with DMA
 * @param bufferSize buffer size
 */
void refillDMA_IRQ(AudioDriver::AudioBuffer buffer, unsigned int bufferSize) {
    DMA1_Stream5->CR = 0;
    DMA1_Stream5->PAR = reinterpret_cast<unsigned int>(&SPI3->DR);
    DMA1_Stream5->M0AR = reinterpret_cast<unsigned int>(buffer);
    DMA1_Stream5->NDTR = bufferSize;
    DMA1_Stream5->CR = DMA_SxCR_PL_1 |      //High priority DMA stream
                       DMA_SxCR_MSIZE_0 |   //Read  16bit at a time from RAM
                       DMA_SxCR_PSIZE_0 |   //Write 16bit at a time to SPI
                       DMA_SxCR_MINC |      //Increment RAM pointer
                       DMA_SxCR_DIR_0 |     //Memory to peripheral direction
                       DMA_SxCR_TCIE |      //Interrupt on completion
                       DMA_SxCR_EN;         //Start the DMA
}

/**
 * This function is a wrapper to refillDMA_IRQ that can be called
 * outside an interrupt.
 *
 * @param buffer AudioBuffer to move with DMA
 * @param bufferSize buffer size
 */
void refillDMA(AudioDriver::AudioBuffer buffer, unsigned int bufferSize) {
    FastInterruptDisableLock lock;
    refillDMA_IRQ(buffer, bufferSize);
}

AudioDriver &AudioDriver::getInstance() {
    static AudioDriver instance;
    return instance;
}

void AudioDriver::init(SampleRate::SR sampleRate) {
    int plli2sn;    // Multiplication factor for VCO
    int plli2sr;    // Division factor for I2S clocks
    int i2sdiv;     // I2S linear prescaler
    int i2sodd;     // Odd factor for the prescaler
    switch(sampleRate) {
        case SampleRate::_8000Hz:
            this->sampleRate = 8000.0;
            plli2sn = 256;
            plli2sr = 5;
            i2sdiv  = 122;
            i2sodd  = 1;
            break;
        case SampleRate::_16000Hz:
            this->sampleRate = 16000.0;
            plli2sn = 213;
            plli2sr = 2;
            i2sdiv  = 13;
            i2sodd  = 0;
            break;
        case SampleRate::_32000Hz:
            this->sampleRate = 32000.0;
            plli2sn = 213;
            plli2sr = 2;
            i2sdiv  = 6;
            i2sodd  = 1;
            break;
        case SampleRate::_48000Hz:
            this->sampleRate = 48000.0;
            plli2sn = 258;
            plli2sr = 3;
            i2sdiv  = 3;
            i2sodd  = 1;
            break;
        case SampleRate::_96000Hz:
            this->sampleRate = 96000.0;
            plli2sn = 344;
            plli2sr = 2;
            i2sdiv  = 3;
            i2sodd  = 1;
            break;
        case SampleRate::_22050Hz:
            this->sampleRate = 22050.0;
            plli2sn = 429;
            plli2sr = 4;
            i2sdiv  = 9;
            i2sodd  = 1;
            break;
        case SampleRate::_44100Hz:
            this->sampleRate = 44100.0;
            plli2sn = 271;
            plli2sr = 2;
            i2sdiv  = 6;
            i2sodd  = 0;
            break;
        default:
            this->sampleRate = 0.0;
            plli2sn = 0;
            plli2sr = 0;
            i2sdiv  = 0;
            i2sodd  = 0;
            break;
    }

    {
        FastInterruptDisableLock dLock;
        //Enable DMA1 and SPI3/I2S3
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
        RCC_SYNC();
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        RCC_SYNC();

        //Enable audio PLL
        RCC->PLLI2SCFGR = (plli2sr << 28) | (plli2sn << 6);
        RCC->CR |= RCC_CR_PLLI2SON;
    }
    Cs43l22dac::init();


    //Wait for PLL to lock
    while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0);


    SPI3->CR2 = SPI_CR2_TXDMAEN;
    SPI3->I2SPR = SPI_I2SPR_MCKOE | (i2sodd << 8) | i2sdiv;
    SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD    //I2S mode selected
                    | SPI_I2SCFGR_I2SE      //I2S Enabled
                    | SPI_I2SCFGR_I2SCFG_1; //Master transmit

    NVIC_SetPriority(DMA1_Stream5_IRQn, 2); //High priority for DMA
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    refillDMA(audioBuffer, bufferSize);
    Cs43l22dac::send(0x02, 0x9e);

    // TODO: set the volume based on the volume attribute
}

AudioDriver::AudioDriver()
        :
        bufferSize(256),
        audioProcessable(&audioProcessableDummy) { // TODO: fine tune the bufferSize
    audioBuffer = new unsigned short[bufferSize]; // TODO: manage memory in a better way
    fill(audioBuffer, audioBuffer + bufferSize, 0); // zeroing the audio buffer
}

AudioDriver::~AudioDriver() {
    delete [] audioBuffer;
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
    // TODO: handle DMA errors (maybe cleaning the buffer?)
    // removing the interrupts flags
    DMA1->HIFCR = DMA_HIFCR_CTCIF5 |
                  DMA_HIFCR_CTEIF5 |
                  DMA_HIFCR_CDMEIF5 |
                  DMA_HIFCR_CFEIF5;

    // refilling the DMA buffer
    AudioDriver::AudioBuffer buffer = audioDriver.getBuffer();
    unsigned int bufferSize = audioDriver.getBufferSize();
    refillDMA_IRQ(buffer, bufferSize);

    // callback to the AudioProcessable to process the buffer
    audioDriver.getAudioProcessable().process();
}

