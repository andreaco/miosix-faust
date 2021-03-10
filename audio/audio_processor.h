
#ifndef MIOSIX_KERNEL_AUDIO_PROCESSOR_H
#define MIOSIX_KERNEL_AUDIO_PROCESSOR_H

#include "audio.h"
#include "audio_processable.h"
#include "audio_buffer.h"

/**
 * Base abstract class that can be extended to implement a
 * processor for audio.
 */
class AudioProcessor : public AudioProcessable {
public:
    AudioProcessor();

    virtual void process() = 0;

    // TODO: find an way to define the buffer len
    inline AudioBuffer<float, 2, AUDIO_DRIVER_BUFFER_SIZE> &getBuffer() { return audioDriver.getBuffer();};

    inline unsigned int getBufferSize() { return audioDriver.getBufferSize(); };

    inline float getSampleRate() { return audioDriver.getSampleRate(); };

private:
    /**
     * Reference to the AudioDriver.
     */
    AudioDriver &audioDriver;
};

#endif //MIOSIX_KERNEL_AUDIO_PROCESSOR_H
