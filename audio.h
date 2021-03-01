
#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H

#include <miosix.h>
#include <functional>
#include <vector>
#include "dac.h"

class AudioDriver {
public:

    typedef std::function<void(unsigned short *, unsigned int)> CallbackFunction;

    /**
     * Type used for the audio buffer.
     */
    typedef unsigned short *AudioBuffer;

    /**
     * Gets an instance of the AudioDriver singleton.
     *
     * @return AudioDriver singletor
     */
    // TODO: move initialization in initialize() method, and use parameters to setup the drivers
    static AudioDriver &getInstance();

    /**
     * Initializes the audio driver.
     */
    void init();

    // TODO: switch from callback logic to storing a reference to an AudioProcessor
    void setDMACallback(CallbackFunction callback);

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

    inline CallbackFunction getCallbackFunction() { return callback; };


private:

    /**
     * This buffer can be used to write in the DAC.
     */
    AudioBuffer buffer;

    /**
     * Size of the buffer.
     */
    unsigned int bufferSize;

    /**
     * Dac driver.
     */
    Cs43l22dac audioDac;

    CallbackFunction callback;

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
