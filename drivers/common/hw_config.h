
#ifndef MIOSIX_AUDIO_HW_CONFIG_H
#define MIOSIX_AUDIO_HW_CONFIG_H

/**
 * This header is used to modify the hardware
 * configuration of the system.
 */

/**
 * Number of samples to be averaged by the ADC to stabilize the read value.
 */
#define ADC_AVG_SAMPLES (8)

/**
 * ADC Resolution
 * Set to the desired values (x) to get the desired resolution
 * if (0): 12-bit (15 ADCCLK cycles)
 * if (1): 10-bit (13 ADCCLK cycles)
 * if (2): 8-bit  (11 ADCCLK cycles)
 * if (3): 6-bit  ( 9 ADCCLK cycles)
 */
#define ADC_RESOLUTION (8)


#endif //MIOSIX_AUDIO_HW_CONFIG_H
