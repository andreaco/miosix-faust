#include "adc_reader.h"

/**
 * Static Initialization
 */
AdcReader::ValueState AdcReader::values;

void AdcReader::init() {
    {
        miosix::FastInterruptDisableLock dLock;

        // Enable ADC1 Clock
        RCC->APB2ENR |= (1 << 8);
        // Enable GPIOA clock
        RCC->AHB1ENR |= (1 << 0);

        // Prescaler
        ADC->CCR = (2 << 16);

        // Resolution
        ADC1->CR1 |= (1 << 8);  // Scan Mode
        ADC1->CR1 |= (2 << 24); // 8-bit resolution
        //ADC1->CR1 |= (0 << 24); // 12-bit resolution


        // Data Alignment
        ADC1->CR2 &= ~(1 << 11); // Left aligned
        ADC1->CR2 |= (1 << 10);  // EOC bit is set at the end of each regular conversion
        ADC1->CR2 |= (1 << 1);   // Continuous conversion mode

        //ADC1->SMPR2 &= ~((1<<3) | (1<<12)); // Sampling time for channel 1 and 4
        ADC1->SMPR2 |= (1 << 0) | (1 << 3) | (1 << 6) | (1 << 9); // Sampling time for channel 0, 1, 2


        ADC1->SQR1 |= (1 << 2); // Sequence for 2 channel conversions
        //ADC1->SQR1 |= (3<<0); // Sequence for 8? channel conversions

        GPIOA->MODER |= (3 << 2 * 2); //PA4 analog mode
        GPIOA->MODER |= (3 << 5 * 2); //PA5 analog mode
        GPIOA->MODER |= (3 << 6 * 2); //PA6 analog mode
        GPIOA->MODER |= (3 << 7 * 2); //PA7 analog mode



        //End setup

        //Enable
        ADC1->CR2 |= 1 << 0;

        // Wait to stabilize adc
        miosix::delayUs(100);
    }
}


AdcReader::ValueState& AdcReader::readAll()
{
    {
        miosix::FastInterruptDisableLock dLock;

        int N = 1;
        uint32_t avg = 0;
        for (int j = 0; j < N; ++j)
            avg += readChannel(2);
        avg = avg / N;

        values[0] = avg >> 1;


        avg = 0;
        for (int j = 0; j < N; ++j)
            avg += readChannel(5);
        avg = avg / N;

        values[1] = avg >> 1;

        avg = 0;
        for (int j = 0; j < N; ++j)
            avg += readChannel(6);
        avg = avg / N;

        values[2] = avg >> 1;

        avg = 0;
        for (int j = 0; j < N; ++j)
            avg += readChannel(7);
        avg = avg / N;

        values[3] = avg >> 1;
    }
    return values;
}


uint16_t AdcReader::readChannel(unsigned int channel)

{
    // ADC start
    ADC1->SQR3 = 0; // clear sequence register
    ADC1->SQR3 |= (channel << 0); // conversion of just one channel(in first slot) in a regular sequence

    ADC1->SR = 0; // clear status register

    ADC1->CR2 |= (1<<30); // start the conversion

    //wait for the conversion to finish (EOC)
    while (!(ADC1->SR & (1<<1)));

    //Extract val from DataRegister
    return  (ADC1->DR) ;
}