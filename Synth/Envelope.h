#ifndef MIOSIX_DRUM_ENVELOPE_H
#define MIOSIX_DRUM_ENVELOPE_H

class Envelope {
public:
    Envelope() { }

    void setAttackTime(float attackTimeMs, float sampleRate);

    void setReleaseTime(float releaseTimeMs, float sampleRate);

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

private:
    enum STATE { IDLE=0, ATTACK, RELEASE };
    STATE currentState;

    float attackTime, releaseTime;
    float output;
};


#endif //MIOSIX_DRUM_ENVELOPE_H
