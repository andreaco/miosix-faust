#include <cstdint>
#include <thread>
#include "miosix.h"
#include "drivers/common/audio.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include "drivers/stm32f407vg_discovery/button.h"
#include "drivers/stm32f407vg_discovery/potentiometer.h"
#include "audio/audio_processor.h"
#include "Faust/FaustAudioProcessor.h"
#include "drivers/stm32f407vg_discovery/midi_in.h"
#include "midi/midi_parser.h"

/**
 * ADC Pin Definition
 */
typedef Potentiometer<GPIOA_BASE, 2, 2> slider1;
typedef Potentiometer<GPIOA_BASE, 5, 5> slider2;
typedef Potentiometer<GPIOA_BASE, 6, 6> slider3;
typedef Potentiometer<GPIOA_BASE, 7, 7> slider4;

/**
 * Encoders Pin Definition
 */
typedef Encoder<TIM1_BASE, GPIOE_BASE,  9, 11> encoder1;
typedef Encoder<TIM3_BASE, GPIOB_BASE,  4,  5> encoder2;
typedef Encoder<TIM4_BASE, GPIOD_BASE, 12, 13> encoder3;
typedef Encoder<TIM5_BASE, GPIOA_BASE,  0,  1> encoder4;

/**
 * Buttons Pin Definition
 */
typedef Button<GPIOD_BASE, 0> button1;
typedef Button<GPIOD_BASE, 1> button2;
typedef Button<GPIOD_BASE, 2> button3;
typedef Button<GPIOD_BASE, 3> button4;


/**
 * Audio Driver and Synthesizer declaration
 */
AudioDriver audioDriver;
Faust_AudioProcessor synth(audioDriver);

/**
 * Midi Parser declaration and initialization
 */
static MidiParser midiParser;

/**
 * Slider UI Thread Function
 */
void sliderUI()
{
    slider1::init();
    slider2::init();
    slider3::init();
    slider4::init();

    while (true)
    {
        synth.setSlider1(slider1::read());
        synth.setSlider2(slider2::read());
        synth.setSlider3(slider3::read());
        synth.setSlider4(slider4::read());
        miosix::Thread::sleep(200);
    }

}

/**
 * Encoder UI Thread Function
 */
void encoderUI()
{
    // Encoders Initialization
    encoder1::init();
    encoder2::init();
    encoder3::init();
    encoder4::init();

    while (true)
    {
        synth.setEncoder1(encoder1::getValue());
        synth.setEncoder2(encoder2::getValue());
        synth.setEncoder3(encoder3::getValue());
        synth.setEncoder4(encoder4::getValue());

        miosix::Thread::sleep(50);
    }
}

/**
 * Encoder UI Thread Function
 */
void buttonUI()
{
    // Buttons Initialization
    button1::init();
    button2::init();
    button3::init();
    button4::init();

    while (true)
    {
        button1::getState();
        button2::getState();
        button3::getState();
        button4::getState();
        miosix::Thread::sleep(10);
    }
}


void midiParsing()
{
    MidiIn midiIn;

    uint8_t status;
    uint8_t byte;
    while(true)
    {
        uint8_t status = midiIn.read(&byte);
        if(status > 0)
            midiParser.parseByte(byte);
    }
}

void midiProcessing()
{
    while(true)
    {
        if (midiParser.isNoteAvaiable())
        {
            MidiNote note = midiParser.popNote();
            if (note.msgType == MidiNote::NOTE_ON)
                synth.gateOn();
            else
                synth.gateOff();
        }
        miosix::Thread::sleep(20);
    }

}

int main()
{
    // Audio Driver and Faust initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);

    // Hardware UI Threads
    std::thread encoderUIThread(encoderUI);
    std::thread buttonUIThread(buttonUI);
    std::thread sliderUIThread(sliderUI);

    // MIDI Threads
    std::thread midiParsingThread(midiParsing);
    std::thread midiProcessingThread(midiProcessing);

    // Audio Thread
    audioDriver.start();
}

#if 0
#include <util/lcd44780.h>
/**
 * LCD Pin Definition
 */
typedef miosix::Gpio<GPIOB_BASE, 12> d4;
typedef miosix::Gpio<GPIOB_BASE, 13> d5;
typedef miosix::Gpio<GPIOB_BASE, 14> d6;
typedef miosix::Gpio<GPIOB_BASE, 15> d7;
typedef miosix::Gpio<GPIOC_BASE, 1> rs;
typedef miosix::Gpio<GPIOC_BASE, 2> e;

/**
 * LCD Initialization
 */
static miosix::Lcd44780 display(rs::getPin(), e::getPin(), d4::getPin(),
                                d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);

struct LCDParameter
{
    std::string name;
    int value;
};

void lcdPage(miosix::Lcd44780& lcd,
             LCDParameter& p1,
             LCDParameter& p2,
             LCDParameter& p3,
             LCDParameter& p4)
{
    display.go(0, 0);
    display.printf(" %s %s %s %s",
                   p1.name.c_str(),
                   p2.name.c_str(),
                   p3.name.c_str(),
                   p4.name.c_str());

    display.go(0, 1);
    display.printf(" %3d %3d %3d %3d",
                   p1.value,
                   p2.value,
                   p3.value,
                   p4.value);
}



void lcdThreadF()
{
    while(true) {
        {
            int e1 = 1;
            int e2 = 2;
            int e3 = 3;
            int e4 = 4;

            LCDParameter p1{"En1", (int) e1};
            LCDParameter p2{"En2", (int) e2};
            LCDParameter p3{"En3", (int) e3};
            LCDParameter p4{"En4", (int) e4};

            LCDParameter lp;
            lcdPage(display, p1, p2, p3, p4);
            miosix::Thread::sleep(1000);
        }
    }
}
#endif
