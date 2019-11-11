/********************************************
 *
 * Race On - PCINT test
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
int main(void)
{
    DDRD &= ~(1<<PD3); // set PD3 as input
    PORTD |= (1<<PD3); // enable pullup resistor on PD3

    PCICR |= (1 << PCIE2); // enable PCINT for port D
	PCMSK2 |= (1 << PCINT19); // enable PD3 (D3)
	sei(); // global interrupt enable

    lcd_init();
    lcd_clearscreen();
    //Print test text
    lcd_stringout(" RACE ON! @ USC");
    lcd_moveto(1,0);
    lcd_stringout(">> PCINT Test <<");
    _delay_ms(1000);
    lcd_clearscreen();

    //DDRB |= (1<<PB5); // PB5(D13) is onboard LED output

    while (1) {
        if (trigStatus == 0) {
            lcd_moveto(0,0);
            lcd_writedata('0');
        } else {
            lcd_moveto(0,0);
            lcd_writedata('1');
        }
        _delay_ms(1000);
        lcd_clearscreen();
    }
    return 0; // never reached
}

ISR(PCINT2_vect)
{
    pd3_button = ( PIND&(1<<PD3) );
    
    if(pd3_button == 0) {
        lcd_moveto(1,0);
        lcd_writedata('d');
        trigStatus = 0;
    } else if (pd3_button == (1<<PD3)) {
        lcd_moveto(1,0);
        lcd_writedata('D');
        trigStatus = 1;
    }
}