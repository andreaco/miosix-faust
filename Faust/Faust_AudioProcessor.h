#ifndef MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#define MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#include "../audio/audio_processor.h"
#include "../drivers/common/audio_config.h"

#include "FaustSynth.h"


class Faust_AudioProcessor : public AudioProcessor {
public:
    Faust_AudioProcessor(AudioDriver &audioDriver)
        : AudioProcessor(audioDriver)
    {
        float currentSampleRate = audioDriver.getSampleRate();
        //float blockSize = audioDriver.getBufferSize();

        synth.init(currentSampleRate); // initializing the Faust module
        synth.buildUserInterface(&control); // linking the Faust module to the controler

        // setting default values for the Faust module parameters
        control.setParamValue("/FaustSynth/freq",440);
        control.setParamValue("/FaustSynth/gain",1);
        control.setParamValue("/FaustSynth/gate", 1);
        control.setParamValue("/FaustSynth/A", 0.01);
        control.setParamValue("/FaustSynth/R", 1.4);
    }

    void process() override
    {
        float* audioBuffer = getBuffer().getWritePointer(1);

        synth.compute(getBufferSize(), NULL, &audioBuffer); // computing one block with Faust
    }

    inline void gateOn()
    {
        control.setParamValue("/FaustSynth/gate", 1);
    }

    inline void gateOff()
    {
        control.setParamValue("/FaustSynth/gate", 0);
    }

    inline void setAttackTime(float attackTimeMs)
    {
    }

    inline void setReleaseTime(float releaseTimeMs)
    {
    }

    inline void setFrequency(float freq)
    {
        control.setParamValue("/FaustSynth/freq", freq);
    }

    inline void setRatio(float ratio)
    {
        control.setParamValue("/FaustSynth/ratio", ratio);
    }

private:
    FaustSynth synth; // the Faust module (Saw.h)
    MapUI control; // used to easily control the Faust module (Saw.h)


    std::array<std::array<float, AUDIO_DRIVER_BUFFER_SIZE>, 1> audioBuffer;
};


#endif //MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
