
#include "miosix.h"
#include "drivers/common/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.h"
#include "audio/audio_math.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include "containers/circular_buffer.h"
#include <functional>
#include <algorithm>
#include <math.h>
#include <cstdint>

using namespace miosix;
using namespace std;

// testing an implementation of an AudioProcessor
class AudioProcessorTest : public AudioProcessor {
public:
    AudioProcessorTest(AudioDriver &audioDriver) :
            AudioProcessor(audioDriver),
            sineLUT([](float x) { return std::sin(x); }, 0, 2 * M_PI,
                    AudioMath::LookupTableEdges::PERIODIC) {}

    void process() override {
        auto &buffer = getBuffer();
        float *left = buffer.getWritePointer(0);
        float *right = buffer.getWritePointer(1);

        for (uint32_t i = 0; i < getBufferSize(); i += 1) {
            left[i] = 0.8 * sineLUT(phase);
            right[i] = left[i];
            phase += phaseDelta;
            if (phase >= 2 * M_PI) phase -= 2 * M_PI;
        }

    }

    AudioMath::LookupTable<128> sineLUT;

    float phase = 0;
    float phaseDelta = 440 * 2 * M_PI / 44100.0;
};


int main() {

    // initializing the audio driver
    AudioDriver audioDriver(SampleRate::_44100Hz);
    audioDriver.getBuffer();
    AudioProcessorTest audioProcessorTest(audioDriver);
    audioDriver.init();
    audioDriver.setAudioProcessable(audioProcessorTest);
    audioDriver.start();
}
