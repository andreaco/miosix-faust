#ifndef MIOSIX_DRUM_UTILITY_H
#define MIOSIX_DRUM_UTILITY_H

/**
 * Namespace containing utility functions related to GPIOs
 */
namespace GPIOUtility
{
    /**
     * Function used to enable the RCC to the relative  GPIO
     * @param GPIO Which GPIO will get its clock enabled
     */
    static void enableRCC(GPIO_TypeDef *GPIO)
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
}

/**
 * Namespace containing utility functions related to timers
 */
namespace TIMUtility {
    /**
     * Function used to retrieve the correct Alternate Function
     * for each different timer
     * @param TIM Timer that needs the related AFR value
     */
    static int getAFR(TIM_TypeDef *TIM)
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

    /**
     * Function used to enable the RCC to the relative Timer
     * @param TIM Which Timer will get its clock enabled
     */
    static
    static void enableRCC(TIM_TypeDef *TIM)
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
}

#endif //MIOSIX_DRUM_UTILITY_H
