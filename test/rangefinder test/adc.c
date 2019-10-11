/******************************************************
* RACE ON! @ USC - Gate Lap Timer Project
* adc.c - Routines to use ADC for reading analog inputs
* Written by Suraj Chakravarthi (GitHub @solaremperor)
******************************************************/
#include <avr/io.h>
#include "adc.h"


void adc_init(unsigned char channel)
{
    // Initialize the ADC
	ADMUX |= channel;
	ADMUX |= (1<<ADLAR) | (1<<REFS0);
	ADCSRA |= 0x87;
}

unsigned char adc_sample()
{
    // Convert an analog input and return the 8-bit result
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1 << ADSC))!= 0){}
	unsigned char result = ADCH;
	return result;
}
