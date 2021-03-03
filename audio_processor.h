
#ifndef MIOSIX_KERNEL_AUDIO_PROCESSOR_H
#define MIOSIX_KERNEL_AUDIO_PROCESSOR_H

#include "audio.h"
#include "audio_processable.h"

/**
 * Base abstract class that can be extended to implement a
 * processor for audio.
 */
class AudioProcessor : public AudioProcessable {
public:
    AudioProcessor();

    virtual void process() = 0;

    AudioDriver::AudioBuffer getBuffer();

    unsigned int getBufferSize();

    double getSampleRate(); 

private:
    /**
     * Reference to the AudioDriver.
     */
    AudioDriver &audioDriver;
};

#endif //MIOSIX_KERNEL_AUDIO_PROCESSOR_H
