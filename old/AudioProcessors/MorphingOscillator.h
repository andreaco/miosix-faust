#ifndef MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#define MIOSIX_DRUM_MORPHINGOSCILLATOR_H
#include "PolyBlepOscillator.h"
#include "../../audio/audio_math.h"
#include "../../audio/audio_buffer.h"


/**
 * Oscillator that let cycle through different waveforms
 * by interpolating them linearly
 */
class MorphingOscillator
{
public:
    /**
     * Constructor
     */
    MorphingOscillator()
    {
        setState(0);
    }

    /**
     * Mutes the oscillator
     * @param isMuted true if muting, false otherwise
     */
    void setMuted(bool isMuted)
    {
        oscA.setMuted(isMuted);
        oscB.setMuted(isMuted);
    }

    /**
     * Setup of the sample rate
     * @param sampleRate sample rate to be used
     */
    void setSampleRate(float sampleRate)
    {
        oscA.setSampleRate(sampleRate);
        oscB.setSampleRate(sampleRate);
    }

    /**
     * Set the frequency
     * @param frequency frequency to be set
     */
    void setFrequency(float frequency)
    {
        oscA.setFrequency(frequency);
        oscB.setFrequency(frequency);
    }

    /**
     * Set the state of the morphing oscillator.
     * From 0 to 1 is linear interpolation between a Sine and a Triangular oscillator.
     * From 1 to 2 is linear interpolation between a Triangular and a Saw oscillator.
     * From 2 to 3 is linear interpolation between a Saw oscillator and a Square oscillator.
     * @param state value between 0 and 3
     */
    void setState(float state)
    {
        state = AudioMath::clip(state, 0, 3);

        float whole;
        blend = std::modf(state, &whole);
        int modesIndex = (int) whole;

        if (modesIndex == 0)
        {
            oscA.setMode(PolyBlepOscillator::OscMode::SINE);
            oscB.setMode(PolyBlepOscillator::OscMode::TRIANGLE);
        }
        else if (modesIndex == 1)
        {
            oscA.setMode(PolyBlepOscillator::OscMode::TRIANGLE);
            oscB.setMode(PolyBlepOscillator::OscMode::SAW);
        }
        else
        {
            oscA.setMode(PolyBlepOscillator::OscMode::SAW);
            oscB.setMode(PolyBlepOscillator::OscMode::SQUARE);
        }

    }

    /**
     * Get the current frequency in use by the oscillators
     * @return frequency currently in use by the oscillators
     */
    inline float getFrequency()
    {
        return oscA.getFrequency();
    }

    /**
     * Function used to compute the oscillator output one sample at a time
     * @return current sample output of the oscillator
     */
    float nextSample()
    {
        return ((1-blend) * oscA.nextSample() + (blend) * oscB.nextSample())/2.0f;
    }


    /**
     * Function used to compute in batch an entire buffer output of the oscillator
     * @param buffer buffer in which the output of the oscillator will be stored
     */
    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer)
    {
        oscA.process(oscABuffer);
        oscB.process(oscBBuffer);
        oscABuffer.applyGain(1-blend);
        oscBBuffer.applyGain(blend);
        oscABuffer.add(oscBBuffer);
        oscABuffer.applyGain(0.5);
        buffer.copyOnChannel(oscABuffer, 0);
    }
    
private:
    /**
     * Oscillator A
     */
    PolyBlepOscillator oscA;

    /**
     * Buffer used to store and compute oscillator A
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscABuffer;

    /**
     * Oscillator B
     */
    PolyBlepOscillator oscB;

    /**
     * Buffer used to store and compute oscillator B
     */
    AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> oscBBuffer;


    float blend;
};


#endif //MIOSIX_DRUM_MORPHINGOSCILLATOR_H
