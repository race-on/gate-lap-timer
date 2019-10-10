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
        lcd_stringout(" RACE ON! @ USC");
        lcd_moveto(1,0);
        lcd_stringout("    LCD Test");
        // Use a state machine approach to organize your code
        //   - Declare and initialize a variable to 
        //     track what state you are in
        
    while (1) {               // Loop forever

                // Use an outer if statement to select what state you are in
                // Then inside each 'if' or 'else' block, perform the desired
                // output operations and then sample the inputs to update
                // the state for the next iteration
        
                
                // Delay before we go to the next iteration of the loop
                
    }

    return 0;   /* never reached */
}

