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
    uint16_t frequency;
    float morph;
    float attackTimeMs;
    float releaseTimeMs;
    float e3, e4;
    bool gate;

    while (true)
    {
        {
            miosix::Lock<miosix::FastMutex> l(m);

            auto values = AdcReader::readAll();
            attackTimeMs = 2 * values[0] / 127.0f;
            releaseTimeMs = 2 * values[1] / 127.0f;

            gate = button1::getState();
            frequency = (float) encoder1::getValue();
            morph = encoder2::getValue();
            e3 = encoder3::getValue();
            e4 = encoder4::getValue();


            LCDParameter p1 {"FRQ", (int)frequency};
            LCDParameter p2 {"WAV", (int)morph};
            LCDParameter p3 {"Atk", (int)e3};
            LCDParameter p4 {"Rel", (int)e4 };

            synth.setFrequency(frequency);
            morph = morph/10.0f;
            synth.setMorph(morph);
            synth.setAttackTime(attackTimeMs);
            synth.setReleaseTime(releaseTimeMs);

            if(gate)
                synth.gate();
            lcdPage(display, p1, p2, p3, p4);
        }
        miosix::Thread::sleep(40);
    }
}

void lcdThreadF()
{

    while(true) {
        {
            miosix::FastMutex mutex;

            int e1 = encoder1::getValue();
            int e2 = encoder2::getValue();
            int e3 = encoder3::getValue();
            int e4 = encoder4::getValue();

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
    // ADC Initialization
    AdcReader::init();

    // Buttons Initialization
    button1::init();

    // Encoders Initialization
    encoder1::init();
    encoder2::init();
    encoder3::init();
    encoder4::init();

    // Audio Driver and Synth initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);

    // Hardware UI Thread
    std::thread hardwareInterfaceThread(hardwareUIThreadFunction);
    //std::thread lcdThread(lcdThreadF);
    usleep(1000);
    // Audio Thread
    audioDriver.start();
}
