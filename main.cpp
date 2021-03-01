
#include "miosix.h"
#include "audio.h"
#include "audio_processor.h"
#include <functional>
#include <math.h>

using namespace miosix;
using namespace std;

// testing an implementation of an AudioProcessor
class AudioProcessorTest : public AudioProcessor {
public:
    inline void process() override {
        static int i = 0;
        i++;
    }
};

int main() {
    AudioProcessorTest audioProcessorTest;

    // initializing the audio driver
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.init();

    // setting the audio processor
    audioDriver.setAudioProcessable(audioProcessorTest);

    // infinite loop
    for (;;) {

    }

}
