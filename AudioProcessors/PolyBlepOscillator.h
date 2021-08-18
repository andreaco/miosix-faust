#ifndef POLYBLEPOSCILLATOR_H
#define POLYBLEPOSCILLATOR_H

#include <math.h>
#include <array>
#include "../audio/audio_buffer.h"
#include "../drivers/common/audio_config.h"

#define TABLE_SIZE 2048
#define SIN_RMS 0.7071067811865476
#define TRI_RMS 0.5773795568491479
#define SAW_RMS 0.9004251044154165
#define SQR_RMS 1.0000000000000000


class PolyBlepOscillator {
public:
    /**
     * Enum containing the oscillator modes
     */
    enum class OscMode {
        SINE = 0,
        SAW,
        SQUARE,
        TRIANGLE,
        kNumOscModes
    };
    
    /**
     * Set the current mode
     * @param mode mode to be used
     */
    inline void setMode(OscMode mode)
    {
        oscMode = mode;
    }
    
    /**
     * Set the current frequency of the oscillator
     * @param frequency frequency in Hz to be set
     */
    inline void setFrequency(float frequency)
    {
        frequency = fminf(fmaxf(frequency, 0), 20000);
        this->frequency = frequency;
        updatePhaseIncrement();
    }
    
    /**
     * Set the current sample rate in use by the oscillator
     * @param sampleRate sample rate to be set
     */
    inline void setSampleRate(float sampleRate)
    {
        this->sampleRate = sampleRate;
        updatePhaseIncrement();
    }

    /**
     * Function used to mute/unmute the oscillator.
     * The oscillator is always constructed as muted and needss to be unmuted manually
     */
    inline void setMuted(bool muted) { isMuted = muted; }
    
    /**
     * Get next sample and updates phase variables
     */
    float nextSample();

    /**
     * Process an entire mono AudioBuffer
     * @param buffer
     */
    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

    /**
    * Process an entire mono AudioBuffer by using a frequency modulator
    * @param buffer
    */
    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer,
                 AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &modulation);

    /**
     * Frequency getter
     */
     inline float getFrequency()
    {
         return frequency;
    }
    
    /**
     * Default Constructor
     */
    PolyBlepOscillator() :
        oscMode(OscMode::SINE),
        isMuted(true),
        frequency(440.0f),
        phase(0.0f),
        sampleRate(44100.0f)
    {
        updatePhaseIncrement();
        computeTables();
    };
    
    /**
     * Reset the phase of the oscillator
     */
    inline void reset() { phase = 0.0; }
    
private:
    /**
     * Mode currently in use by the oscillator
     */
    OscMode oscMode;
    
    /**
     * Mute state variable
     */
    bool isMuted;
    
    /**
     * Current frequency
     */
    float frequency;
    
    /**
     * Current phase
     */
    float phase;
    
    /**
     * Sample rate currently in use
     */
    float sampleRate;
    
    /**
     * Phase increment related to the current frequency in use
     */
    float phaseIncrement;
    
    /**
     * Auxiliary function used to update the phase increment
     */
    inline void updatePhaseIncrement()
    {
        float clippedFreq = fmin(fmax(frequency, 0), sampleRate/2.0f);
        phaseIncrement = clippedFreq / sampleRate;
    }
    
    /**
     * Used to store the last computed output
     */
    float lastOutput;
    
    /**
     * Function to compute the needed PolyBLEP
     */
    double polyBlep(double t);
    
    /**
     * Function used to pre-compute the waveform tables
     */
    void computeTables();
    
    typedef std::array<float, TABLE_SIZE> Table;
    /**
     * Waveform Tables
     */
    Table SinTable;
    
};

#endif // POLYBLEPOSCILLATOR_H
