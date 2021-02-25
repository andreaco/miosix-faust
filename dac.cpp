#include "dac.h"
#include "miosix.h"

/**
 * Function that initialize the periphereal to play audio.
 * Call this function in the AudioDriver constructor after having set up the DMA
 */
void AudioDAC::init() {
    {
        miosix::FastInterruptDisableLock dLock;
        //Configure GPIOs
        i2c::init();
        mclk::mode(miosix::Mode::ALTERNATE);
        mclk::alternateFunction(6);
        sclk::mode(miosix::Mode::ALTERNATE);
        sclk::alternateFunction(6);
        sdin::mode(miosix::Mode::ALTERNATE);
        sdin::alternateFunction(6);
        lrck::mode(miosix::Mode::ALTERNATE);
        lrck::alternateFunction(6);
        reset::mode(miosix::Mode::OUTPUT);
    }   

    reset::low(); //Keep in reset state
    miosix::delayUs(5);
    reset::high();
    miosix::delayUs(5);

    send(0x00, 0x99); //These five command are the "magic" initialization
    send(0x47, 0x80);
    send(0x32, 0xbb);
    send(0x32, 0x3b);
    send(0x00, 0x00);

    send(0x05, 0x20); //AUTO=0, SPEED=01, 32K=0, VIDEO=0, RATIO=0, MCLK=0
    send(0x04, 0xaf); //Headphone always ON, Speaker always OFF
    send(0x06, 0x04); //I2S Mode
    setVolume(-20);
}

AudioDAC::AudioDAC() {

   
}

AudioDAC &AudioDAC::getInstance() {
    static AudioDAC instance;
    return instance;
}

void AudioDAC::send(unsigned char index, unsigned char data) {
    i2c::sendStart();
    i2c::send(0x94);
    i2c::send(index);
    i2c::send(data);
    i2c::sendStop();
}

void AudioDAC::setVolume(int db) {
    db = 2 * max(-102, min(0, db));
    unsigned char vol = static_cast<unsigned int>(db) & 0xff;
    send(0x20, vol);
    send(0x21, vol);
}