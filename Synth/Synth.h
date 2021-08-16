#ifndef MIOSIX_DRUM_SYNTH_H
#define MIOSIX_DRUM_SYNTH_H
#include "../audio/audio_processor.h"
#include "PolyBlepOscillator.h"
#include "Envelope.h"
#include "MorphingOscillator.h"
#include "../drivers/stm32f407vg_discovery/encoder.h"
#include "../drivers/stm32f407vg_discovery/button.h"

#define CONTROL_RATE 10
class Synth : public AudioProcessor {
public:
    Synth(AudioDriver &audioDriver);

    void process() override;

    inline void gate()
    {
        envelope.gate();
        pitchEnv.gate();
    }

    inline void setAttackTime(float attackTimeMs)
    {
        envelope.setAttackTime(attackTimeMs);
    }

    inline void setReleaseTime(float releaseTimeMs)
    {
        envelope.setReleaseTime(releaseTimeMs);
    }

    inline void setFrequency(float freq)
    {
        oscillator.setFrequency(freq);
    }

    inline void setMorph(float morph)
    {
        oscillator.setState(morph);
    }

    void controlCallback()
    {
        controlCounter--;
        if(controlCounter == 0)
        {
            controlCounter = CONTROL_RATE;
        }
    }


private:
    MorphingOscillator oscillator;
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscillatorBuffer;

    Envelope envelope;
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> envelopeBuffer;

    Envelope pitchEnv;
    int controlCounter;
};
#endif //MIOSIX_DRUM_SYNTH_H

