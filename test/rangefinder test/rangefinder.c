#include <avr/io.h>
#include <avr/delay.h>
#include "rangefinder.h"

unsigned char trigger_pin = (1<<PB5);
unsigned char echo_pin =  (1<<PB4);
unsigned char port_ID = 'B';
void rangefinder_init(unsigned char portID, unsigned short trigPin, unsigned short echoPin)
{
    unsigned short pinLimit = 0;
    switch (portID){
    case 'B':
        pinLimit = 5;
        port_ID = portID;
        if (trigPin >= 0 && trigPin <= pinLimit && echoPin >= 0 && echoPin <= pinLimit && trigPin != echoPin){
            trigger_pin = (1<<trigPin);
            echo_pin    = (1<<echoPin);
            DDRB |= trigger_pin; // set trigger pin as output
            DDRB &= ~echo_pin; // set echo pin as input
        }
        break;

    case 'C':
        pinLimit = 5;
        port_ID = portID;
        if (trigPin >= 0 && trigPin <= pinLimit && echoPin >= 0 && echoPin <= pinLimit && trigPin != echoPin){
            trigger_pin = (1<<trigPin);
            echo_pin    = (1<<echoPin);
            DDRC |= trigger_pin; // set trigger pin as output
            DDRC &= ~echo_pin; // set echo pin as input
        }
        break;

    case 'D':
        pinLimit = 7;
        port_ID = portID;
        if (trigPin >= 0 && trigPin <= pinLimit && echoPin >= 0 && echoPin <= pinLimit && trigPin != echoPin){
            trigger_pin = (1<<trigPin);
            echo_pin    = (1<<echoPin);
            DDRD |= trigger_pin; // set trigger pin as output
            DDRD &= ~echo_pin; // set echo pin as input
        }
        break;
        
    default:
        break;
    }
    if (pinLimit == 0) {
        DDRB |= trigger_pin; // set trigger pin as output
        DDRB &= ~echo_pin; // set echo pin as input
    }
}

inline void rangefinder_trigger()
{
    switch (port_ID)
    {
    case 'B':
        PORTB |= trigger_pin;
        _delay_us(10);
        PORTB &= ~trigger_pin;
        break;
    
    case 'C':
        PORTC |= trigger_pin;
        _delay_us(10);
        PORTC &= ~trigger_pin;
        break;
    
    case 'D':
        PORTD |= trigger_pin;
        _delay_us(10);
        PORTD &= ~trigger_pin;
        break;

    default:
        break;
    }
}
