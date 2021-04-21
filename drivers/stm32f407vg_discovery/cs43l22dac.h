#ifndef MIOSIX_CS43L22DAC_H
#define MIOSIX_CS43L22DAC_H

#include "miosix.h"
#include "util/software_i2c.h"

#define CS43L22DAC_DEFAULT_VOLUME_DB -20

/**
 * Class to control the CS43L22 on STM32F407VG_Discovery boards
 */
class Cs43l22dac {
public:
    /**
     * Function that initialize the peripheral to play audio.
     * Call this function in the AudioDriver constructor after having set up the DMA
     */
    static void init(uint32_t sampleRate);

    /**
     * Function used to write registers in the CS43L22 DAC periphereal
     * \param index address of the register to be modified
     * \param data data to be written to the register
     */
    static void send(unsigned char index, unsigned char data);

    /**
     * \param db volume level in db (0 to -102). Warning: 0db volume is LOUD!
     * \return value to store in register 0x20 and 0x21
     */
    static void setVolume(int db);

private:
    /**
     * Static class, constructor disabled
     */
    Cs43l22dac();

    /**
     * Static class, copy constructor disabled
     */
    Cs43l22dac(const Cs43l22dac &);

    /**
     * Static class, assignment operator disabled
     */
    Cs43l22dac &operator=(const Cs43l22dac &);
};


#endif //MIOSIX_CS43L22DAC_H