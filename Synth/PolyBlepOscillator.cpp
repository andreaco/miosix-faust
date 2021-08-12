#include "PolyBlepOscillator.h"

void PolyBlepOscillator::computeTables()
{
    for (int i = 0; i < TABLE_SIZE; ++i) {
        float phi = i / (float) TABLE_SIZE;
        SinTable[i] = sinf(phi * 2 * M_PI);
    }
}



float PolyBlepOscillator::nextSample()
{
    if (isMuted) return 0.0f;
    
    // Init output value
    float value = 0.0f;
    
    switch (oscMode) {
        case OscMode::SINE:
            value = SinTable[floorf(phase * TABLE_SIZE)];
            break;
        case OscMode::SAW:
            value = (2.0f * phase) - 1.0f;
            value -= polyBlep(phase);
            break;
        case OscMode::SQUARE:
            // PWM Square
            // Saw 1
            value = (2.0 * phase) - 1.0f;
            value -= polyBlep(phase);
            
            // Saw 2 with offset
            value -= (2.0 * fmod(phase + pulseWidth, 1.0)) - 1.0f;
            value += polyBlep(fmod(phase + pulseWidth, 1.0));
            
            // Normalization
            value *= 0.5;
            
            /*
            // Fixed Square
            value = (phase <= 0.5f) ? 1.0f : -1.0f;
            value += polyBlep(phase);
            value -= polyBlep(fmod(phase + 0.5f, 1.0));
            */
            
            break;
        case OscMode::TRIANGLE:
            value = (phase <= 0.5f) ? 1.0f : -1.0f;
            value += polyBlep(phase);
            value -= polyBlep(fmod(phase + 0.5, 1.0));
             // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
            value = phaseIncrement * value + (1 - phaseIncrement) * lastOutput;
            lastOutput = value;
    }
    
    // Update phase
    phase += phaseIncrement;
    while (phase >= 1.0f) {
        phase -= 1.0f;
    }
    
    return value;
}
    

// PolyBLEP by Tale
// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double PolyBlepOscillator::polyBlep(double t) {
    double dt = phaseIncrement;
    // 0 <= t < 1
    if (t < dt) {
        t /= dt;
        return t+t - t*t - 1.0;
    }
    // -1 < t < 0
    else if (t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t*t + t+t + 1.0;
    }
    // 0 otherwise
    else return 0.0;
}
