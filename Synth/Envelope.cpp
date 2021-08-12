#include "Envelope.h"

void Envelope::setAttackTime(float attackTimeMs, float sampleRate)
{
    attackTime = attackTimeMs * sampleRate;
}

void Envelope::setReleaseTime(float releaseTimeMs, float sampleRate)
{
    releaseTime = releaseTimeMs * sampleRate;
}

void Envelope::gate()
{
    currentState = ATTACK;
}