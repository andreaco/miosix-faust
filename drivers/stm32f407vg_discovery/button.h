#ifndef MIOSIX_DRUM_BUTTON_H
#define MIOSIX_DRUM_BUTTON_H

template <uint32_t GPIO_BASE, int PIN>
class Button {
public:
    static void init() {
        {
            miosix::FastInterruptDisableLock dLock;
            GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;

            // GPIO Setup
            // TODO: GLOBAL ENABLEGPIO
            enableGPIORCC(GPIO);

            GPIO->MODER &= ~(3 << (PIN * 2)); // setting the GPIO in input mode
            GPIO->PUPDR &= ~(3 << (PIN * 2)); // reset
            GPIO->OTYPER &= ~(1 << PIN);       // reset
            GPIO->PUPDR |= (2 << (PIN * 2));  // pull-down
        }
    }

    static bool getState() {
        GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;
        {
            miosix::FastInterruptDisableLock dLock;
            previousState = (GPIO->IDR & (1 << 1)) >> 1;
        }
        return previousState;
    }

    static bool risingEdge()
    {
        bool currentState;
        GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;
        {
            miosix::FastInterruptDisableLock dLock;
            currentState = (GPIO->IDR & (1 << 1)) >> 1;
        }
        bool isRising = false;
        if (previousState == false && currentState == true)
            isRising = true;
        previousState = currentState;
        return isRising;
    }

    static bool fallingEdge()
    {
        GPIO_TypeDef *GPIO = (GPIO_TypeDef *) GPIO_BASE;
        bool currentState;
        {
            miosix::FastInterruptDisableLock dLock;
            currentState = (GPIO->IDR & (1 << 1)) >> 1;
        }
        bool isFalling = false;
        if (previousState == true && currentState == false)
            isFalling = true;
        previousState = currentState;
        return isFalling;
    }
private:
    static bool previousState;
};

template <uint32_t GPIO_BASE, int PIN>
bool Button<GPIO_BASE, PIN>::previousState = false;

#endif //MIOSIX_DRUM_BUTTON_H
