#ifndef MIOSIX_DRUM_SYNTH_H
#define MIOSIX_DRUM_SYNTH_H
#include "../audio/audio_processor.h"
#include "PolyBlepOscillator.h"
#include "Envelope.h"


class Synth : public AudioProcessor {
public:
    Synth(AudioDriver &audioDriver);

    void process() override;

    inline void gate()
    {
        envelope.gate();
    }

private:
    PolyBlepOscillator oscillator;
    Envelope envelope;
};
#endif //MIOSIX_DRUM_SYNTH_H

