#ifndef MIOSIX_DRUM_ENCODER_H
#define MIOSIX_DRUM_ENCODER_H
#include "miosix.h"

static void enableGPIORCC(GPIO_TypeDef* GPIO)
{
    if (GPIO == GPIOA)
        RCC->AHB1ENR |= (1 << 0);
    if (GPIO == GPIOB)
        RCC->AHB1ENR |= (1 << 1);
    if (GPIO == GPIOC)
        RCC->AHB1ENR |= (1 << 2);
    if (GPIO == GPIOD)
        RCC->AHB1ENR |= (1 << 3);
    if (GPIO == GPIOE)
        RCC->AHB1ENR |= (1 << 4);
    if (GPIO == GPIOF)
        RCC->AHB1ENR |= (1 << 5);
    if (GPIO == GPIOG)
        RCC->AHB1ENR |= (1 << 6);
    if (GPIO == GPIOH)
        RCC->AHB1ENR |= (1 << 7);
    if (GPIO == GPIOI)
        RCC->AHB1ENR |= (1 << 8);
}


static int getAFR(TIM_TypeDef* TIM)
{
    if (TIM == TIM1 || TIM == TIM2)
        return 1;
    if (TIM == TIM3 || TIM == TIM4 || TIM == TIM5)
        return 2;
    if (TIM == TIM8 || TIM == TIM9 || TIM == TIM10 || TIM == TIM11)
        return 3;
    else
        return 0;
}

static void enableTIMRCC(TIM_TypeDef* TIM)
{
    if (TIM == TIM1)
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    if (TIM == TIM2)
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    if (TIM == TIM3)
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    if (TIM == TIM4)
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    if (TIM == TIM5)
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
}

template <uint32_t TIM_BASE, uint32_t GPIO_BASE, int PIN1, int PIN2>
class Encoder {
public:
    static void init()
    {
        TIM_TypeDef* TIM = (TIM_TypeDef*) TIM_BASE;
        GPIO_TypeDef* GPIO = (GPIO_TypeDef*) GPIO_BASE;

        // GPIO Setup
        enableGPIORCC(GPIO);
        GPIO->MODER |= (2 << PIN1 * 2); // TIM Mode
        GPIO->MODER |= (2 << PIN2 * 2); // TIM Mode


        int afr = getAFR(TIM);
        GPIO->AFR[PIN1 / 8] |= (afr << (PIN1 % 8) * 4);
        GPIO->AFR[PIN2 / 8] |= (afr << (PIN2 % 8) * 4);

        // Encoder Timer Setup
        enableTIMRCC(TIM);

        // Value to count up to
        TIM->ARR = 0xFFFF;

        // Setup (RM9000, pg. 616)
        TIM->CCMR1 |= (TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0);
        TIM->CCER  |= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
        TIM->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;


        TIM->CCMR1 |= (3 << 4); // filtering

        TIM->CR1   |= TIM_CR1_CEN;
    }

    static int getValue()
    {
        return ((TIM_TypeDef*) TIM_BASE)->CNT >> 2;
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


#endif //MIOSIX_DRUM_ENCODER_H
