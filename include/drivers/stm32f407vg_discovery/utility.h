#ifndef MIOSIX_DRUM_UTILITY_H
#define MIOSIX_DRUM_UTILITY_H
#include "../../../miosix/miosix.h"

/**
 * Namespace containing utility functions related to GPIOs
 */
namespace GPIOUtility
{
    /**
     * Function used to enable the RCC to the relative  GPIO
     * @param GPIO Which GPIO will get its clock enabled
     */
    void enableRCC(GPIO_TypeDef *GPIO);
}

/**
 * Namespace containing utility functions related to timers
 */
namespace TIMUtility
{
    /**
     * Function used to retrieve the correct Alternate Function
     * for each different timer
     * @param TIM Timer that needs the related AFR value
     */
    int getAFR(TIM_TypeDef *TIM);

    /**
     * Function used to enable the RCC to the relative Timer
     * @param TIM Which Timer will get its clock enabled
     */
    void enableRCC(TIM_TypeDef *TIM);
}

#endif //MIOSIX_DRUM_UTILITY_H
