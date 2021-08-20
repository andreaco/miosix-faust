#ifndef MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#define MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#include "../audio/audio_processor.h"
#include "../drivers/common/audio_config.h"
#include "parameter_config.h"
#include "faust_synth.h"

class FaustAudioProcessor : public AudioProcessor {
public:
    /**
     * Constructor
     * Sets the sampleRate and builds the UI
     * @param audioDriver
     */
    FaustAudioProcessor(AudioDriver &audioDriver)
        : AudioProcessor(audioDriver)
    {
        float currentSampleRate = audioDriver.getSampleRate();

        synth.init(currentSampleRate); // initializing the faust module
        synth.buildUserInterface(&control); // linking the faust module to the controler
    }

    /**
     * Process buffers
     */
    void process() override
    {
        audioBuffers[0] = getBuffer().getWritePointer(0);
        audioBuffers[1] = getBuffer().getWritePointer(1);

        synth.compute(getBufferSize(), NULL, audioBuffers); // computing one block with faust
    }

    /**
     * Set slider 1 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setSlider1(float value)
    {
        control.setParamValue(SLIDER1_NAME, SLIDER1_MIN + value * (SLIDER1_MAX - SLIDER1_MIN));
    }

    /**
     * Set slider 2 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setSlider2(float value)
    {
        control.setParamValue(SLIDER2_NAME, SLIDER2_MIN + value * (SLIDER2_MAX - SLIDER2_MIN));
    }

    /**
     * Set slider 3 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setSlider3(float value)
    {
        control.setParamValue(SLIDER3_NAME, SLIDER3_MIN + value * (SLIDER3_MAX - SLIDER3_MIN));
    }

    /**
     * Set slider 4 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setSlider4(float value)
    {
        control.setParamValue(SLIDER4_NAME, SLIDER4_MIN + value * (SLIDER4_MAX - SLIDER4_MIN));
    }

    /**
     * Set encoder 1 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setEncoder1(float value)
    {
        control.setParamValue(ENCODER1_NAME, ENCODER1_MIN + value * (ENCODER1_MAX - ENCODER1_MIN));
    }

    /**
     * Set encoder 2 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setEncoder2(float value)
    {
        control.setParamValue(ENCODER2_NAME, ENCODER2_MIN + value * (ENCODER2_MAX - ENCODER2_MIN));
    }

    /**
     * Set encoder 3 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setEncoder3(float value)
    {
        control.setParamValue(ENCODER3_NAME, ENCODER3_MIN + value * (ENCODER3_MAX - ENCODER3_MIN));
    }

    /**
     * Set encoder 4 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    inline void setEncoder4(float value)
    {
        control.setParamValue(ENCODER4_NAME, ENCODER4_MIN + value * (ENCODER4_MAX - ENCODER4_MIN));
    }

    /**
     * Set button 1 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    inline void setButton1(bool value)
    {
        control.setParamValue(BUTTON1_NAME, value);
    }

    /**
     * Set button 2 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    inline void setButton2(bool value)
    {
        control.setParamValue(BUTTON2_NAME, value);
    }

    /**
     * Set button 3 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    inline void setButton3(bool value)
    {
        control.setParamValue(BUTTON3_NAME, value);
    }

    /**
     * Set button 4S value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    inline void setButton4(bool value)
    {
        control.setParamValue(BUTTON4_NAME, value);
    }

    /**
     * MIDI Gate On function
     */
    inline void gateOn()
    {
        control.setParamValue("/faust_synth/gate", 1);
    }

    /**
     * MIDI Gate Off function
     */
    inline void gateOff()
    {
        control.setParamValue("/faust_synth/gate", 0);
    }

private:
    /**
     * Faust processor coming from the compilation of faust_synth.dsp
     */
    FaustSynth synth;

    /**
     * Faust UI control coming from the compilation of faust_synth.dsp
     */
    MapUI control;

    /**
     * Array containing the output buffer raw pointers to be passed to faust
     */
    float* audioBuffers[2];
};


#endif //MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
