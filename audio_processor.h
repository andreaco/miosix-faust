
#ifndef MIOSIX_KERNEL_AUDIO_PROCESSOR_H
#define MIOSIX_KERNEL_AUDIO_PROCESSOR_H

#include "audio.h"

class AudioProcessor {
public:
    AudioProcessor();

    virtual void process() = 0;

    AudioDriver::AudioBuffer getBuffer();

    unsigned int getBufferSize();

    int getSampleRate(); // TODO: implement audio rate

private:
    AudioDriver &audioDriver;
};

#endif //MIOSIX_KERNEL_AUDIO_PROCESSOR_H
