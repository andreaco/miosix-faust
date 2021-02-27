//
// Created by Federico Di Marzo on 24/02/2021.
//
#include "miosix.h"
#include "miosix/kernel/queue.h"
#include <functional>
#include <vector>


#ifndef MIOSIX_AUDIO_DRIVER_AUDIO_H
#define MIOSIX_AUDIO_DRIVER_AUDIO_H


typedef std::function<void(unsigned short *, unsigned int)> CallbackFunction;
typedef unsigned short *AudioBuffer;


class AudioDriver {
public:

    static AudioDriver &getInstance();

    void setDMACallback(CallbackFunction callback);

    inline AudioBuffer getBuffer() { return buffer; };

    inline unsigned int getBufferSize() { return bufferSize; };

    inline CallbackFunction getCallbackFunction() { return callback; };
private:
    unsigned int bufferSize;
    AudioBuffer buffer;
    CallbackFunction callback;
    Cs43l22dac audioDac;


    AudioDriver();

    AudioDriver(const AudioDriver &);

    AudioDriver &operator=(const AudioDriver &);
};


#endif //MIOSIX_AUDIO_DRIVER_AUDIO_H
