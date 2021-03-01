
#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H

#include "miosix.h"
#include "dac.h"
#include "audio_processable.h"
#include <functional>
#include <vector>

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
    void init();

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
    inline AudioBuffer getBuffer() { return buffer; };

    /**
     * Getter method for the bufferSize.
     *
     * @return bufferSize
     */
    inline unsigned int getBufferSize() { return bufferSize; };

    ~AudioDriver();

private:
    /**
     * This buffer can be used to write in the DAC.
     */
    AudioBuffer buffer; // TODO: change buffer in audio buffer with a refactor

    /**
     * Size of the buffer.
     */
    unsigned int bufferSize;

    /**
     * Dac driver.
     */
    Cs43l22dac audioDac;

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

    // TODO:

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
