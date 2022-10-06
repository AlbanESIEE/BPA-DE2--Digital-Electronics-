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


>Here is my scheme of traffic light application with one red/yellow/green light for cars, one red/green light for pedestrians, and one push button (I made it on Easy Eda).
>Please note that for pedestrians push button, I use an *active HIGH configuration* and for the LEDs, an *active high configuration* too. 

![Schematic_Digital electronics 2 (DE2)_2022-10-06](https://user-images.githubusercontent.com/114081879/194310267-dada0ad1-df7f-4a3d-88b0-925073b8f804.svg)

According to the Arduino UNO pinout (which is based on an ATMEGA328P microcontroller), we connect : 

**For cars :**
 - `LED_CARS_GREEN` *(led green for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB5`.
 - `LED_CARS_ORANGE` *(led orange/yellow for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB4`.
 - `LED_CARS_RED` *(led red for cars)* to a digital pin (that we will configure in output). Here, I have chosen `PB3`.
 
**For pedestrians :**
 - `LED_PEDESTRIANS_RED` *(led red for pedestrians)* to a digital pin (that we will configure in output). Here, I have chosen `PB2`.
 - `LED_PEDESTRIANS_GREEN` *(led green for pedestrians)* to a digital pin (that we will configure in output). Here, I have chosen `PB1`.
 - `PUSH_BUTTON`*(the push-button for pedestrians)* to an analog pin (configured in input). Here, I connected it to `PC0`.

In the following pinout diagram of the Arduino UNO (based on a microcontroller ATMEGA328P), we can see the location of `PB5` to `PB1`and `PC0` pins I used.
>Please note I made the electrical diagram on EasyEda (software to create electricals diagrams and PCBs routing), so I didn't represent the Arduino Board but the Atmega328P microcontroller. 

![Pinout-UNOrev3_latest](https://user-images.githubusercontent.com/114081879/194305483-f8924e3c-08c0-4ced-933a-794cdd1d37e3.png)
