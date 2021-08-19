#ifndef MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#define MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#include "../audio/audio_processor.h"
#include "../drivers/common/audio_config.h"
#include "parameter_config.h"

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
    }

    void process() override
    {
        float* audioBuffer = getBuffer().getWritePointer(1);

        synth.compute(getBufferSize(), NULL, &audioBuffer); // computing one block with Faust
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
        control.setParamValue("/FaustSynth/gate", 1);
    }

    inline void gateOff()
    {
        control.setParamValue("/FaustSynth/gate", 0);
    }

private:
    FaustSynth synth; // the Faust module (Saw.h)
    MapUI control; // used to easily control the Faust module (Saw.h)


    std::array<std::array<float, AUDIO_DRIVER_BUFFER_SIZE>, 1> audioBuffer;
};


#endif //MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
