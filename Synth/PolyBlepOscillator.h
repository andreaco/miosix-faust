#ifndef MICROAUDIO_OSCILLATOR
#define MICROAUDIO_OSCILLATOR

#include <math.h>
#include <array>

#define TABLE_SIZE 2048

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
     * Set the pulse width used by the square wave
     */
    inline void setPulseWidth(float pulseWidth)
    {
        
        pulseWidth = fmax(fmin(pulseWidth, 1), 0);
        this->pulseWidth = pulseWidth;
        
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
        sampleRate(44100.0f),
        pulseWidth(0.5f)
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
     * Pulse width control
     */
    float pulseWidth;
    
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
    void updatePhaseIncrement()
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

#endif // MICROAUDIO_OSCILLATOR
