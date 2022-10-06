# Lab 2: Alban FERRACANI

### GPIO control registers

1. Complete the table for DDRB and PORTB control register values.

   | **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
   | :-: | :-: | :-: | :-: | :-- |
   | 0 | 0 | input | no | Tri-state, high-impedance |
   | 0 | 1 | input | yes | pull-up activated|
   | 1 | 0 | output | no| low level output|
   | 1 | 1 | output | no| high level output|

### GPIO library

2. Complete the table with code sizes from three versions of the blink application.

   | **Version** | **Size [B]** |
   | :-- | :-: |
   | Arduino-style     | 480 |
   | Registers         | 182 |
   | Library functions | 182 |
   >We can note that with using registers or library functions, we save about 62% of flash compared to Arduino coding style !

### Traffic light

3. Scheme of traffic light application with one red/yellow/green light for cars, one red/green light for pedestrians, and one push button. Connect AVR device, LEDs, resistors, push button (for pedestrians), and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()

>According to the Arduino UNO pinout (which is based on an ATMEGA328P microcontroller), we connect : 
> - LED_CARS_GREEN *(led green for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB5`.
> - LED_CARS_ORANGE *(led orange/yellow for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB4`.
> - LED_CARS_RED *(led red for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB3`.
> 
> - LED_PEDESTRIANS_RED *(led red for pedestrians)* to a digital pin (that we will configure in output). Here, I have chosen `PB2`.
> - LED_PEDESTRIANS_GREEN *(led green for pedestrians)* to a digital pin (that we will configure in output). Here, I have chosen `PB1`.

![Pinout-UNOrev3_latest](https://user-images.githubusercontent.com/114081879/194305483-f8924e3c-08c0-4ced-933a-794cdd1d37e3.png)
