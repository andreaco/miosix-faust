#include "dac.h"
#include "miosix.h"

/**
 * Function that initialize the periphereal to play audio.
 * Call this function in the AudioDriver constructor after having set up the DMA
 */
void Cs43l22dac::init() {
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

    /**
     * "Magic" Initialization:
     * https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 32
     */
    send(0x00, 0x99); 
    send(0x47, 0x80);
    send(0x32, 0xbb);
    send(0x32, 0x3b);
    send(0x00, 0x00);

    send(0x05, 0x20); //AUTO=0, SPEED=01, 32K=0, VIDEO=0, RATIO=0, MCLK=0
    send(0x04, 0xaf); //Headphone always ON, Speaker always OFF
    send(0x06, 0x04); //I2S Mode
    setVolume(-20); // TODO: use the attribute volume
}

Cs43l22dac::Cs43l22dac() {

   
}


void Cs43l22dac::send(unsigned char index, unsigned char data) {
    i2c::sendStart();
    i2c::send(0x94); //0x94 = [100101][0][0] is [start condition][AD0][0] (https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 33)
    i2c::send(index);
    i2c::send(data);
    i2c::sendStop();
}

void Cs43l22dac::setVolume(int db) {
    db = 2 * std::max(-102, std::min(0, db));
    unsigned char vol = static_cast<unsigned int>(db) & 0xff;
    send(0x20, vol);
    send(0x21, vol);
}