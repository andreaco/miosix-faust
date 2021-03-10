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



void Cs43l22dac::init(SampleRate::SR sampleRate) {
    int plli2sn;    // Multiplication factor for VCO
    int plli2sr;    // Division factor for I2S clocks
    int i2sdiv;     // I2S linear prescaler
    int i2sodd;     // Odd factor for the prescaler
    switch(sampleRate) {
        case SampleRate::_8000Hz:
            plli2sn = 256;
            plli2sr = 5;
            i2sdiv  = 122;
            i2sodd  = 1;
            break;
        case SampleRate::_16000Hz:
            plli2sn = 213;
            plli2sr = 2;
            i2sdiv  = 13;
            i2sodd  = 0;
            break;
        case SampleRate::_32000Hz:
            plli2sn = 213;
            plli2sr = 2;
            i2sdiv  = 6;
            i2sodd  = 1;
            break;
        case SampleRate::_48000Hz:
            plli2sn = 258;
            plli2sr = 3;
            i2sdiv  = 3;
            i2sodd  = 1;
            break;
        case SampleRate::_96000Hz:
            plli2sn = 344;
            plli2sr = 2;
            i2sdiv  = 3;
            i2sodd  = 1;
            break;
        case SampleRate::_22050Hz:
            plli2sn = 429;
            plli2sr = 4;
            i2sdiv  = 9;
            i2sodd  = 1;
            break;
        case SampleRate::_44100Hz:
            plli2sn = 271;
            plli2sr = 2;
            i2sdiv  = 6;
            i2sodd  = 0;
            break;
        default:
            plli2sn = 0;
            plli2sr = 0;
            i2sdiv  = 0;
            i2sodd  = 0;
            break;
    }

    //Pin initialization
    {
        miosix::FastInterruptDisableLock dLock;

        //Enable DMA1 and SPI3/I2S3
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
        RCC_SYNC();
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        RCC_SYNC();

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

        //Enable audio PLL
        RCC->PLLI2SCFGR = (plli2sr << 28) | (plli2sn << 6);
        RCC->CR |= RCC_CR_PLLI2SON;
    }

    //Wait for PLL to lock
    while ((RCC->CR & RCC_CR_PLLI2SRDY) == 0);

    //The device will remain in the Power-Down State
    //until RESET is brought high.
    reset::low();
    miosix::delayUs(5);
    reset::high();
    miosix::delayUs(5);


    //"Magic" Initialization:
    //https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 32
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

    SPI3->CR2 = SPI_CR2_TXDMAEN;
    SPI3->I2SPR = SPI_I2SPR_MCKOE | (i2sodd << 8) | i2sdiv;
    SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD    //I2S mode selected
                    | SPI_I2SCFGR_I2SE      //I2S Enabled
                    | SPI_I2SCFGR_I2SCFG_1; //Master transmit

    NVIC_SetPriority(DMA1_Stream5_IRQn, 1); //High priority for DMA
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    Cs43l22dac::send(0x02, 0x9e);
}



void Cs43l22dac::send(unsigned char index, unsigned char data) {
    i2c::sendStart();

    //0x94 = [100101][0][0] is [start condition][AD0][0]
    // (https://www.mouser.com/datasheet/2/76/CS43L22_F2-1142121.pdf, Page 33)
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