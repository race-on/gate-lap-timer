/********************************************
 *
 * Race On - Stopwatch test
 * Written by Suraj Chakravarthi Raja
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "lcd.h"

volatile unsigned char trigStatus = 1;
volatile unsigned char pd3_button = 0;

volatile unsigned char watchState = 0;
volatile unsigned int ms = 0, s = 0, m = 0;
volatile unsigned char dispChange = 1;
int main(void)
{
    // Button press inputs
    DDRD &= ~(1<<PD3); // set PD3 as input
    PORTD |= (1<<PD3); // enable pullup resistor on PD3

    // Stopwatch timer (timer0)
    TCCR1B |= (1 << WGM12); // Enable CTC mode
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 interrupt
    OCR1A = 15625; // for prescaler of 128, 

    // Pin (PD3) change interrupt
    PCICR |= (1 << PCIE2); // enable PCINT for port D
	PCMSK2 |= (1 << PCINT19); // enable PD3 (D3)
	sei(); // global interrupt enable

    // LCD initialization and splash
    lcd_init();
    lcd_clearscreen();
    //Print test text
    lcd_stringout(" RACE ON! @ USC");
    lcd_moveto(1,0);
    lcd_stringout("> STPWTCH Test <");
    _delay_ms(1000);
    lcd_clearscreen();

    char watchTime[17];
    while (1) {
        if (dispChange == 1) {
            dispChange = 0;
            snprintf(watchTime, 17, "%02d:%02d.%03d", m, s, ms);
            lcd_moveto(1,0);
            lcd_stringout(watchTime);
        }
    }
    return 0; // never reached
}

ISR(PCINT2_vect)
{
    pd3_button = ( PIND&(1<<PD3) ) >> PD3;
    
    // AFTER ACTIVE LOW BUTTON PRESS, PIN GOES BACK TO HIGH
    // USING THIS AS A RUDIMENTARY DEBOUNCE
    if(pd3_button == 1) {
        if(watchState == 0) {
            lcd_moveto(0,0);
            lcd_writedata('R');
            ms = 0; s = 0; m = 0; // resetting watch time registers
            dispChange = 1;
            watchState = 1;
            // run timer here by setting prescaler - SCR
        } else if(watchState == 1) {
            // stop timer here by clearing prescaler - SCR
            watchState = 0;
            lcd_moveto(0,0);
            lcd_writedata('S');
            dispChange = 1;
        }
    }
}