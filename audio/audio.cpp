#include "audio.h"
#include "cs43l22dac.h"
#include "miosix.h"
#include "audio_processor.h"
#include <functional>
#include <algorithm>
#include <memory>
#include <cstdint>


// TODO: Remove using declarations
using namespace miosix;

// singleton instance of the AudioDriver
static AudioDriver &audioDriver = AudioDriver::getInstance();

// instance of an AudioProcessable with an empty processor
static AudioProcessableDummy audioProcessableDummy;

// pointers to _bufferRight and _bufferLeft members of AudioDriver
static uint16_t *_bufferRightIRQ;
static uint16_t *_bufferLeftIRQ;


/**
 * This function is used to fill the DMA with a buffer.
 *
 * @param buffer AudioBuffer to move with DMA
 * @param bufferSize buffer size
 */
void refillDMA_IRQ(const uint16_t *bufferLeft, const uint16_t *bufferRight, unsigned int bufferSize) {
    // TODO: stereo DMA
    DMA1_Stream5->CR = 0;
    DMA1_Stream5->PAR = reinterpret_cast<uint32_t>(&SPI3->DR);
    DMA1_Stream5->M0AR = reinterpret_cast<uint32_t>(bufferLeft);
    // TODO: assert overflow buffer size
    DMA1_Stream5->NDTR = bufferSize; // setting the buffer size
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
void refillDMA(const uint16_t *bufferLeft, const uint16_t *bufferRight, unsigned int bufferSize) {
    FastInterruptDisableLock lock;
    refillDMA_IRQ(bufferLeft, bufferRight, bufferSize);
}

AudioDriver &AudioDriver::getInstance() {
    static AudioDriver instance;
    return instance;
}

void AudioDriver::setSampleRate(SampleRate::SR sampleRate) {
    switch (sampleRate) {
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
    refillDMA(_bufferLeft, _bufferRight, bufferSize);

    // TODO: set the volume based on the volume attribute
}

AudioDriver::AudioDriver()
        :
        bufferSize(AUDIO_DRIVER_BUFFER_SIZE),
        audioProcessable(&audioProcessableDummy) { // TODO: fine tune the bufferSize

    // initializing uint16_t buffers for right and left channels
    _bufferLeft = new uint16_t[AUDIO_DRIVER_BUFFER_SIZE];
    _bufferRight = new uint16_t[AUDIO_DRIVER_BUFFER_SIZE];
    std::fill(_bufferLeft, _bufferLeft + AUDIO_DRIVER_BUFFER_SIZE, 0);
    std::fill(_bufferRight, _bufferRight + AUDIO_DRIVER_BUFFER_SIZE, 0);

    // saving the pointer for the interrupts
    _bufferLeftIRQ = _bufferLeft;
    _bufferRightIRQ = _bufferRight;
}

AudioDriver::~AudioDriver() {}


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

    audioDriver.getAudioProcessable().process();


    unsigned int bufferSize = audioDriver.getBufferSize();
    auto& buffer = audioDriver.getBuffer();

    auto bufferLeftFloat = buffer.getReadPointer(0);
    auto bufferRightFloat = buffer.getReadPointer(1);

    // float to int conversion
    // TODO: find a better way to do it
    for (unsigned int i = 0; i < bufferSize; i++) {
        _bufferLeftIRQ[i] = static_cast<uint16_t>(((bufferLeftFloat[i] + 1.0f) / 2) * DAC_MAX_VALUE);
        _bufferRightIRQ[i] = static_cast<uint16_t>(((bufferRightFloat[i] + 1.0f) / 2) * DAC_MAX_VALUE);
    }

    // refilling the DMA buffer
    refillDMA_IRQ(_bufferLeftIRQ, _bufferRightIRQ, bufferSize);

    // callback to the AudioProcessable to process the buffer

//    AudioProcessable& audioProcessable = audioDriver.getAudioProcessable();
//    audioProcessable.process();
}

