DEVICE     = atmega328p
CLOCK      = 16000000
OSFLAG    :=
PROGRAMMER =
ifeq ($(OS),Windows_NT)
	PROGRAMMER += -c arduino -b 115200 -P COM9 # line for Windows
	OSFLAG += WIN
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		PROGRAMMER += -c arduino -b 115200 -P /dev/tty.usbmodem* # line for Mac
		OSFLAG += OSX
	endif
	ifeq ($(UNAME_S),Linux)
		PROGRAMMER += -c arduino -b 115200 -P /dev/ttyACM* # line for Linux
		OSFLAG += LINUX
	endif
endif
OBJECTS    = lcd_test.o lcd.o
FUSES      = -U hfuse:w:0xde:m -U lfuse:w:0xff:m -U efuse:w:0x05:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
PCINT: raceon_gate_timer_PCINT_D3.hex
	$(AVRDUDE) -U flash:w:raceon_gate_timer_PCINT_D3.hex:i
