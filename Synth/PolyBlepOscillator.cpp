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
            value *= TRI_RMS / SIN_RMS;
            break;
        case OscMode::SAW:
            value = (2.0f * phase) - 1.0f;
            value -= polyBlep(phase);
            value *= TRI_RMS / SAW_RMS;
            break;
        case OscMode::SQUARE:
            // Fixed Square
            value = (phase <= 0.5f) ? 1.0f : -1.0f;
            value += polyBlep(phase);
            value -= polyBlep(fmod(phase + 0.5f, 1.0));
            value *= TRI_RMS / SQR_RMS;
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

void PolyBlepOscillator::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer)
{
    if (isMuted) return;

    float *writePointer = buffer.getWritePointer(0);
    switch (oscMode) {
        case OscMode::SINE:
            for (unsigned int i = 0; i < buffer.getBufferLength(); ++i)
            {
                writePointer[i] = SinTable[floorf(phase * TABLE_SIZE)];
                // Update phase
                phase += phaseIncrement;
                while (phase >= 1.0f) {
                    phase -= 1.0f;
                }
            }
            buffer.applyGain(TRI_RMS / SIN_RMS);
            break;
        case OscMode::SAW:
            for (unsigned int i = 0; i < buffer.getBufferLength(); ++i)
            {
                writePointer[i] = (2.0f * phase) - 1.0f;
                writePointer[i] -= polyBlep(phase);

                // Update phase
                phase += phaseIncrement;
                while (phase >= 1.0f) {
                    phase -= 1.0f;
                }
            }
            buffer.applyGain(TRI_RMS / SAW_RMS);
            break;
        case OscMode::SQUARE:
            for (unsigned int i = 0; i < buffer.getBufferLength(); ++i)
            {
                // Fixed Square
                writePointer[i] = (phase <= 0.5f) ? 1.0f : -1.0f;
                writePointer[i] += polyBlep(phase);
                writePointer[i] -= polyBlep(fmod(phase + 0.5f, 1.0));
                // Update phase
                phase += phaseIncrement;
                while (phase >= 1.0f) {
                    phase -= 1.0f;
                }
            }
            buffer.applyGain(SQR_RMS / SAW_RMS);
            break;
        case OscMode::TRIANGLE:
            for (unsigned int i = 0; i < buffer.getBufferLength(); ++i)
            {
                // Fixed Square
                writePointer[i] = (phase <= 0.5f) ? 1.0f : -1.0f;
                writePointer[i] += polyBlep(phase);
                writePointer[i] -= polyBlep(fmod(phase + 0.5f, 1.0));

                // Leaky integrator: y[n] = A * x[n] + (1 - A) * y[n-1]
                writePointer[i] = phaseIncrement * writePointer[i] + (1 - phaseIncrement) * lastOutput;
                // Update phase
                phase += phaseIncrement;
                while (phase >= 1.0f) {
                    phase -= 1.0f;
                }
                lastOutput = writePointer[i];
            }
    }
}

    

// PolyBLEP by Tale
// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double PolyBlepOscillator::polyBlep(double t)
{
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
