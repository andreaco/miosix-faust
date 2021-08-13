#include "Envelope.h"

void Envelope::setAttackTime(float attackTimeMs)
{
    attackTime = attackTimeMs * sampleRate;
}

void Envelope::setReleaseTime(float releaseTimeMs)
{
    releaseTime = releaseTimeMs * sampleRate;
}

void Envelope::gate()
{
    currentState = ATTACK;
}