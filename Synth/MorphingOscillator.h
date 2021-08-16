#ifndef MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#define MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#include "PolyBlepOscillator.h"
#include "../audio/audio_math.h"
#include "../audio/audio_buffer.h"


class MorphingOscillator
{
public:
    MorphingOscillator()
    {
        setState(0);
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
        else if (modesIndex == 1)
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

    inline float getFrequency()
    {
        return osc1.getFrequency();
    }

    float nextSample()
    {
        return ((1-blend) * osc1.nextSample() + (blend) * osc2.nextSample())/2.0f;
    }

    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer)
    {
        osc1.process(osc1Buffer);
        osc2.process(osc2Buffer);
        osc1Buffer.applyGain(1-blend);
        osc2Buffer.applyGain(blend);
        osc1Buffer.add(osc2Buffer);
        osc1Buffer.applyGain(0.5);
        buffer.copyOnChannel(osc1Buffer, 0);
    }



private:
    PolyBlepOscillator osc1;
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> osc1Buffer;

    PolyBlepOscillator osc2;
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> osc2Buffer;


    float blend;
};


#endif //MIOSIX_DRUM_MORPHINGOSCILLATOR_H
