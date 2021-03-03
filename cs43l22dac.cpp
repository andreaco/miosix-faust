#include "cs43l22dac.h"
#include "miosix.h"

/**
 * Left Right Clock:
 * Determines which channel is currently
 * active on the serial audio data line
 */
typedef miosix::Gpio<GPIOA_BASE, 4> lrck;

/**
 * Master Clock:
 * Clock source for delta-sigma modulators
 */
typedef miosix::Gpio<GPIOC_BASE, 7> mclk;

/**
 * Serial Clock:
 * Serial clock for the serial audio interface
 */
typedef miosix::Gpio<GPIOC_BASE, 10> sclk;

/**
 * Serial Audio Data Input:
 * Input for two's complement serial audio data
 */
typedef miosix::Gpio<GPIOC_BASE, 12> sdin;

/**
 * Reset:
 * The device enters a low power mode when the pin is driven low
 */
typedef miosix::Gpio<GPIOD_BASE, 4> reset;

/**
 * Software I2C initialization
 */
typedef miosix::Gpio<GPIOB_BASE, 6> scl;     //! Serial Clock
typedef miosix::Gpio<GPIOB_BASE, 9> sda;     //! Data Line
typedef miosix::SoftwareI2C<sda, scl> i2c;   //! Software I2C static object



void Cs43l22dac::init() {
    //Pin initialization
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

    /*
     * The device will remain in the Power-Down State
     * until RESET is brought high.
     */
    reset::low();
    miosix::delayUs(5);
    reset::high();
    miosix::delayUs(5);

    /*
     * "Magic" Initialization:
     * https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 32
     */
    send(0x00, 0x99); 
    send(0x47, 0x80);
    send(0x32, 0xbb);
    send(0x32, 0x3b);
    send(0x00, 0x00);

    // Clocking Control
    send(0x05, 0x20); //AUTO=0, SPEED=01, 32K=0, VIDEO=0, RATIO=0, MCLK=0

    // Power Control
    send(0x04, 0xaf); //Headphone always ON, Speaker always OFF

    // Interface Control 1
    send(0x06, 0x04); //I2S Mode

    // Initial default volume
    setVolume(-20);
}



void Cs43l22dac::send(unsigned char index, unsigned char data) {
    i2c::sendStart();
    /*
     * TODO: Define of this value?
     * 0x94 = [100101][0][0] is [start condition][AD0][0]
     * (https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 33)
     */
    i2c::send(0x94);
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