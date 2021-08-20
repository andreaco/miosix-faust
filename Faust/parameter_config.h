#ifndef MIOSIX_DRUM_PARAMETER_CONFIG_H
#define MIOSIX_DRUM_PARAMETER_CONFIG_H

/**
 * This file is used to link Faust Parameters to physical ones
 */


/**
 * SLIDERS
 */
#define SLIDER1_NAME "/FaustSynth/A"
#define SLIDER1_MIN 0.001f
#define SLIDER1_MAX 2.000f

#define SLIDER2_NAME "/FaustSynth/D"
#define SLIDER2_MIN 0.001f
#define SLIDER2_MAX 2.000f

#define SLIDER3_NAME "/FaustSynth/S"
#define SLIDER3_MIN 0.001f
#define SLIDER3_MAX 1.000f

#define SLIDER4_NAME "/FaustSynth/R"
#define SLIDER4_MIN 0.001f
#define SLIDER4_MAX 2.000f

/**
 * ENCODERS
 */

#define ENCODER1_NAME "/FaustSynth/freq"
#define ENCODER1_MIN 20.0f
#define ENCODER1_MAX 2000.0f

#define ENCODER2_NAME "/FaustSynth/ratio"
#define ENCODER2_MIN 0.0f
#define ENCODER2_MAX 20.0f

#define ENCODER3_NAME "/FaustSynth/distortion"
#define ENCODER3_MIN 0.0f
#define ENCODER3_MAX 100.0f

#define ENCODER4_NAME "/FaustSynth/gain"
#define ENCODER4_MIN 0.0f
#define ENCODER4_MAX +96.0f

#endif //MIOSIX_DRUM_PARAMETER_CONFIG_H
