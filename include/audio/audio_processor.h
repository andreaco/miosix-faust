
#ifndef MIOSIX_KERNEL_AUDIO_PROCESSOR_H
#define MIOSIX_KERNEL_AUDIO_PROCESSOR_H

#include "../drivers/common/audio.h"
#include "audio_processable.h"
#include "audio_buffer.h"

/**
 * Base abstract class that can be extended to implement a
 * processor for audio.
 */
class AudioProcessor : public AudioProcessable {
public:

    /**
     * Constructor, builds the object by referencing to a specific AudioDriver
     * that will give the buffers to write to and its other attributs such like sample rate and block size.
     * @param audioDriver The AudioDriver to use
     */
    AudioProcessor(AudioDriver &audioDriver) : audioDriver(audioDriver) {};

    /**
     * Inherited from AudioProcessable, called each time a buffer is ready to be written
     */
    virtual void process() = 0;

    /**
     * Used to obtain a reference to the output AudioBuffer
     * @return The audio buffer in which to write to
     */
    inline AudioBuffer<float, 2, AUDIO_DRIVER_BUFFER_SIZE> &getBuffer() const { return audioDriver.getBuffer(); };

    /**
     * Getter for the buffer size
     * @return Audio Driver BufferSize
     */
    inline unsigned int getBufferSize() const { return audioDriver.getBufferSize(); };

    /**
     * Get the current sample rate in use by the Audio Driver
     * @return Current sample rate
     */
    inline float getSampleRate() const { return audioDriver.getSampleRate(); };

    /**
     * Inheriting destructor from AudioProcessable
     */
    AudioProcessor() = delete;

private:
    /**
     * Reference to the AudioDriver.
     */
    AudioDriver &audioDriver;
};

#endif //MIOSIX_KERNEL_AUDIO_PROCESSOR_H
