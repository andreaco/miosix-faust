#ifndef MIOSIX_DRUM_LCD_INTERFACE_H
#define MIOSIX_DRUM_LCD_INTERFACE_H
#include "miosix.h"
#include <util/lcd44780.h>

namespace LCDUtils
{
    /**
     * Struct used to hold and map strings and int values displayed by LCD
     */
    struct LCDParameter
    {
        LCDParameter() : name(""), value(0) {}
        std::string name;
        int value;
    };

    /**
     * Struct containing a single page to be displayed by the LCD
     */
    struct LCDPage
    {
        LCDParameter p[4];
    };

    /**
     * Function printing an entire formatted LCD page to the LCD
     * @param lcd lcd that will display the page
     * @param page page containing 4 parameters with relative mappings
     */
    void lcdPrintPage(miosix::Lcd44780 &lcd, LCDPage &page);
};
#endif //MIOSIX_DRUM_LCD_INTERFACE_H
