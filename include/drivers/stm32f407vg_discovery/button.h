#ifndef MIOSIX_DRUM_BUTTON_H
#define MIOSIX_DRUM_BUTTON_H
#include "../../../miosix/miosix.h"
#include "utility.h"

/**
 * Templated static class that allows to initialize
 * and get the state of a button at a given time
 * @tparam GPIO_BASE Which GPIO base to use
 * @tparam PIN  Which pin of the GPIO base
 */
template <uint32_t GPIO_BASE, int PIN>
class Button
{
public:
    /**
     * Function to be called to initialize the static class
     */
    static void init()
    {
        {
            miosix::FastInterruptDisableLock dLock;
            GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;

            // GPIO Setup
            GPIOUtility::enableRCC(GPIO);

            GPIO->MODER &= ~(3 << (PIN * 2)); // setting the GPIO in input mode
            GPIO->PUPDR &= ~(3 << (PIN * 2)); // reset
            GPIO->OTYPER &= ~(1 << PIN);      // reset
            GPIO->PUPDR |= (2 << (PIN * 2));  // pull-down
        }
    }

    /**
     * Get the current state of the button
     * @return true if the button is being pressed, false otherwise
     */
    static bool getState()
    {
        bool currentState;
        GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;
        {
            miosix::FastInterruptDisableLock dLock;
            currentState = (GPIO->IDR & (1 << PIN)) >> PIN;
        }

        return currentState;
    }

private:

    /**
     * Default constructor disabled
     */
    Button() = delete;

    /**
     * Copy constructor disabled
     */
    Button(const Button &) = delete;

    /**
     * Assignment operator disabled
     */
    Button &operator=(const Button &) = delete;
};

#endif //MIOSIX_DRUM_BUTTON_H
