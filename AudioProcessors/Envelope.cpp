#include "Envelope.h"

void Envelope::setAttackTime(float attackTime)
{
    this->attackTime = attackTime * sampleRate;
}

void Envelope::setReleaseTime(float releaseTime)
{
    this->releaseTime = releaseTime * sampleRate;
}

void Envelope::gate()
{
    currentState = ATTACK;
}

void Envelope::process(AudioBuffer<float, 1, AUDIO_DRIVER_BUFFER_SIZE> &buffer)
{
    float *writePointer = buffer.getWritePointer(0);
    for (size_t i = 0; i < buffer.getBufferLength(); ++i)
    {
        switch (currentState) {
            case IDLE:
                writePointer[i] = 0;
                break;
            case ATTACK:
                output += 1/attackTime;
                if (output >= 1.0) {
                    output = 1.0;
                    currentState = RELEASE;
                }
                writePointer[i] = output;
                break;
            case RELEASE:
                output -= 1/releaseTime;
                if (output <= 0.0) {
                    output = 0.0;
                    currentState = IDLE;
                }
                writePointer[i] = output;
                break;
        }
    }
}