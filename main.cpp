#include "miosix.h"
#include "drivers/common/audio.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include "drivers/stm32f407vg_discovery/button.h"
#include "audio/audio_processor.h"
#include "drivers/stm32f407vg_discovery/adc_reader.h"
#include "Synth/Synth.h"
#include <cstdint>
#include <util/lcd44780.h>
#include <thread>


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
 * Encoders Pin Definition
 */
typedef Encoder<TIM1_BASE, GPIOE_BASE,  9, 11> encoder1;
typedef Encoder<TIM3_BASE, GPIOB_BASE,  4,  5> encoder2;
typedef Encoder<TIM4_BASE, GPIOD_BASE, 12, 13> encoder3;
typedef Encoder<TIM5_BASE, GPIOA_BASE,  0,  1> encoder4;

/**
 * Buttons Pin Definition
 */
typedef Button<GPIOD_BASE, 1> button1;
typedef Button<GPIOD_BASE, 2> button2;
typedef Button<GPIOD_BASE, 3> button3;
typedef Button<GPIOD_BASE, 4> button4;

/**
 * LCD Initialization
 */
static miosix::Lcd44780 display(rs::getPin(), e::getPin(), d4::getPin(),
                                d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);

/**
 * Audio Driver and Synthesizer declaration
 */
AudioDriver audioDriver;
Synth synth(audioDriver);

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

miosix::FastMutex m;

/**
 * Hardware UI Thread Function
 */
void hardwareUIThreadFunction() {
    // ADC Initialization
    AdcReader::init();

    // Buttons Initialization
    button1::init();
    button2::init();
    button3::init();
    button4::init();

    // Encoders Initialization
    encoder1::init();
    encoder2::init();
    encoder3::init();
    encoder4::init();

    uint16_t frequency;
    float morph;
    float attackTimeMs;
    float releaseTimeMs;
    float e3, e4;
    bool gate;

    while (true) {
        {
            auto values = AdcReader::readAll();
            attackTimeMs = 2 * values[0] / 127.0f;
            releaseTimeMs = 2 * values[1] / 127.0f;

            gate = button1::getState();
            frequency = (float) encoder1::getValue();
            morph = encoder2::getValue();
            e3 = encoder3::getValue();
            e4 = encoder4::getValue();
        }

        if (gate)
            synth.gate();

        synth.setFrequency(frequency);
        morph = morph / 10.0f;
        synth.setMorph(morph);
        synth.setAttackTime(attackTimeMs);
        synth.setReleaseTime(releaseTimeMs);

        miosix::Thread::sleep(20);
    }
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

int main() {
    // Audio Driver and Synth initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);

    // Hardware UI Thread
    std::thread hardwareInterfaceThread(hardwareUIThreadFunction);
    //std::thread lcdThread(lcdThreadF);

    // Audio Thread
    audioDriver.start();
}
