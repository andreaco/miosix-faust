#ifndef MIOSIX_DRUM_ENCODER_H
#define MIOSIX_DRUM_ENCODER_H
#include <miosix.h>
#include "drivers/stm32f407vg_discovery/utility.h"

template <uint32_t TIM_BASE, uint32_t GPIO_BASE, int PIN1, int PIN2>
class Encoder {
public:
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


            uint8_t AFR = TIMUtility::getAFR(TIM);
            GPIO->AFR[PIN1 / 8] |= (AFR << (PIN1 % 8) * 4);
            GPIO->AFR[PIN2 / 8] |= (AFR << (PIN2 % 8) * 4);

            // Encoder Timer Setup
            TIMUtility::enableRCC(TIM);

            // Value to count up to
            TIM->ARR = 0xFFFF;

            // Setup (RM9000, pg. 616)
            TIM->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
            TIM->CCER |= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
            TIM->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;


            TIM->CCMR1 |= (3 << 4); // filtering

            TIM->CR1 |= TIM_CR1_CEN;
        }
    }

    static int getValue()
    {
        {
            miosix::FastInterruptDisableLock dLock;
            return ((TIM_TypeDef *) TIM_BASE)->CNT >> 2;
        }
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
};
/*
template <uint32_t GPIO_BASE, int PIN>
bool Button<GPIO_BASE, PIN>::previousState = false;

*/

#endif //MIOSIX_DRUM_ENCODER_H
