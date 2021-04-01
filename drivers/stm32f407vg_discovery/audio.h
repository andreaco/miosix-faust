
#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H

#include "miosix.h"
#include "cs43l22dac.h"
#include "audio/audio_processable.h"
#include "audio/audio_buffer.h"
#include <functional>
#include <vector>
#include <cstdint>

// TODO: setting in a common header
// TODO: create an abstract class to allow portability

/**
 * Size of the stereo DAC buffer.
 */
// TODO: fine tune the bufferSize
#define AUDIO_DRIVER_BUFFER_SIZE 512
//#define AUDIO_DRIVER_BUFFER_SIZE 2 TODO: why can I put the length so low without any problem?

/**
 * Max value for DAC conversion ((2 ^ (BIT_DEPTH-1)) - 1).
 * Positive integer upper bound of the DAC values.
 */
#define DAC_MAX_POSITIVE_VALUE 32767

/**
 * This singleton class offers an interface to the low level audio
 * functionalities of the system.
 * It allows to set an AudioProcessable object to be called as a callback
 * to handle the audio processing.
 */
class AudioDriver {
public:

    /**
     * Gets an instance of the AudioDriver singleton.
     *
     * @return AudioDriver singleton
     */
    static AudioDriver &getInstance();

    /**
     * Initializes the audio driver.
     */
    void init(SampleRate::SR = SampleRate::_44100Hz);

    /**
     * Blocking call that starts the audio driver and
     * begins the audio processing.
     * In normal execution cases it never returns.
     */
    void start();

    /**
     * Getter for audioProcessable.
     *
     * @return audioProcessable
     */
    inline AudioProcessable &getAudioProcessable() {
        return *audioProcessable;
    }

    /**
     * Set the audio processable, this method must be called
     * after the init method.
     *
     */
    inline void setAudioProcessable(AudioProcessable &newAudioProcessable) {
        audioProcessable = &newAudioProcessable;
    }

    /**
     * Getter method for AudioBuffer.
     *
     * @return AudioBuffer
     */
    AudioBuffer<float, 2, AUDIO_DRIVER_BUFFER_SIZE> &getBuffer() { return audioBuffer; };

    /**
     * Getter method for the bufferSize.
     *
     * @return bufferSize
     */
    inline unsigned int getBufferSize() const { return bufferSize; };

    /**
     * Getter method for the sampleRate.
     *
     * @return sampleRate
     */
    inline float getSampleRate() const { return sampleRate; };

    /**
     * Sets the volume in a value between 0 and 1.
     * This value is mapped to the full decibel range of
     * the DAC.
     *
     * @param newVolume new volume value between 0 and 1
     */
    void setVolume(float newVolume);

    /**
     * Getter for the volume of the DAC.
     *
     * @return a value between 0 and 1
     */
    inline float getVolume() { return volume; };

    /**
     * Destructor.
     */
    ~AudioDriver();

    /**
     * The copy constructor is disabled.
     */
    AudioDriver(const AudioDriver &) = delete;

    /**
     * The move operator is disabled.
     */
    AudioDriver &operator=(const AudioDriver &) = delete;


private:
    /**
     * This stereo buffer is used by the audioProcessable for
     * the sound processing. the values inside the buffer must
     * be bounded in the interval [-1.0, 1.0].
     */
    AudioBuffer<float, 2, AUDIO_DRIVER_BUFFER_SIZE> audioBuffer;

    /**
     * Size of the buffer.
     */
    unsigned int bufferSize;

    /**
     * Sample rate of the DAC conversion.
     */
    float sampleRate;

    /**
     * Instance of an AudioProcessable used as a callback
     * to process the buffer.
     */
    AudioProcessable *audioProcessable;

    /**
     * Volume value of the audio driver.
     */
    float volume;

    /**
     * Setup of the sample rate from SampleRate enum class
     */
    void setSampleRate(SampleRate::SR sampleRate);

    /**
     * Utility method to copy current float buffers to the DAC integer output buffer
     */
    void writeToOutputBuffer(int16_t *writableRawBuffer) const;

    /**
     * Private constructor, use getInstance() to get the singleton.
     */
    AudioDriver();
};


#endif //MIOSIX_AUDIO_DRIVER_AUDIO_H
