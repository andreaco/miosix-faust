#ifndef MIOSIX_DRUM_MIDI_IN_H
#define MIOSIX_DRUM_MIDI_IN_H
#include "config/hw_config.h"
#include "../../miosix/arch/common/drivers/serial_stm32.h"

/**
 * Class encapsulating the serial communication
 * incoming through the MIDI in port
 */
class MidiIn
{
public:
    /**
     * Constructor
     * Initialize the serial port with the specified serial ID in hw_config.h
     */
    MidiIn() : serialPort(MIDI_SERIAL_ID, 31250) {}

    /**
     * Wrapper function for blocking function STM32Serial.readBlock
     * Reads a single byte and returns it
     * This is a blocking function, will wait until another message is received\
     * @param byte to be read from the serial port
     * @return number of bytes read (1 in our case) if some error happened it returns -1 instead
     */
    inline uint8_t read(uint8_t* byte)
    {
        uint8_t status = serialPort.readBlock(byte, 1, 0);
        return status;
    }

private:

    /**
     * Serial Port Object
     */
    miosix::STM32Serial serialPort;
};
#endif //MIOSIX_DRUM_MIDI_IN_H
