#ifndef MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#define MIOSIX_DRUM_FAUST_AUDIOPROCESSOR_H
#include "../audio/audio_processor.h"
#include "config/audio_config.h"
#include "config/parameter_config.h"
#include "faust_synth.h"

class FaustAudioProcessor : public AudioProcessor {
public:
    /**
     * Constructor
     * Sets the sampleRate and builds the UI
     * @param audioDriver
     */
    FaustAudioProcessor(AudioDriver &audioDriver);

    /**
     * Process buffers
     */
    void process() override;

    /**
     * Set slider 1 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setSlider1(float value);

    /**
     * Set slider 2 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setSlider2(float value);

    /**
     * Set slider 3 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setSlider3(float value);

    /**
     * Set slider 4 value, mappings between slider and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setSlider4(float value);

    /**
     * Set encoder 1 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setEncoder1(float value);

    /**
     * Set encoder 2 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setEncoder2(float value);

    /**
     * Set encoder 3 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setEncoder3(float value);

    /**
     * Set encoder 4 value, mappings between encoder and faust parameters
     * can be made through the parameter_config.h file
     * @param value Slider value between 0 and 1
     */
    void setEncoder4(float value);

    /**
     * Set button 1 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    void setButton1(bool value);

    /**
     * Set button 2 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    void setButton2(bool value);

    /**
     * Set button 3 value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    void setButton3(bool value);

    /**
     * Set button 4S value, mappings between button and faust parameters
     * can be made through the parameter_config.h file
     * @param value button state (true if On, false if Off)
     */
    void setButton4(bool value);

    /**
     * MIDI Gate On function
     */
    void gateOn();

    /**
     * MIDI Gate Off function
     */
    void gateOff();

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
