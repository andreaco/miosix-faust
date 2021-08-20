#include "../../../include/drivers/stm32f407vg_discovery/midi_in.h"

 uint8_t MidiIn::read(uint8_t* byte)
{
    uint8_t status = serialPort.readBlock(byte, 1, 0);
    return status;
}