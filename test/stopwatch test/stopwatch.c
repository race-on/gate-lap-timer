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
volatile unsigned char pd3_button = 0, pc0_button = 0;

volatile char watchState = -1;
volatile unsigned int ms = 0, s = 0, m = 0;
volatile unsigned char dispChange = 1;
int main(void)
{
    // Laser Pin (PD3) inputs
    DDRD &= ~(1<<PD3); // set PD3 as input
    PORTD |= (1<<PD3); // enable pullup resistor on PD3

    // Laser Pin (PD3) change interrupt
    PCICR |= (1 << PCIE2); // enable PCINT for port D
	PCMSK2 |= (1 << PCINT19); // enable PD3 (D3)
	
    // Stopwatch timer (timer0)
    TCCR0A |= (1 << WGM01); // Enable CTC mode
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 interrupt
    OCR0A = 250; // for prescaler of 64, 250 gives us 1 ms interrupts
    TCCR0B &= ~ ((1 << CS02) | (1 << CS01) | (1 << CS00)); // stop timer0
	
	// ARM Pin (PC0) inputs
    DDRC &= ~(1<<PC0); // set PD3 as input

    // ARM Pin (PC0) change interrupt
    PCICR |= (1 << PCIE1); // enable PCINT for port C
	PCMSK1 |= (1 << PCINT8); // enable PC0 (A0)

    // LCD initialization and splash
    lcd_init();
    lcd_clearscreen();
    //Print test text
    lcd_stringout(" RACE ON! @ USC");
    lcd_moveto(1,0);
    lcd_stringout("> STPWTCH Test <");
    _delay_ms(1000);
    lcd_clearscreen();

	sei(); // global interrupt enable

    char watchTime[17];
    while (1) {
        if (dispChange == 1) {
            dispChange = 0;
            snprintf(watchTime, 17, "%03d:%02d.%03d", m, s, ms);            
            lcd_moveto(1,0);
            lcd_stringout(watchTime);
        }
    }
    return 0; // never reached
}

ISR(PCINT1_vect)
{
	pc0_button = ( PINC&(1<<PC0) ) >> PC0;
	if(pc0_button == 0 && watchState == -1) {
		lcd_moveto(0,0);
		lcd_stringout("ARM ");
		watchState = 0;
	}
}

ISR(PCINT2_vect)
{
    pd3_button = ( PIND&(1<<PD3) ) >> PD3;
    
    // AFTER ACTIVE LOW BUTTON PRESS, PIN GOES BACK TO HIGH
    // USING THIS AS A RUDIMENTARY DEBOUNCE
    if(pd3_button == 0) {
        if(watchState == 0) {
            lcd_moveto(0,0);
            lcd_stringout("RUN ");
            ms = 0; s = 0; m = 0; // resetting watch time registers
            dispChange = 1;
            watchState = 1;
            TCCR0B |= ((1 << CS01) | (1 << CS00)); // run timer0 here by setting prescaler to 64
        } else if(watchState == 1) {
            TCCR0B &= ~ ((1 << CS02) | (1 << CS01) | (1 << CS00)); // stop timer0 here by clearing prescaler
            watchState = -1;
            lcd_moveto(0,0);
            lcd_stringout("STP ");
            dispChange = 1;
        }
    }
}

ISR(TIMER0_COMPA_vect)
{
    if(watchState == 1) {
        ms+=1;
        if(ms > 999) {
            ms = 0;
            s+=1;
            dispChange = 1;
        }
        if(s > 59) {
            s = 0;
            m+=1;
        }
    } else {
        // timer shouldn't be running. Stop it.
        TCCR0B &= ~ ((1 << CS02) | (1 << CS01) | (1 << CS00)); // stop timer0
    }
}