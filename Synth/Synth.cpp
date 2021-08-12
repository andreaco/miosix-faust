#include "Synth.h"
Synth::Synth(AudioDriver &audioDriver) :
AudioProcessor(audioDriver)
{
    oscillator.setSampleRate(getSampleRate());
    oscillator.setFrequency(440);
    oscillator.setMuted(false);
    oscillator.setMode(PolyBlepOscillator::OscMode::SQUARE);

    envelope.setAttackTime(0.01, getSampleRate());
    envelope.setReleaseTime(1.5, getSampleRate());
}


void Synth::process()
{
    auto &buffer = getBuffer();
    float *left = buffer.getWritePointer(0);
    float *right = buffer.getWritePointer(1);

    if ((GPIOD->IDR & (1 << 1)) >> 1)
    {
        oscillator.setFrequency(40+rand()%400);
        gate();
    }

    for (uint32_t i = 0; i < getBufferSize(); i++)
    {
        float env = envelope.nextSample();
//        oscillator.setFrequency(220 + 220* env);
        float sample = env * oscillator.nextSample();
        left[i]  = sample;
        right[i] = sample;
    }
}