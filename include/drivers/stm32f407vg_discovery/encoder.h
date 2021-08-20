#ifndef MIOSIX_DRUM_ENCODER_H
#define MIOSIX_DRUM_ENCODER_H
#include "../../../miosix/miosix.h"
#include <math.h>
#include "utility.h"

/**
 * Templated static class that allows to initialize
 * and get the state of an encoder at any given time
 * @tparam TIM_BASE Timer to be used
 * @tparam GPIO_BASE GPIO base for pin 1 and 2
 * @tparam PIN1 Pin 1
 * @tparam PIN2 Pin 2
 */
template <uint32_t TIM_BASE, uint32_t GPIO_BASE, int PIN1, int PIN2>
class Encoder {
public:
    /**
     * Function to be called to initialize the static class
     */
    static void init()
    {
        TIM_TypeDef* TIM = (TIM_TypeDef*) TIM_BASE;
        GPIO_TypeDef* GPIO = (GPIO_TypeDef*) GPIO_BASE;

        {
            miosix::FastInterruptDisableLock dLock;

            // GPIO Setup
            GPIOUtility::enableRCC(GPIO);
            GPIO->MODER |= (2 << PIN1 * 2); // TIM Mode
            GPIO->MODER |= (2 << PIN2 * 2); // TIM Mode

            // Set Alternate Function RegisterS
            uint8_t AFR = TIMUtility::getAFR(TIM);
            GPIO->AFR[PIN1 / 8] |= (AFR << (PIN1 % 8) * 4);
            GPIO->AFR[PIN2 / 8] |= (AFR << (PIN2 % 8) * 4);

            // Encoder Timer Setup
            TIMUtility::enableRCC(TIM);

            // Value to count up to
            TIM->ARR = 0xFFFF;
            TIM->CNT = 0x7FFF;

            // Setup (RM9000, pg. 616)
            TIM->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
            TIM->CCER |= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
            TIM->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

            // filtering
            TIM->CCMR1 |= (3 << 4);

            // Enable
            TIM->CR1 |= TIM_CR1_CEN;
        }
    }

    /**
     * Get the current value held by the encoder
     * @return Current value
     */
    static float getValue()
    {
        // Update value
        value += getIncrement();

        // Clip value
        value = std::min(std::max(value, 0.0f), 1.0f);
        return value;
    }

    /**
     * Get how much the encoder has incremented/decremented
     * Useful for updating different variables from a single encoder
     * @return Current increment
     */
    static float getIncrement()
    {
        TIM_TypeDef* TIM = (TIM_TypeDef*) TIM_BASE;
        uint16_t counterValue;
        {
            miosix::FastInterruptDisableLock dLock;
            // Read Counter
            counterValue = TIM->CNT;
            // Reset Counter
            TIM->CNT = 0x7FFF;
        }
        return (counterValue - 0x7FFF) / 1000.0f;
    }

private:
    /**
     * Default constructor disabled
     */
    Encoder() = delete;

    /**
     * Copy constructor disabled
     */
    Encoder(const Encoder &);

    /**
     * Assignment operator disabled
     */
    Encoder &operator=(const Encoder &);

    /**
     * Current Stored Value
     */
    static float value;
};

/**
 * Static initialization of associated float value
 */
template <uint32_t TIM_BASE, uint32_t GPIO_BASE, int PIN1, int PIN2>
float Encoder<TIM_BASE, GPIO_BASE, PIN1, PIN2>::value = 0.0f;

#endif //MIOSIX_DRUM_ENCODER_H
