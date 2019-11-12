# gate-lap-timer| RACE ON! @ USC
Gate timer to measure lap time on race day.

## Required Hardware
- Arduino UNO or compatible board.
- DFRobot [1602 LCD shield](https://www.dfrobot.com/product-51.html).
- Jumper cables.
- Laser source and detector pair (OR) [ultrasonic rangefinder](https://www.sparkfun.com/products/15569).

## Software Prerequisites
- AVR-GCC toolchain
  - For Windows PC, install [WinAVR](https://sourceforge.net/projects/winavr/).
  - For Mac OS, install [CrossPack for AVR development](https://www.obdev.at/products/crosspack/index.html).
  - For Linux, use the package manager of your OS to install the packages `gcc-avr`, `binutils-avr` and `avr-libc`.
- Arduino device drivers
  - The [Arduino IDE](https://www.arduino.cc/en/main/software) is a reliable source for Arduino drivers.
  - You may also need to install the official [FTDI VCP drivers](https://www.ftdichip.com/Drivers/VCP.htm).
- Terminal
  - The terminal in both Linux and Mac OS will suffice.
  - You could use CMD on Windows, but you could optionally also install MINGW. [Git for Windows](https://gitforwindows.org/) bundles git with MINGW - I personally prefer this approach when using Windows.

## Operation Modes
We can either operate the gate timer using a LASER switch or an ultrasonic rangefiner.

### LASER swtich mode
In this mode, we are simply using a swtiching digital signal to detect the passage of a car and start/stop the timer.
- Connect the LCD shield to the Arduino UNO.
- Attach the detector (Active High) to Arduino digital pin 3 (AVR pin PD3). You may use jumper cables for this.
- Power both the LASER source and detector using the Arduino. Then connect the Arduino to a computer with all the prerequisite software installed.
- Using the command-lines (Terminal/CMD), navigate to the root directory of this project and run the command `make laser` to flash the correct hex file on to the Arduino.
- The board is now ready to use as a standalone gate timer.

### Rangefinder mode
This mode uses the rangefinder to detect the passage of a car to start/stop the timer.
- Connect the LCD shield to the Arduino UNO.
- using jumper cables, attach the Rangefinder to the Arduino board.
  - VCC and GND go to the appropriate pins on the board.
  - Connect the TRIG pin of the rangefinder to Ardunio pin D13 (AVR pin PB5).
  - Connect the ECHO pin of the rangefinder to Arduino pin D12 (AVR pin PB4).
- Connec the board to a computer with all the prerequisite software installed.
- Using the command-lines (Terminal/CMD), navigate to the root directory of this project and run the command `make rangefinder` to flash the correct hex file on to the Arduino.
- The board is now ready to use as a standalone gate timer.
- **NOTE:** The trigger distance and reset distance for the rangefinder can be modified by changing the constants towards the top of `rangefinder_gate.c`, available in the directory `./test/rangefinder gate`.
  - A new hex file can be generated using the `make` command. We must then replace the `raceon_gate_timer_rangefinder.hex` file in the root directory with the new hex file that is generated.

## Using the Gate Timer
- First, `ARM` the gate timer by pressing the *RIGHT* button on the LCD shield. Without this step, the stopwatch on the gate timer will not function. This prevents spurious and false starts/stops.
- Now, when the car first passes the gate, the timer will `RUN` with the time updating on the LCD every second.
- When the car passes the gate a second time, the timer will go to `STP` mode and stop timing. Now the accurate time (accurate down to 1 ms) is displayed.
- To run the gate timer again, it must be armed once more - see the first step.

## LICENSE
This project and all the code in it is distributed as-is and with no warranty under the GNU General Public License, Version 3. The included datasheets are distributed under appropriate licenses as specified by the manufacturer/publisher of said datasheets.
