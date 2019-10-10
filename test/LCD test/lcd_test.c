/******************************************************
* RACE ON! @ USC - Gate Lap Timer Project
* lcd_test.c - LCD shiled tester
* Written by Suraj Chakravarthi (GitHub @solaremperor)
******************************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

int main(void) {

        // Setup DDR and PORT bits for the 2 input buttons
        // as necessary

        // Initialize the LCD
        lcd_init();
        
        lcd_clearscreen();
        lcd_writecommand(0);
        lcd_stringout(" RACE ON! @ USC");
        lcd_moveto(1,0);
        lcd_stringout(">>> LCD Test");
        return 0;   /* never reached */
}

