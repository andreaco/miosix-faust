
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
#define ADC_RESOLUTION (1)

/**
 * For STM32F407VG
 * (1): USART1: tx=PA9  rx=PA10 cts=PA11 rts=PA12
 * (2): USART2: tx=PA2  rx=PA3  cts=PA0  rts=PA1
 * (3): USART3: tx=PB10 rx=PB11 cts=PB13 rts=PB14
 */
#define MIDI_SERIAL_ID (1)

/**
 * MIDI notes circular buffer size
 */
#define MIDI_NOTE_MESSAGE_QUEUE_SIZE (32)

/**
 * MIDI CC circular buffer size
 */
#define CC_MESSAGE_QUEUE_SIZE (32)

#endif //MIOSIX_AUDIO_HW_CONFIG_H
