/******************************************************
* RACE ON! @ USC - Gate Lap Timer Project
* lcd_button.h - use ADC to detect buttton press on A0
               - designed for LCD shield's buttons
* Written by Suraj Chakravarthi (GitHub @solaremperor)
*****i*************************************************/
typedef enum buttontype {none, select, left, up, down, right}buttonType;
void lcd_button_init(void);
 buttonType lcd_button_read(void);