#ifndef MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#define MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#include "PolyBlepOscillator.h"
#include "../audio/audio_math.h"

class MorphingOscillator
{
public:
    MorphingOscillator()
    {
        osc1.setMode(PolyBlepOscillator::OscMode::SINE);
        osc2.setMode(PolyBlepOscillator::OscMode::TRIANGLE);
    }

    void setMuted(bool isMuted)
    {
        osc1.setMuted(isMuted);
        osc2.setMuted(isMuted);
    }

    void setSampleRate(float sampleRate)
    {
        osc1.setSampleRate(sampleRate);
        osc2.setSampleRate(sampleRate);
    }

    void setFrequency(float frequency)
    {
        osc1.setFrequency(frequency);
        osc2.setFrequency(frequency);
    }

    void setState(float state)
    {
        state = AudioMath::clip(state, 0, 3);

        float whole;
        blend = std::modf(state, &whole);
        int modesIndex = (int) whole;

        if (modesIndex == 0)
        {
            osc1.setMode(PolyBlepOscillator::OscMode::SINE);
            osc2.setMode(PolyBlepOscillator::OscMode::TRIANGLE);
        }
        if (modesIndex == 1)
        {
            osc1.setMode(PolyBlepOscillator::OscMode::TRIANGLE);
            osc2.setMode(PolyBlepOscillator::OscMode::SAW);
        }
        else
        {
            osc1.setMode(PolyBlepOscillator::OscMode::SAW);
            osc2.setMode(PolyBlepOscillator::OscMode::SQUARE);
        }

    }

    float nextSample()
    {
        return (1-blend) * osc1.nextSample() + (blend) * osc2.nextSample();
    }


private:
    PolyBlepOscillator osc1;
    PolyBlepOscillator osc2;

    float blend;
};


#endif //MIOSIX_DRUM_MORPHINGOSCILLATOR_H
