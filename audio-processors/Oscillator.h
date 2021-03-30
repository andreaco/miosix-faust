
#pragma once

#include <array>
#include <cmath>
#include <math.h> // sin
#include "audio/audio_math.h"

class Oscillator {
public:
    Oscillator() : sineLUT([](float x) { return sin(x); }, 0, 2 * M_PI, AudioMath::LookupTableEdges::PERIODIC)
    { }
    
    void prepare()
    {
    }
    
    void setFrequency (float frequency, float sampleRate)
    {
        auto phaseStep = 2 * M_PI / sampleRate;
        phaseDelta = frequency * phaseStep;
    }
    
    
    
    inline float getNextSample()
    {

        float currentSample = sineLUT(phase);
        phase += phaseDelta;

        if (phase >= 2 * M_PI)
            phase -= 2 * M_PI;

        return currentSample;
    }
    
    
    
private:

    AudioMath::LookupTable<128> sineLUT;

    float phase = 0.0f, phaseDelta = 0.0f;
    float freq;
};
