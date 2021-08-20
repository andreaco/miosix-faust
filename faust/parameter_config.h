#ifndef MIOSIX_DRUM_PARAMETER_CONFIG_H
#define MIOSIX_DRUM_PARAMETER_CONFIG_H

/**
 * This file is used to link faust Parameters to physical ones
 */


/**
 * SLIDERS
 */
#define SLIDER1_NAME "/faust_synth/A"
#define SLIDER1_MIN 0.001f
#define SLIDER1_MAX 2.000f

#define SLIDER2_NAME "/faust_synth/D"
#define SLIDER2_MIN 0.001f
#define SLIDER2_MAX 2.000f

#define SLIDER3_NAME "/faust_synth/S"
#define SLIDER3_MIN 0.001f
#define SLIDER3_MAX 1.000f

#define SLIDER4_NAME "/faust_synth/R"
#define SLIDER4_MIN 0.001f
#define SLIDER4_MAX 2.000f

/**
 * ENCODERS
 */

#define ENCODER1_NAME "/faust_synth/freq"
#define ENCODER1_MIN 20.0f
#define ENCODER1_MAX 200.0f

#define ENCODER2_NAME "/faust_synth/ratio"
#define ENCODER2_MIN 0.0f
#define ENCODER2_MAX 20.0f

#define ENCODER3_NAME "/faust_synth/distortion"
#define ENCODER3_MIN 0.0f
#define ENCODER3_MAX 100.0f

#define ENCODER4_NAME "/faust_synth/gain"
#define ENCODER4_MIN 0.0f
#define ENCODER4_MAX +96.0f

#endif //MIOSIX_DRUM_PARAMETER_CONFIG_H
