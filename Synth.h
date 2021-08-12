#ifndef MIOSIX_DRUM_SYNTH_H
#define MIOSIX_DRUM_SYNTH_H
#include "drivers/audio_processor.h"
class Synth : public AudioProcessor {
public:
    Synth(AudioDriver &audioDriver) :
        AudioProcessor(audioDriver)
    {}

    void process() override {

    }
};
#endif //MIOSIX_DRUM_SYNTH_H

