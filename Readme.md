# Drum Module based on STM32F407VG

## Introduction
This project aims at integrating a digital drum synthesizer using Miosix real time OS on an STM32F407VG Discovery Board.
To do this, a user interface by means of buttons, sliders, encoders, and an LCD has been implemented to control the synthesizer.
Moreover, a serial MIDI input port has been added in order to control the synthesizer externally by using an external sequencer or controller.

## Hardware Inputs
Most of the hardware input classes have been developed by using templated static classes in which we define the pins and other hardware related definitions through the template arguments, and are then initialized by calling an ```init()``` function. Each one of those class will later offer a method to retrieve its value by performing specific hardware related actions and setting the relative hardware registers.

### Potentiometer
The ADC reading has been abstracted through the means of a ```Potentiometer``` static class.  This class is templated, and needs the definition of
- GPIO_BASE: base address of the GPIO group needed
- PIN: GPIO input pin needed
- ADC_CHANNEL: ADC channel related to the chosen GPIO pin

Below a brief code example explains the usage of the ```Potentiometer``` class
```cpp
    // Definition
    typedef Potentiometer<GPIOA_BASE, 2, 2> slider;
    // Initialization
    slider::init();
    // Reading
    float value = slider::read();
```
