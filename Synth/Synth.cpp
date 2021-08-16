#include "Synth.h"

Synth::Synth(AudioDriver &audioDriver) :
AudioProcessor(audioDriver), controlCounter(CONTROL_RATE)
{
    // Audio Rate
    oscillator.setSampleRate(getSampleRate());
    oscillator.setFrequency(440);
    oscillator.setMuted(false);
    oscillator.setState(0);

    envelope.setSampleRate(getSampleRate());
    envelope.setAttackTime(0.01);
    envelope.setReleaseTime(0.3);

    pitchEnv.setSampleRate(getSampleRate()/CONTROL_RATE);
    pitchEnv.setAttackTime(0.001);
    pitchEnv.setReleaseTime(0.1);

}

void Synth::process()
{
    oscillator.process(oscillatorBuffer);
    envelope.process(envelopeBuffer);
    oscillatorBuffer.multiply(envelopeBuffer);
    getBuffer().copyOnChannel(oscillatorBuffer, 0);
    getBuffer().copyOnChannel(oscillatorBuffer, 1);
}