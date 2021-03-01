
#ifndef MIOSIX_DAC_H
#define MIOSIX_DAC_H

#include "miosix.h"
#include "miosix/util/software_i2c.h"

// TODO: complete the mapping and add a comment to suggest the configuration
#define SCL_GPIO_BASE GPIOB_BASE
#define SCL_GPIO_PIN 6


/**
 * Class to control the CS43L22 on STM32F407VG_Discovery boards
 */
class Cs43l22dac {
public:
    Cs43l22dac();

    /**
     * Initialize the DAC.
     */
    void init();

    /**
     * Function used to write registers in the CS43L22 DAC periphereal
     * \param index address of the register to be modified
     * \param data data to be written to the register
     */
    void send(unsigned char index, unsigned char data);

    /**
     * \param db volume level in db (0 to -102). Warning: 0db volume is LOUD!
     * \return value to store in register 0x20 and 0x21
     */
    void setVolume(int db);

private:
    Cs43l22dac(const Cs43l22dac &);

    Cs43l22dac &operator=(const Cs43l22dac &);

    // TODO: add volume variable

    // TODO: find the meaning of these strange names
    // TODO: move all typedefs on the top
    typedef miosix::Gpio<SCL_GPIO_BASE, SCL_GPIO_PIN> scl;
    typedef miosix::Gpio<GPIOB_BASE, 9> sda;
    typedef miosix::Gpio<GPIOA_BASE, 4> lrck;
    typedef miosix::Gpio<GPIOC_BASE, 7> mclk;
    typedef miosix::Gpio<GPIOC_BASE, 10> sclk;
    typedef miosix::Gpio<GPIOC_BASE, 12> sdin;
    typedef miosix::Gpio<GPIOD_BASE, 4> reset;
    typedef miosix::SoftwareI2C<sda, scl> i2c;
};


#endif //MIOSIX_DAC_H