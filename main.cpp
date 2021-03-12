
#include "miosix.h"
#include "audio/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.hpp"
#include "audio/audio_math.h"
#include "audio/midi.h"
#include "tests/midi_test_data.h"
#include <functional>
#include <algorithm>
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
        auto &buffer = getBuffer();
        auto leftChannel = buffer.getWritePointer(0);
        for (unsigned int i = 0; i < getBufferSize(); ++i) {
            leftChannel[i] = sinTable[i];
        }
    }



    float sinTable[AUDIO_DRIVER_BUFFER_SIZE];
};

int main() {

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.getBuffer();
    AudioProcessorTest audioProcessorTest;
    audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.init(SampleRate::_44100Hz);

    // midi test
    uint8_t *p = noteTest;
    Midi::Parser parser;
    parser.maskChannel(2);
    while (p != std::end(noteTest)) {
        parser.parse(p);
    }

    for (;;) {

    }

}
