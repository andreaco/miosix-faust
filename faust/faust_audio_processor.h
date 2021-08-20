#ifndef MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#define MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#include "../audio/audio_processor.h"
#include "../drivers/common/audio_config.h"
#include "parameter_config.h"
#include "faust_synth.h"

class Faust_AudioProcessor : public AudioProcessor {
public:
    Faust_AudioProcessor(AudioDriver &audioDriver)
        : AudioProcessor(audioDriver)
    {
        float currentSampleRate = audioDriver.getSampleRate();

        synth.init(currentSampleRate); // initializing the faust module
        synth.buildUserInterface(&control); // linking the faust module to the controler
    }

    void process() override
    {
        audioBuffers[0] = getBuffer().getWritePointer(0);
        audioBuffers[1] = getBuffer().getWritePointer(1);

        synth.compute(getBufferSize(), NULL, audioBuffers); // computing one block with faust
    }

    inline void setSlider1(float value)
    {
        control.setParamValue(SLIDER1_NAME, SLIDER1_MIN + value * (SLIDER1_MAX - SLIDER1_MIN));
    }

    inline void setSlider2(float value)
    {
        control.setParamValue(SLIDER2_NAME, SLIDER2_MIN + value * (SLIDER2_MAX - SLIDER2_MIN));
    }

    inline void setSlider3(float value)
    {
        control.setParamValue(SLIDER3_NAME, SLIDER3_MIN + value * (SLIDER3_MAX - SLIDER3_MIN));
    }

    inline void setSlider4(float value)
    {
        control.setParamValue(SLIDER4_NAME, SLIDER4_MIN + value * (SLIDER4_MAX - SLIDER4_MIN));
    }

    inline void setEncoder1(float value)
    {
        control.setParamValue(ENCODER1_NAME, ENCODER1_MIN + value * (ENCODER1_MAX - ENCODER1_MIN));
    }

    inline void setEncoder2(float value)
    {
        control.setParamValue(ENCODER2_NAME, ENCODER2_MIN + value * (ENCODER2_MAX - ENCODER2_MIN));
    }

    inline void setEncoder3(float value)
    {
        control.setParamValue(ENCODER3_NAME, ENCODER3_MIN + value * (ENCODER3_MAX - ENCODER3_MIN));
    }

    inline void setEncoder4(float value)
    {
        control.setParamValue(ENCODER4_NAME, ENCODER4_MIN + value * (ENCODER4_MAX - ENCODER4_MIN));
    }

    inline void gateOn()
    {
        control.setParamValue("/faust_synth/gate", 1);
    }

    inline void gateOff()
    {
        control.setParamValue("/faust_synth/gate", 0);
    }

private:
    FaustSynth synth;
    MapUI control;

    /**
     * Array containing the output buffer raw pointers to be passed to faust
     */
    float* audioBuffers[2];
};


#endif //MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
