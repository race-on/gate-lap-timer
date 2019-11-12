#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rangefinder.h"
#include "lcd.h"

volatile unsigned char ECHO_flag = 0;
volatile unsigned char timer_flag = 0;
volatile unsigned char pinValB = 0;
volatile unsigned long pulse_cnt = 0;
char disp[17];

int main()
{
    //rangefinder_init('B', 5, 4); // initialize rangefinder pins
	DDRB |= (1<<PB5); // PB5 is output (for trig)
	DDRB &=~(1<<PB4); // PB4 is input (for echo)

    //Initialize pin change interrupt to detect echo pulse
    PCICR |= (1<<PCIE0); //  enable PCINT on PORTB
    PCMSK0|= (1<<PCINT4); // set PCINT mask for echo pin (PB4)
	
    // Initialize timer1 for rangefinder
    TCCR1B |= (1 << WGM12); // enable CTC mode on timer1
    TIMSK1 |= (1 << OCIE1A); // enable interrupt on timer1
    OCR1A |= 46400; // for a 4 m range limit when prescaler set to 8(CS[2:0] = 010)
   	
    lcd_init();
	lcd_clearscreen();
	lcd_stringout(" RACE ON! @ USC");
	lcd_moveto(1,0);
	lcd_stringout("RANGEFINDER TEST");
    _delay_ms(1000);
	lcd_clearscreen();

	sei(); // enable interrupts
	
	while(1) {
		//send trigger pulse to rangefinder	
		if(timer_flag == 1){
			snprintf(disp, 17, "%lu mm", (long)((pulse_cnt*5/58)) );
			lcd_moveto(0,0);
			lcd_stringout(disp);
			timer_flag = 0;
		} else {
			PORTB |= (1<<PB5);
			_delay_us(10);
			PORTB &= ~(1<<PB5);
		}
	}
    return 0;
}

// invoked if the distance is more than 4m and timer1 expires.
ISR(TIMER1_COMPA_vect)
{
	TCCR1B &= ~((1<<CS11));//stops timer
	ECHO_flag = 0;//sets flag to 0
	TCNT1 = 0;//sets timer to start from 0 again
}


ISR(PCINT0_vect)
{
	pinValB = PINB;
	pinValB = (pinValB & (1<<PB4)) >> PB4;
	
	if (ECHO_flag == 0 && pinValB == 1) {
        //set flag to one
        ECHO_flag = 1;
		timer_flag = 0;
        TCNT1 = 0;
        TCCR1B |= (1 << CS11); // start timer1 by setting prescaler
    } else if (ECHO_flag == 1 && pinValB == 0) {
        TCCR1B &= ~(1 << CS11); // stop timer1 by clearing prescaler
		//set flag to zero
        ECHO_flag = 0;        
        timer_flag = 1;
        pulse_cnt = TCNT1;
    }
}


/* Rangefinder connections
VCC     = VCC
TRIG    = Arduino pin D13   (AVR PB5)
ECHO    = Arduino pin D12    (AVR PB4)
GND     = GND
*/