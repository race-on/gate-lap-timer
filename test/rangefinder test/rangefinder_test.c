#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rangefinder.h"
#include "lcd_button.h"
#include "lcd.h"

volatile unsigned char flag = 0;
volatile unsigned char pinVal = 0;
volatile unsigned long pulse_cnt = 0;
char disp[17];

int main()
{
    //Initialize rangefinder pins
    DDRB |= (1<<PB3);
    DDRD &=~(1<<PD3);

    //Initialize pin change interrupt to detect echo pulse
    PCICR |= (1<<PCIE2);
    PCMSK2|= (1<<PCINT19);
    // Initialize time of flight counter
    TCCR1B |= (1 << WGM12); // CTC 16b timer
    TIMSK1 |= (1 << OCIE1A);
    OCR1A |= 46400;
    sei(); // enable interrupts

    lcd_button_init();
    lcd_init();
    lcd_splashscreen();
    _delay_ms(1000);

    while(1) {
        if(lcd_button_read() == select) {
            _delay_us(10);
            //while(lcd_button_read() == select){}
            _delay_us(10);

            // send trigger pulse for 10us
            PORTB |= (1<<PB3);
            _delay_us(10);
            PORTB &=~(1<<PB3);

            lcd_moveto(1,15);
            lcd_writedata('t');
            _delay_ms(1000);
            lcd_moveto(1,15);
            lcd_writedata(' ');
        } else {
            lcd_moveto(1,15);
            lcd_writedata('n');
            _delay_us(100);
        }
    }

    return 0;
}

ISR(PCINT2_vect)
{
    pinVal = PIND;
    pinVal &= (1<<PD3);
    if(pinVal) {
        lcd_moveto(1,14);
        lcd_writedata('p');
        unsigned long disp1, disp2;
        /*
        if(flag == 0) {
            TCNT1 = 0;
            TCCR1B |= (1<<CS11); // start timer
            flag = 1;
        } else {
            TCCR1B &= ~(1<<CS11);
            pulse_cnt = TCNT1;
            flag = 0;

            disp1 = (pulse_cnt/2)/58;
            disp2 = (pulse_cnt*5/58)%10;
            snprintf(disp, 17, "%ld.%ld", disp1, disp2);
            //lcd_clearscreen();
            lcd_moveto(0,0);
            lcd_stringout(disp);
            lcd_moveto(1,0);
            snprintf(disp, 17, "%ld", disp);
            lcd_stringout(disp);
        }
        */
    }
}

/* Rangefinder connections
VCC     = VCC
TRIG    = Arduino pin D11   (AVR PB3)
ECHO    = Arduino pin D3    (AVR PD3)
GND     = GND
*/