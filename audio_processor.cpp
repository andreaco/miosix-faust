

#include "audio_processor.h"

AudioProcessor::AudioProcessor()
        : audioDriver(AudioDriver::getInstance()) {}

AudioDriver::AudioBuffer AudioProcessor::getBuffer() {
    return audioDriver.getBuffer();
}

unsigned int AudioProcessor::getBufferSize() {
    return audioDriver.getBufferSize();
}

double AudioProcessor::getSampleRate() {
    return audioDriver.getSampleRate();
}