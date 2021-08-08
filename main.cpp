#include "miosix.h"
#include "drivers/common/audio.h"
#include "audio/audio_processor.h"
#include "audio/audio_buffer.h"
#include "audio/audio_math.h"
#include "midi/midi.h"
#include "tests/midi_test_data.h"
#include "containers/circular_buffer.h"
#include <functional>
#include <algorithm>
#include <math.h>
#include <cstdint>
#include "TestAudioProcessor.h"
#include "drivers/stm32f407vg_discovery/encoder.h"
#include <util/lcd44780.h>

typedef miosix::Gpio<GPIOB_BASE, 12> d4;
typedef miosix::Gpio<GPIOB_BASE, 13> d5;
typedef miosix::Gpio<GPIOB_BASE, 14> d6;
typedef miosix::Gpio<GPIOB_BASE, 15> d7;
typedef miosix::Gpio<GPIOC_BASE, 1> rs;
typedef miosix::Gpio<GPIOC_BASE, 2> e;

typedef Encoder<TIM1_BASE, GPIOE_BASE,  9, 11> encoder1;
typedef Encoder<TIM3_BASE, GPIOB_BASE,  4,  5> encoder2;
typedef Encoder<TIM4_BASE, GPIOD_BASE, 12, 13> encoder3;
typedef Encoder<TIM5_BASE, GPIOA_BASE,  0,  1> encoder4;

int main()
{
    // Display Setup
    miosix::Lcd44780 display(rs::getPin(),e::getPin(),d4::getPin(),
                             d5::getPin(),d6::getPin(),d7::getPin(),2,16);
    encoder1::init();
    encoder2::init();
    encoder3::init();
    encoder4::init();

    for(;;) {
        miosix::Thread::sleep(1000);
        display.clear();
        display.go(0, 1);
        display.printf("[%02d][%02d][%02d][%02d]",
                       encoder1::getValue(),
                       encoder2::getValue(),
                       encoder3::getValue(),
                       encoder4::getValue());

    }
}

// AudioDriver Test
#if 0
int main() {
    // initializing the audio driver
    AudioDriver audioDriver;
    audioDriver.getBuffer();
    AudioProcessorTest audioProcessorTest(audioDriver);
    audioDriver.init();
    audioDriver.setAudioProcessable(audioProcessorTest);



    audioDriver.start();
}

#endif