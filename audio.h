
#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H

#include "miosix.h"
#include "cs43l22dac.h"
#include "audio_processable.h"
#include <functional>
#include <vector>

/**
 * This class encapsulates the SR enum
 */
class SampleRate
{
public:
    /**
     * Example usage:
     * \code audioDriver.init(SampleRate::_44100Hz); \endcode
     */
    enum SR
    {
        _8000Hz,
        _16000Hz,
        _32000Hz,
        _48000Hz,
        _96000Hz,
        _22050Hz,
        _44100Hz
    };
private:
    SampleRate(); // Wrapper class, disallow creating instances
};



class AudioDriver {
public:


    /**
     * Type used for the audio buffer.
     */
    typedef unsigned short *AudioBuffer;

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
    inline AudioBuffer getBuffer() { return audioBuffer; };

    /**
     * Getter method for the bufferSize.
     *
     * @return bufferSize
     */
    inline unsigned int getBufferSize() { return bufferSize; };

    /**
     * Getter method for the sampleRate;
     *
     * @return sampleRate
     */
    inline double getSampleRate() { return sampleRate; };

    /**
     * Destructor
     */
    ~AudioDriver();

private:
    /**
     * This buffer can be used to write in the DAC.
     */
    AudioBuffer audioBuffer;

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
    AudioProcessable* audioProcessable;

    /**
     * Volume value of the audio driver.
     */
     // TODO: implement volume attribute (maybe is better a float?)
//    int volume;

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
