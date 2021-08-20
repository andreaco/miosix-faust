#include <cstdint>
#include <thread>
#include <util/lcd44780.h>
#include "miosix.h"
#include "drivers/common/audio.h"
#include "drivers/common/lcd_interface.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include "drivers/stm32f407vg_discovery/button.h"
#include "drivers/stm32f407vg_discovery/potentiometer.h"
#include "drivers/stm32f407vg_discovery/midi_in.h"
#include "faust/faust_audio_processor.h"
#include "midi/midi_parser.h"
#include "config/thread_update_rates.h"


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
 * ADC Pin Definition
 */
typedef Potentiometer<GPIOA_BASE, 2, 2> slider1;
typedef Potentiometer<GPIOA_BASE, 5, 5> slider2;
typedef Potentiometer<GPIOA_BASE, 6, 6> slider3;
typedef Potentiometer<GPIOA_BASE, 7, 7> slider4;

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
 * Audio Driver and Synthesizer declaration
 */
static AudioDriver audioDriver;
static FaustAudioProcessor synth(audioDriver);

/**
 * Midi Parser declaration and initialization
 */
static MidiParser midiParser;

/**
 * LCD Initialization, declaration of shared LCDPage and its mutex
 */
static miosix::Lcd44780 display(rs::getPin(), e::getPin(), d4::getPin(),
                                d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);
LCDUtils::LCDPage lcdPage;
miosix::Mutex lcdMutex;

/**
 * Slider UI Thread Function
 */
void sliderUI()
{
    // Sliders Initialization
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
        miosix::Thread::sleep(SLIDER_SLEEP_TIME);
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
        // Read from the encoders
        float e1 = encoder1::getValue();
        float e2 = encoder2::getValue();
        float e3 = encoder3::getValue();
        float e4 = encoder4::getValue();

        // Set encoder to synth
        synth.setEncoder1(e1);
        synth.setEncoder2(e2);
        synth.setEncoder3(e3);
        synth.setEncoder4(e4);

        // Update shared variable lcdPage to reflect the changes on the LCD
        {
            miosix::Lock<miosix::Mutex> l(lcdMutex);
            lcdPage.p[0].value = (int) (e1 * 999);
            lcdPage.p[1].value = (int) (e2 * 999);
            lcdPage.p[2].value = (int) (e3 * 999);
            lcdPage.p[3].value = (int) (e4 * 999);
        }
        miosix::Thread::sleep(ENCODER_SLEEP_TIME);
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
        synth.setButton1(button1::getState());
        synth.setButton2(button2::getState());
        synth.setButton3(button3::getState());
        synth.setButton4(button4::getState());
        miosix::Thread::sleep(BUTTON_SLEEP_TIME);
    }
}

/**
 * LCD update UI Thread Function
 */
void lcdUI()
{
    // Setup
    lcdPage.p[0].name = ENCODER1_LCD_NAME;
    lcdPage.p[1].name = ENCODER2_LCD_NAME;
    lcdPage.p[2].name = ENCODER3_LCD_NAME;
    lcdPage.p[3].name = ENCODER4_LCD_NAME;

    while(true)
    {
        {
            miosix::Lock<miosix::Mutex> l(lcdMutex);
            LCDUtils::lcdPrintPage(display, lcdPage);
            miosix::Thread::sleep(LCD_SLEEP_TIME);
        }
    }
}

/**
 * Serial MIDI in read (blocking call)
 * and parsing of read bytes
 */
void midiParsing()
{
    MidiIn midiIn;

    uint8_t byte;
    while(true)
    {
        uint8_t status = midiIn.read(&byte);
        if(status > 0)
            midiParser.parseByte(byte);
    }
}

/**
 * Processing of MIDI data
 */
void midiProcessing()
{
    while(true)
    {
        if (midiParser.isNoteAvaiable())
        {
            MidiNote note = midiParser.popNote();
            if (note.msgType == MidiNote::NOTE_ON && note.velocity > (uint8_t) 0)
                synth.gateOn();
            else if (note.msgType == MidiNote::NOTE_OFF || note.velocity == 0)
                synth.gateOff();
        }
        miosix::Thread::sleep(MIDI_SLEEP_TIME);
    }
}




int main()
{
    // Audio Driver initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);

    // Hardware UI Threads
    std::thread encoderUIThread(encoderUI);
    std::thread buttonUIThread(buttonUI);
    std::thread sliderUIThread(sliderUI);
    std::thread lcdUIThread(lcdUI);

    // MIDI Threads
    std::thread midiParsingThread(midiParsing);
    std::thread midiProcessingThread(midiProcessing);

    // Audio Thread
    audioDriver.start();
}