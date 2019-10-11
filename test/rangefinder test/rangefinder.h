#include <avr/io.h>
#include <avr/delay.h>

extern unsigned char trigger_pin;
extern unsigned char echo_pin;
extern unsigned char port_ID;

void rangefinder_init(unsigned char, unsigned short, unsigned short);
inline void rangefinder_trigger(void);