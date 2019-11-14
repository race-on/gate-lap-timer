#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"

volatile char watchState = -1;
volatile unsigned int ms = 0, s = 0, m = 0;
volatile unsigned char dispChange = 1;

volatile unsigned char ECHO_flag = 0;
volatile unsigned char timer_flag = 0;
volatile unsigned char pinValB = 0;
volatile unsigned char pinValC = 0;
volatile unsigned long pulse_cnt = 0;
volatile unsigned long pulse_mm = 4000;
volatile unsigned long pulse_mm_old = 0; // mm

const unsigned long gate_trigger_distance = 1000; // mm
const unsigned long gate_reset_distance = 1090; // mm
volatile unsigned int debounce_timer = 1008;
volatile unsigned char gate_car_state = 0;
volatile unsigned char gate_car_state_old = 0;

char disp[17];

int main()
{
	// RANGEFINDER INIT
	// ----------------
    //rangefinder_init('B', 5, 4); // initialize rangefinder pins
	DDRB |= (1<<PB5); // PB5 is output (for trig)
	DDRB &=~(1<<PB4); // PB4 is input (for echo)

    //Initialize pin change interrupt to detect echo pulse
    PCICR |= (1<<PCIE0); //  enable PCINT on PORTB
    PCMSK0|= (1<<PCINT4); // set PCINT mask for echo pin (PB4)
	
    // Rangefinder timer (timer1)
    TCCR1B |= (1 << WGM12); // enable CTC mode on timer1
    TIMSK1 |= (1 << OCIE1A); // enable interrupt on timer1
    OCR1A |= 46400; // for a 4 m range limit when prescaler set to 8(CS[2:0] = 010)
   	
	// STOPWATCH INIT
	// --------------
	// Stopwatch timer (timer0)
    TCCR0A |= (1 << WGM01); // Enable CTC mode
    TIMSK0 |= (1 << OCIE0A); // Enable Timer0 interrupt
    OCR0A = 250; // for prescaler of 64, 250 gives us 1 ms interrupts
    TCCR0B &= ~ ((1 << CS02) | (1 << CS01) | (1 << CS00)); // stop timer0

	// BUTTON PRESS INIT
	// -----------------
	// Button press inputs
    DDRC &= ~(1<<PC0); // set PD3 as input

    // Pin (PC0) change interrupt
    PCICR |= (1 << PCIE1); // enable PCINT for port C
	PCMSK1 |= (1 << PCINT8); // enable PC0 (A0)


    // LCD initialization and splash
    lcd_init();
	lcd_clearscreen();
	lcd_stringout(" RACE ON! @ USC");
	lcd_moveto(1,0);
	lcd_stringout("RANGEFINDER GATE");
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
		
		if(timer_flag == 1){
			snprintf(disp, 17, "%4lu mm", (long)((pulse_cnt*5/58)) );
			lcd_moveto(0,9);
			lcd_stringout(disp);
			timer_flag = 0;
		} else { //send trigger pulse to rangefinder
			PORTB |= (1<<PB5);
			_delay_us(10);
			PORTB &= ~(1<<PB5);
		}
    }
    return 0; // never reached
}

//Button press to ready stop watch
ISR(PCINT1_vect)
{
	pinValC = PINC;
	pinValC = (pinValC & (1<<PC0)) >> PC0;
	
	if(pinValC == 0 && watchState == -1) {
		lcd_moveto(0,0);
		lcd_stringout("ARM ");
		watchState = 0;
	} else if(pinValC == 0 && watchState == 1) {
		TCCR0B &= ~ ((1 << CS02) | (1 << CS01) | (1 << CS00)); // stop timer0 here by clearing prescaler		
		lcd_moveto(0,0);
		lcd_stringout("ARM ");
		gate_car_state = 0;
		watchState = 0;
	}
}

// invoked twice when rangefinger sends echo
	// once on positive edge, another on the negative edge
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
		pulse_mm = pulse_cnt*5/58;
		
		debounce_timer = m*60 + s;
		
		if (pulse_mm < gate_trigger_distance && gate_car_state == 0) {
			gate_car_state = 1;
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
		} else if (pulse_mm > gate_reset_distance && gate_car_state == 1 && debounce_timer > 2) {
			gate_car_state = 0;
		}				
    }
}

// invoked if the distance is more than 4m and timer1 expires.
ISR(TIMER1_COMPA_vect)
{
	TCCR1B &= ~((1<<CS11));//stops timer
	ECHO_flag = 0;//sets flag to 0
	TCNT1 = 0;//sets timer to start from 0 again
	pulse_mm = 4000;
}

// 1 ms interval timing for stopwatch
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

/* Rangefinder connections
VCC     = VCC
TRIG    = Arduino pin D13   (AVR PB5)
ECHO    = Arduino pin D12    (AVR PB4)
GND     = GND
*/