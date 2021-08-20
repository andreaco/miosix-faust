#ifndef MIOSIX_DRUM_ENVELOPE_H
#define MIOSIX_DRUM_ENVELOPE_H

#include "../../audio/audio_buffer.h"
#include "../../drivers/common/audio_config.h"

/**
 * Linear Envelope generator
 * On gate generates a linear attack-release curve
 * from the given attack and release time
 */
class Envelope {
public:
    /**
     * Constructor
     */
    Envelope() { }

    /**
     * Set the sample rate to control attack and release phase
     * @param sampleRate sample rate to be used by the envelope
     */
    inline void setSampleRate(float sampleRate)
    {
        this->sampleRate = sampleRate;
    }

    /**
     * Set the attack time
     * @param attackTime attack time in seconds
     */
    void setAttackTime(float attackTime);

    /**
     * Set the release time
     * @param releaseTime release time in seconds
     */
    void setReleaseTime(float releaseTime);

    /**
     * Set the envelope in active phase
     */
    void gate();

    /**
     * Function used to compute the envelope a sample at a time
     * @return current sample
     */
    inline double nextSample() {
        switch (currentState) {
            case IDLE:
                break;
            case ATTACK:
                output += 1 / attackTime;
                if (output >= 1.0) {
                    output = 1.0;
                    currentState = RELEASE;
                }
                break;
            case RELEASE:
                output -= 1 / releaseTime;
                if (output <= 0.0) {
                    output = 0.0;
                    currentState = IDLE;
                }
        }
        return output;
    }

    /**
     * Function used to compute the entire envelope in a single buffer
     * @param buffer buffer in which the envelope will be written
     */
    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

private:
    /**
     * Enum containing the states of the envelope
     */
    enum STATE { IDLE=0, ATTACK, RELEASE };

    /**
     * Current state of the envelope
     */
    STATE currentState;

    /**
     * Sample rate
     */
    float sampleRate;

    /**
     * Attack time in samples
     */
    float attackTime;

    /**
     * Release time in samples
     */
    float releaseTime;

    /**
     * Current output of the envelope
     */
    float output;
};


#endif //MIOSIX_DRUM_ENVELOPE_H
