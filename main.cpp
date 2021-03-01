
#include "miosix.h"
#include "audio.h"
#include <functional>
#include <math.h>
#include "audio_processor.h"

#define SINE_LEN 256

using namespace miosix;
using namespace std;

unsigned short sine_buf[SINE_LEN];

class AudioProcessorTest : public AudioProcessor {
public:
    inline void process() override {
        return;
    }
};

void playSine(unsigned short *buffer, unsigned int bufferSize) {
    FastInterruptDisableLock lock;
    for (unsigned int i = 0; i < bufferSize; i++) {
        buffer[i] = sine_buf[i % SINE_LEN];
    }
}

int main() {
    
    for (int i = 0; i < SINE_LEN; i++) {
        sine_buf[i] = (sin(2 * M_PI * SINE_LEN / i) / 2 + 1) * 4096;
    }

    function<void(unsigned short *, int)> callback = playSine;
    AudioDriver &audioDriver = AudioDriver::getInstance();
    audioDriver.init();
    audioDriver.setDMACallback(callback);

    AudioProcessorTest apt;
    apt.getSampleRate();
    apt.getBufferSize();
    apt.getBuffer();
    for (;;) {

    }
    
}
