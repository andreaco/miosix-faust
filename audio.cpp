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

void AudioDriver::setSampleRate(SampleRate::SR sampleRate) {
    switch(sampleRate) {
    case SampleRate::_8000Hz:
        this->sampleRate = 8000.0;
        break;
    case SampleRate::_16000Hz:
        this->sampleRate = 16000.0;
        break;
    case SampleRate::_32000Hz:
        this->sampleRate = 32000.0;
        break;
    case SampleRate::_48000Hz:
        this->sampleRate = 48000.0;
        break;
    case SampleRate::_96000Hz:
        this->sampleRate = 96000.0;
        break;
    case SampleRate::_22050Hz:
        this->sampleRate = 22050.0;
        break;
    case SampleRate::_44100Hz:
        this->sampleRate = 44100.0;
        break;
    default:
        this->sampleRate = 0.0;
    break;
    }
}

void AudioDriver::init(SampleRate::SR sampleRate) {
    // Set up sample rate variable
    setSampleRate(sampleRate);

    // Init DAC with desired SR
    Cs43l22dac::init(sampleRate);

    // Refill DMA with empty audioBuffer
    refillDMA(audioBuffer, bufferSize);

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

