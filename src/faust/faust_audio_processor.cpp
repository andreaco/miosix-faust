#include "../../include/faust/faust_audio_processor.h"

FaustAudioProcessor::FaustAudioProcessor(AudioDriver &audioDriver)
        : AudioProcessor(audioDriver) {
    float currentSampleRate = audioDriver.getSampleRate();

    synth.init(currentSampleRate); // initializing the faust module
    synth.buildUserInterface(&control); // linking the faust module to the controler
}

void FaustAudioProcessor::process() {
    audioBuffers[0] = getBuffer().getWritePointer(0);
    audioBuffers[1] = getBuffer().getWritePointer(1);

    synth.compute(getBufferSize(), NULL, audioBuffers); // computing one block with faust
}

void FaustAudioProcessor::setSlider1(float value) {
    control.setParamValue(SLIDER1_NAME, SLIDER1_MIN + value * (SLIDER1_MAX - SLIDER1_MIN));
}

void FaustAudioProcessor::setSlider2(float value) {
    control.setParamValue(SLIDER2_NAME, SLIDER2_MIN + value * (SLIDER2_MAX - SLIDER2_MIN));
}


void FaustAudioProcessor::setSlider3(float value) {
    control.setParamValue(SLIDER3_NAME, SLIDER3_MIN + value * (SLIDER3_MAX - SLIDER3_MIN));
}

void FaustAudioProcessor::setSlider4(float value) {
    control.setParamValue(SLIDER4_NAME, SLIDER4_MIN + value * (SLIDER4_MAX - SLIDER4_MIN));
}

void FaustAudioProcessor::setEncoder1(float value) {
    control.setParamValue(ENCODER1_NAME, ENCODER1_MIN + value * (ENCODER1_MAX - ENCODER1_MIN));
}

void FaustAudioProcessor::setEncoder2(float value) {
    control.setParamValue(ENCODER2_NAME, ENCODER2_MIN + value * (ENCODER2_MAX - ENCODER2_MIN));
}

void FaustAudioProcessor::setEncoder3(float value) {
    control.setParamValue(ENCODER3_NAME, ENCODER3_MIN + value * (ENCODER3_MAX - ENCODER3_MIN));
}

void FaustAudioProcessor::setEncoder4(float value) {
    control.setParamValue(ENCODER4_NAME, ENCODER4_MIN + value * (ENCODER4_MAX - ENCODER4_MIN));
}

void FaustAudioProcessor::setButton1(bool value) {
    control.setParamValue(BUTTON1_NAME, value);
}

void FaustAudioProcessor::setButton2(bool value) {
    control.setParamValue(BUTTON2_NAME, value);
}

void FaustAudioProcessor::setButton3(bool value) {
    control.setParamValue(BUTTON3_NAME, value);
}

void FaustAudioProcessor::setButton4(bool value) {
    control.setParamValue(BUTTON4_NAME, value);
}

void FaustAudioProcessor::gateOn() {
    control.setParamValue("/faust_synth/gate", 1);
}

void FaustAudioProcessor::gateOff() {
    control.setParamValue("/faust_synth/gate", 0);
}

