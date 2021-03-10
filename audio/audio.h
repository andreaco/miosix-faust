
#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H

#include "miosix.h"
#include "cs43l22dac.h"
#include "audio_processable.h"
#include "audio_buffer.h"
#include <functional>
#include <vector>
#include <cstdint>

// TODO: add noexcepts

#define AUDIO_DRIVER_BUFFER_SIZE 128
#define DAC_BIT_DEPTH 12
#define DAC_MAX_VALUE 4096

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
     * @return AudioDriver singletor
     */
    static AudioDriver &getInstance();

    /**
     * Initializes the audio driver.
     */
    void init(SampleRate::SR = SampleRate::_44100Hz);

    /**
     * Getter for audioProcessable.
     *
     * @return audioProcessable
     */
    inline AudioProcessable &getAudioProcessable() {
        return *audioProcessable;
    }

    /**
     * Setter for audioProcessable.
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
     *
     * //TODO: change from double to float
     */
    inline double getSampleRate() const { return sampleRate; };

    /**
     * Destructor.
     */
    ~AudioDriver();

private:
    /**
     * This buffer can be used to write in the DAC.
     */
    AudioBuffer<float, 2, AUDIO_DRIVER_BUFFER_SIZE> audioBuffer;

    /**
     * Left channel buffer, used to convert the float audioBuffer in uint_16
     */
    uint16_t *_bufferLeft;

    /**
    * Left channel buffer, used to convert the float audioBuffer in uint_16
    */
    uint16_t *_bufferRight;

    /**
     * Size of the buffer.
     */
    unsigned int bufferSize;

    /**
     *
     */
    double sampleRate;

    /**
     * Instance of an AudioProcessable used as a callback
     * to process the buffer
     */
    AudioProcessable *audioProcessable;

    /**
     * Volume value of the audio driver.
     */
    // TODO: implement volume attribute (maybe is better a float?)
//    int volume;

    /**
     * Setup of the sample rate from SampleRate enum class
     */
    void setSampleRate(SampleRate::SR sampleRate);

    /**
     * Private constructor, use getInstance() to get the singleton.
     */
    AudioDriver();

    /**
     * The copy constructor is disabled.
     */
    AudioDriver(const AudioDriver &);

    /**
     * The move operator is disabled.
     */
    AudioDriver &operator=(const AudioDriver &);
};


#endif //MIOSIX_AUDIO_DRIVER_AUDIO_H
