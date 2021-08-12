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

    inline void setFrequency(float freq)
    {
        oscillator.setFrequency(freq);
    }

    inline void setFMFreq(float fmfreq)
    {
        fmOsc.setFrequency(fmfreq);
    }

private:
    PolyBlepOscillator oscillator;
    PolyBlepOscillator fmOsc;
    Envelope envelope;
};
#endif //MIOSIX_DRUM_SYNTH_H

