#include <avr/io.h>
#include <avr/delay.h>
#include "lcd.h"
#include "lcd_button.h"

int main()
{
    buttonType buttonPressed;
    lcd_init();
    lcd_clearscreen();
    lcd_button_init();
    while(1) {
        buttonPressed = lcd_button_read();
        lcd_clearscreen();
        switch (buttonPressed)
        {
            case none:
                        lcd_stringout("no button");
                        break;
            case select:
                        lcd_stringout("select button");
                        break;
            case left:
                        lcd_stringout("left button");
                        break;
            case down:
                        lcd_stringout("down button");
                        break;
            case up:
                        lcd_stringout("up button");
                        break;
            case right:
                        lcd_stringout("right button");
                        break;
            default:
                        break;
        }
        _delay_ms(100);
    }
    return 0;
}