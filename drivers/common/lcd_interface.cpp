#include "lcd_interface.h"


void LCDUtils::lcdPrintPage(miosix::Lcd44780& lcd,
                  LCDUtils::LCDPage& page)
{
    lcd.go(0, 0);
    lcd.printf(" %s %s %s %s",
                   page.p[0].name.c_str(),
                   page.p[1].name.c_str(),
                   page.p[2].name.c_str(),
                   page.p[3].name.c_str());

    lcd.go(0, 1);
    lcd.printf(".%03d.%03d.%03d.%03d",
                   page.p[0].value,
                   page.p[1].value,
                   page.p[2].value,
                   page.p[3].value);
}