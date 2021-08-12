#ifndef MIOSIX_DRUM_ADC_READER_H
#define MIOSIX_DRUM_ADC_READER_H
#include "miosix.h"
#include <array>
#include <cmath>
#include "../common/hw_config.h"

#define ADC_GPIO GPIOA
#define ADC_PIN1 4
#define ADC_PIN2 5
#define ADC_PIN3 6
#define ADC_PIN4 7

class AdcReader
{
public:
    typedef std::array<uint16_t , 8> ValueState;

    /**
     * Static function used to initialize the ADC and the relative pins
     */
    static void init();

    /**
     * Read values from the ADC and update the current state
     */
    static ValueState& readAll();



private:

    /**
     * Performs a single read from the specified channel
     * @param channel Channel in which to perform the read operation
     * @return Value read from the ADC channel
     */
    static uint16_t readChannel(unsigned int channel);

    /**
     * Array containing the values read from ADC
     */
    static ValueState values;

    /**
     * Static class, constructor disabled
     */
    AdcReader();

    /**
     * Static class, copy constructor disabled
     */
    AdcReader(const AdcReader &);

    /**
     * Static class, assignment operator disabled
     */
    AdcReader &operator=(const AdcReader &);
};

#endif //MIOSIX_DRUM_ADC_READER_H
