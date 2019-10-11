/******************************************************
* RACE ON! @ USC - Gate Lap Timer Project
* lcd.h - Headers to send data and commands to LCD
* Written by Suraj Chakravarthi (GitHub @solaremperor)
******************************************************/
void lcd_init(void);
void lcd_moveto(unsigned char, unsigned char);
void lcd_stringout(char *);
void lcd_clearscreen(void);
void lcd_writecommand(unsigned char);
void lcd_writedata(unsigned char);
void lcd_splashscreen(void);
