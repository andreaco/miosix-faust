#ifndef MIOSIX_DRUM_ENVELOPE_H
#define MIOSIX_DRUM_ENVELOPE_H

#include "../audio/audio_buffer.h"
#include "../drivers/common/audio_config.h"

class Envelope {
public:
    Envelope() { }

    inline void setSampleRate(float sampleRate)
    {
        this->sampleRate = sampleRate;
    }

    void setAttackTime(float attackTimeMs);

    void setReleaseTime(float releaseTimeMs);

    void gate();

    inline double nextSample() {
        switch (currentState) {
            case IDLE:
                break;
            case ATTACK:
                output += 1/attackTime;
                if (output >= 1.0) {
                    output = 1.0;
                    currentState = RELEASE;
                }
                break;
            case RELEASE:
                output -= 1/releaseTime;
                if (output <= 0.0) {
                    output = 0.0;
                    currentState = IDLE;
                }
        }
        return output;
    }
    void process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer);

private:
    enum STATE { IDLE=0, ATTACK, RELEASE };
    STATE currentState;

    float sampleRate;

    float attackTime, releaseTime;
    float output;
};


#endif //MIOSIX_DRUM_ENVELOPE_H
