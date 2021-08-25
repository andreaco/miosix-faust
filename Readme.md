# Drum Module based on STM32F407VG
[Documentation](https://andreaco.github.io/miosix-drum/)

## Introduction
This project aims at integrating a digital drum synthesizer using Miosix real time OS on an STM32F407VG Discovery Board.
To do this, a user interface by means of buttons, sliders, encoders, and an LCD has been implemented to control the synthesizer.
Moreover, a serial MIDI input port has been added in order to control the synthesizer externally by using an external sequencer or controller.

## Faust Integration
The audio is implemented by compiling the Faust script inside the ```src/faust/faust_synth.dsp```.
It is possible to map custom parameters to the hardware inputs by editing the ```parameter_config.h``` file.
Here is a brief example showing the mapping between a Faust parameter and an hardware periphereal.
```cpp
    // Faust Script
    ...
    freq = nentry("freq[unit:Hz]", 440, 20, 20000, 1);
    ...
```

```cpp
    // Parameter Config File
    ...
    #define ENCODER1_NAME "/faust_synth/freq"
    #define ENCODER1_MIN 20.0f
    #define ENCODER1_MAX 200.0f
    #define ENCODER1_LCD_NAME "FRQ"
    ...
```
Note that the value currently fetched by the encoder is displayed on the HD44780 LCD, and it is possible to specify a custom name composed by 3 letters.
The result is shown in the following figure.

![lcd](https://user-images.githubusercontent.com/33195819/130662372-f4fb3494-0fb2-4cf9-874b-a74850180bae.jpg)

It is currently under development in another branch the native integration of the MIDI CC inputs from Faust scripts.
Note that if you need a mapping between the faust script and MIDI you should edit the ```midiProcessing``` thread function, handling the messages manually.


## Hardware Inputs
Most of the hardware input classes have been developed by using templated static classes in which we define the pins and other hardware related definitions through the template arguments, and are then initialized by calling an ```init()``` function. Each one of those class will later offer a method to retrieve its value by performing specific hardware related actions and setting the relative hardware registers.

### Potentiometer
The ADC reading has been abstracted through the means of a ```Potentiometer``` static class.  This class is templated, and needs the definition of
- ```GPIO_BASE```: base address of the GPIO group needed
- ```PIN```: GPIO input pin needed
- ```ADC_CHANNEL```: ADC channel related to the chosen GPIO pin

Below a brief code example explains the usage of the ```Potentiometer``` class.
```cpp
    // Definition
    typedef Potentiometer<GPIOA_BASE, 2, 2> slider;
    // Initialization
    slider::init();
    // Reading
    float value = slider::read();
```

### Encoder
The ```Encoder``` class is an abstraction to the STM32 timers in encoder mode.
Similarly to the ```Potentiometer``` class, this class is templated and needs the definition of:
- ```TIM_BASE```: timer address to be linked to the encoder.
- ```GPIO_BASE```: base address of the GPIO group needed.
- ```PIN1```: pin 1 connected to the encoder.
- ```PIN2```: pin 2 connected to the encoder.

Analogously to the potentiometer class, the initialization of the encoder is performed by calling the static function \verb!init()!.
In the following code snippet a basic usage of this class is shown.
```cpp
    // Definition
    typedef Encoder<TIM1_BASE, GPIOE_BASE, 9, 11> encoder;
    // Initialization
    encoder::init();
    // Simple Reading
    float value = encoder::getValue();
    // External Reading
    float externalValue1 = 0;
    float externalValue2 = 0;
    if (STATE == PAGE1)
        externalValue1 += encoder::getIncrement();
    if (STATE == PAGE2)
        externalValue2 += encoder::getIncrement();
```

### Button
A class for reading buttons has being defined in a similar fashion as the previous two.
The static class ```Button``` is templated and needs just the definition of:
- ```GPIO_BASE``` GPIO address to be used.
- ```PIN```: GPIO pin to read from.

As before, we need to initialize it by using the static method ```init()``` which simply enables the GPIO RCC timer and set its register to input mode with pull-down option.
Here is a code snippet showing its usage:
```cpp
    // Definition
    typedef Button<GPIOD_BASE, 0> button;
    // Initialization
    button::init();
    // Reading
    bool value = button::getState();
```

### MIDI
The ```MidiIn``` class, is simply a wrapper of the class ```miosix::STM32Serial```.
It calls the constructor by using the selected standard input to be used which can be chosen by editing the macro ```MIDI_SERIAL_ID``` in the ```hw_config.h``` header. It then allows reading one byte from it by means of a function ```read(uint8_t *byte)```.
To parse the incoming stream of data from the ```MidiIn``` port, a ```MidiParser``` class has been implemented. As of now the class have implemented only the parsing of MIDI Note On, Note Off and Control Change messages, which are represented by two different structs respectively.
The class is featured with two ```CircularBuffer``` classes with discarding policy, one for the Note messages and one for the Control Change messages. The size of each of those buffers can be set by editing the ```hw_config.h``` file.
Here follows two code snippets, one displaying the usage of the ```MidiIn``` class, the other the consumption of the data produced by the parser.
```cpp
    uint8_t byte;
    uint8_t status = midiIn.read(&byte); // Blocking call
    if (status > 0)
        midiParser.parseByte(byte);
```
```cpp
     // MIDI Note consumption
    if (midiParser.isNoteAvaiable()) {
        MidiNote note = midiParser.popNote();
        if (note.msgType == MidiNote::NOTE_ON)
            synth.gateOn();
        if (note.msgType == MidiNote::NOTE_OFF)
            synth.gateOff();
    }
    // MIDI CC consumption
    if (midiParser.isCCAvaiable()) {
        ControlChange cc = midiParser.popCC();
        if(cc.controlNumber == 42)
            synth.setFoo(cc.value);
        if(cc.controlNumber == 33)
            synth.setBar(cc.value);
    }
```
