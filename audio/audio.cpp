#include "audio.h"
#include "cs43l22dac.h"
#include "miosix.h"
#include "../miosix/kernel/scheduler/scheduler.h"
#include "audio_processor.h"
#include "audio_buffer.hpp"
#include "audio_math.h"
#include <functional>
#include <algorithm>
#include <memory>

#define DOUBLE_BUFFER_BUFFERS 4

// TODO: static is not good

// singleton instance of the AudioDriver
static AudioDriver &audioDriver = AudioDriver::getInstance();

// instance of an AudioProcessable with an empty processor
static AudioProcessableDummy audioProcessableDummy;

// pointers to _bufferRight and _bufferLeft members of AudioDriver
static miosix::BufferQueue<int16_t, AUDIO_DRIVER_BUFFER_SIZE * 2,DOUBLE_BUFFER_BUFFERS > *doubleBuffer;
static miosix::Thread *writerThread;


/**
 * This function is used to fill the DMA with a buffer.
 *
 * @param buffer AudioBuffer to move with DMA
 * @param bufferSize buffer size
 */
void refillDMA_IRQ(miosix::BufferQueue<int16_t, AUDIO_DRIVER_BUFFER_SIZE * 2, DOUBLE_BUFFER_BUFFERS> *bufferQueue,
                   unsigned int bufferSize) {

    const int16_t *rawBuffer = nullptr;
    unsigned int size = 0;
    if (bufferQueue->tryGetReadableBuffer(rawBuffer, size) == false) {
        // TODO: error no buffer error
        return;
    }

    DMA1_Stream5->CR = 0;
    DMA1_Stream5->PAR = reinterpret_cast<unsigned int>(&SPI3->DR);
    DMA1_Stream5->M0AR = reinterpret_cast<unsigned int>(rawBuffer);

    // TODO: assert buffer size don't overflow the limit
    // TODO change bufferSize*2 -> size
    DMA1_Stream5->NDTR =bufferSize * 2; // setting the buffer size (stereo buffer size)
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
void refillDMA(miosix::BufferQueue<int16_t, AUDIO_DRIVER_BUFFER_SIZE * 2, DOUBLE_BUFFER_BUFFERS> *bufferQueue,
               unsigned int bufferSize) { // TODO: remove bufferSize

    miosix::FastInterruptDisableLock lock;
    refillDMA_IRQ(bufferQueue, bufferSize);
}


AudioDriver::AudioDriver()
        :
        bufferSize(AUDIO_DRIVER_BUFFER_SIZE),
        audioProcessable(&audioProcessableDummy) { // TODO: fine tune the bufferSize

    doubleBuffer = new miosix::BufferQueue<int16_t, AUDIO_DRIVER_BUFFER_SIZE * 2, DOUBLE_BUFFER_BUFFERS>();

    // disabling interrupts
    miosix::FastInterruptDisableLock lock;

    // initializing the double buffer
    doubleBuffer->reset();
    doubleBuffer->bufferFilled(doubleBuffer->bufferMaxSize());

}

AudioDriver::~AudioDriver() {
    delete doubleBuffer;
}

void AudioDriver::init(SampleRate::SR sampleRate) {

    // Set up sample rate variable
    setSampleRate(sampleRate);

    // Init DAC with desired SR
    Cs43l22dac::init(sampleRate);

    // default volume
    setVolume(0.7);

    // saving the writer thread
    writerThread = miosix::Thread::getCurrentThread();
    writerThread->setPriority(miosix::PRIORITY_MAX); // TODO: note to fedetft, setPriority not set_priority


}

void AudioDriver::start() {
    int16_t *writableRawBuffer;

    // Refill DMA with empty doubleBuffer
    refillDMA(doubleBuffer, bufferSize);

    while (true) {

        {
            // disable interrupts for synchronization
            miosix::FastInterruptDisableLock dLock;

            // looping until the writableRawBuffer is obtained
            while (doubleBuffer->tryGetWritableBuffer(writableRawBuffer) == false) {
                writerThread->IRQwait();
                {
                    // enabling back the interrupts because the writableRawBuffer
                    // is not available yet
                    miosix::FastInterruptEnableLock eLock(dLock);
                    miosix::Thread::yield();
                }
            }
        }

        // write on the buffer
        // callback to the AudioProcessable to process the buffer
        audioDriver.getAudioProcessable().process();

        unsigned int bufferSize = audioDriver.getBufferSize();
        auto &buffer = audioDriver.getBuffer();

        auto bufferLeftFloat = buffer.getReadPointer(0);
        auto bufferRightFloat = buffer.getReadPointer(1);

        // float to int conversion
        for (unsigned int i = 0; i < bufferSize; i++) {
            writableRawBuffer[i * 2] = static_cast<int16_t>(((bufferLeftFloat[i]) * DAC_MAX_POSITIVE_VALUE));
            writableRawBuffer[i * 2 + 1] = static_cast<int16_t>(((bufferRightFloat[i]) * DAC_MAX_POSITIVE_VALUE));
        }

        // signaling the doubleBuffer is now filled
        doubleBuffer->bufferFilled(doubleBuffer->bufferMaxSize());

    }
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

void AudioDriver::setVolume(float newVolume) {
    // clipping between 0 and 1
    newVolume = std::min(newVolume, 1.0f);
    newVolume = std::max(newVolume, 0.0f);

    // saving the value
    volume = newVolume;

    // mapping the float value to the range of the DAC
    // TODO: fine tune the range
    newVolume = AudioMath::linearMap(newVolume, 0, 1, -102, 0);
    Cs43l22dac::setVolume(static_cast<int>(newVolume));
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
    doubleBuffer->bufferEmptied(); // TODO: discover why this is not below
    refillDMA_IRQ(doubleBuffer, audioDriver.getBufferSize());
//    doubleBuffer->bufferEmptied();

    // waking up the reader
    writerThread->IRQwakeup();

    // forcing the scheduler to run the writerThread
    if (writerThread->IRQgetPriority() > miosix::Thread::IRQgetCurrentThread()->IRQgetPriority())
        miosix::Scheduler::IRQfindNextThread();

}

