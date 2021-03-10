
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.hpp"
#include <functional>
#include <math.h>
#include <cstdint>

using namespace miosix;
using namespace std;

// testing an implementation of an AudioProcessor
class AudioProcessorTest : public AudioProcessor {
public:
    AudioProcessorTest() {
        for (int i = 0; i < AUDIO_DRIVER_BUFFER_SIZE; ++i) {
            sinTable[i] = sin(2 * 3.14 * i / AUDIO_DRIVER_BUFFER_SIZE);
        }
    }

    void process() override {
        auto& buffer = getBuffer();
        auto leftChannel = buffer.getWritePointer(0);
        for (unsigned int i = 0; i < getBufferSize(); ++i) {
            leftChannel[i] = sinTable[i];
        }
        buffer.applyGain(0);
    }


    float sinTable[AUDIO_DRIVER_BUFFER_SIZE];
};

int main() {
    AudioProcessorTest audioProcessorTest;
//    audioProcessorTest.process();

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.init(SampleRate::_44100Hz);

    // AudioBuffer copy test
    AudioBuffer<float, 1, 10> audioBuffer1;
    AudioBuffer<float, 1, 10> audioBuffer2;
    float* audioBufferRaw1 = audioBuffer1.getWritePointer(0);
    for (uint32_t i = 0; i < audioBuffer1.getBufferLength(); i++) {
        audioBufferRaw1[i] = static_cast<float>(i);
    }
//    audioBuffer2.copyOnChannel(audioBuffer1, 0);
//    audioBuffer2.add(audioBuffer1);
//    audioBuffer2.applyGain(0.5);


    // infinite loop
    for (;;) {

    }

}
