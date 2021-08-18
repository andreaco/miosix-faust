#ifndef MIOSIX_DRUM_POTENTIOMETER_H
#define MIOSIX_DRUM_POTENTIOMETER_H

#include "miosix.h"
#include "../common/hw_config.h"
#include "utility.h"


template <uint32_t GPIO_BASE, uint16_t PIN, uint8_t ADC_CHANNEL>
class Potentiometer
{
public:

    /**
     * Static function used to initialize the ADC and the relative pins
     */
    static void init()
    {
        GPIO_TypeDef* GPIO = (GPIO_TypeDef*) GPIO_BASE;
        {
            miosix::FastInterruptDisableLock dLock;

            // Enable ADC1 Clock
            RCC->APB2ENR |= (1 << 8);

            // Enable GPIO clock
            GPIOUtility::enableRCC(GPIO);

            // Prescaler
            ADC->CCR = (2 << 16);

            // Resolution
            ADC1->CR1 |= (1 << 8);  // Scan Mode
            ADC1->CR1 |= (2 << 24); // 8-bit resolution
            //ADC1->CR1 |= (0 << 24); // 12-bit resolution

            // Data Alignment
            ADC1->CR2 &= ~(1 << 11); // Left aligned
            ADC1->CR2 |= (1 << 10);  // EOC bit is set at the end of each regular conversion
            ADC1->CR2 |= (1 << 1);   // Continuous conversion mode

            // Sampling time for adc channel
            ADC1->SMPR2 |= (1 << 3 * ADC_CHANNEL);

            // Sequence for 1 channel conversions
            ADC1->SQR1 |= (1 << 1);

            // Analog Mode
            GPIO->MODER |= (3 << PIN * 2);

            //Enable
            ADC1->CR2 |= 1 << 0;

            // Wait to stabilize ADC
            miosix::delayUs(100);
        }
    }

    /**
     * Read values from the ADC and update the current state
     */
    static uint16_t read()
    {
        uint32_t result = 0;
        for (int i = 0; i < ADC_AVG_SAMPLES; ++i)
        {
            miosix::FastInterruptDisableLock dLock;
            result += readChannel();
        }
        result = result / (float) ADC_AVG_SAMPLES;

        return result >> 1;
    }

private:

    /**
     * Performs a single read from the specified channel
     * @param channel Channel in which to perform the read operation
     * @return Value read from the ADC channel
     */
    static uint16_t readChannel()
    {
        // Clear sequence register
        ADC1->SQR3 = 0;

        // Conversion of just one channel in a regular sequence
        ADC1->SQR3 |= (ADC_CHANNEL << 0);

        // Clear status register
        ADC1->SR = 0;

        // start the conversion
        ADC1->CR2 |= (1<<30);

        //wait for the conversion to finish (EOC)
        while (!(ADC1->SR & (1<<1)));

        //Extract val from DataRegister
        return  (ADC1->DR) ;
    }

    /**
     * Static class, constructor disabled
     */
    Potentiometer() = delete;

    /**
     * Static class, copy constructor disabled
     */
    Potentiometer(const Potentiometer &) = delete;

    /**
     * Static class, assignment operator disabled
     */
    Potentiometer &operator=(const Potentiometer &) = delete;
};

#endif //MIOSIX_DRUM_POTENTIOMETER_H
