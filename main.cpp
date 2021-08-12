#include "miosix.h"
#include "drivers/common/audio.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include "drivers/stm32f407vg_discovery/button.h"
#include "audio/audio_processor.h"
#include "Synth/Synth.h"
#include <cstdint>
#include <util/lcd44780.h>



typedef miosix::Gpio<GPIOB_BASE,12> d4;
typedef miosix::Gpio<GPIOB_BASE,13> d5;
typedef miosix::Gpio<GPIOB_BASE,14> d6;
typedef miosix::Gpio<GPIOB_BASE,15> d7;
typedef miosix::Gpio<GPIOC_BASE,1> rs;
typedef miosix::Gpio<GPIOC_BASE,2> e;


typedef Encoder<TIM1_BASE, GPIOE_BASE,  9, 11> encoder1;
typedef Encoder<TIM3_BASE, GPIOB_BASE,  4,  5> encoder2;
typedef Encoder<TIM4_BASE, GPIOD_BASE, 12, 13> encoder3;
typedef Encoder<TIM5_BASE, GPIOA_BASE,  0,  1> encoder4;

typedef Button<GPIOD_BASE, 1> button1;


//Audio Driver and Synthesizer Objects
static AudioDriver audioDriver;
static Synth synth(audioDriver);



int main() {
    button1::init();

    // LCD Initialization
    miosix::Lcd44780 display(rs::getPin(), e::getPin(), d4::getPin(),
                             d5::getPin(), d6::getPin(), d7::getPin(), 2, 16);
    display.clear();
    display.go(0, 0);
    display.printf("Miosix Synth 01");

    // Audio Driver and Synth initialization
    audioDriver.init();
    audioDriver.setAudioProcessable(synth);
    audioDriver.start();

}
