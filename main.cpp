
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.h"
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
    }

//    void process() override{
//
//    }

    float sinTable[AUDIO_DRIVER_BUFFER_SIZE];
};

int main() {
    AudioProcessorTest audioProcessorTest;
    audioProcessorTest.process();

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.init(SampleRate::_44100Hz);

    // SIMD test
    // TODO: test SIMD
    AudioBuffer<uint16_t, 2, 4> buffer1;
    AudioBuffer<uint16_t, 2, 4> buffer2;
    auto w11 = buffer1.getWritePointer(0);
    auto w12 = buffer1.getWritePointer(1);
    auto w21 = buffer2.getWritePointer(0);
    auto w22 = buffer2.getWritePointer(1);
    w11[0] = 1;
    w11[1] = 2;
    w11[2] = 3;
    w11[3] = 4;

    w12[0] = 5;
    w12[1] = 4;
    w12[2] = 3;
    w12[3] = 7;

    w21[0] = 2;
    w21[1] = 7;
    w21[2] = 7;
    w21[3] = 4;

    w22[0] = 7;
    w22[1] = 8;
    w22[2] = 6;
    w22[3] = 8;



    // infinite loop
    for (;;) {

    }

}
