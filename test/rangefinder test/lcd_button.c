/******************************************************
* RACE ON! @ USC - Gate Lap Timer Project
* lcd_button.c - use ADC to detect buttton press on A0
               - designed for LCD shield's buttons
* Written by Suraj Chakravarthi (GitHub @solaremperor)
******************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "adc.h"
#include "lcd_button.h"

#include "lcd.h"

void lcd_button_init()
{
    adc_init(0);
    lcd_init();
}

buttonType lcd_button_read()
{
    unsigned char result = adc_sample();
    
    if (result >= 220) {
        return none;
    } else if (result < 220 && result >= 190) {
        return select;
    } else if (result < 190 && result >= 130) {
        return left;
    } else if (result < 130 && result >=  80) {
        return down;
    } else if (result <  80 && result >=  30) {
        return up;
    } else {
        return right;
    }

    return 0;
}