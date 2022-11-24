/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 * This program has been updaped by  Alban FERRACANI and Lucas LACROIX.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>           // Peter Fleury's UART library

/* Declaration of variables -------------------------------------------*/
/***********************************************************************/
uint8_t mux = 0;            // Variable uint8_t mux for switching ADC mux.
uint32_t password = 0;      // Password entered is set here.

uint16_t valuex;            // Value for x axis of Joystick
uint16_t valuey;            // Value for y axis of Joystick
uint16_t valueclick;        // Value for click from Joystick

uint32_t temperature_room1 = 20;   // Value with wanted temperature room1
uint32_t temperature_room2 = 20;   // Value with wanted temperature room2
uint8_t room = 1;                  // Select the room

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);

    // Initialize USART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    /*
    if (mux == 8){
      // Configure Analog-to-Digital Convertion unit
      // Internal 1.1V Voltage Reference with external capacitor at AREF pin
      ADMUX = ADMUX |= (1<<REFS0)|(1<<REFS1);
    }
    else {
      // Configure Analog-to-Digital Convertion unit
      // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
      ADMUX = ADMUX | (1<<REFS0);
    }*/

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);

    // Select input channel ADC0 (voltage divider pin)
    //ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX1| 1<<MUX0);

    // Enable ADC module
    ADCSRA = ADCSRA | (1<<ADEN);

    // Enable conversion complete interrupt
    ADCSRA = ADCSRA | (1<<ADIE);

    // Set clock prescaler to 128
    //THE FREAUENCY MUST BE beetween 15 and 200khz
    ADCSRA = ADCSRA | (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
    
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 33 ms and enable overflow interrupt
    TIM1_overflow_33ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static uint8_t no_of_overflows = 0;
    no_of_overflows++;
    // Do this every 3 x 33 ms = 100 ms
    if (no_of_overflows >= 3)
    {
        no_of_overflows = 0;
        /*
        We have to read different analog values from ADC. We have to change mux registers in time to change the pin we are reading.
        So, for example, in fisrt we read ADC0, and we switch to mux1 (ADC1). Finally, we switch to mux2 (ADC2).
        */
        switch (mux)
        {
        case 0:
          // Select input channel ADC0 (voltage divider pin)
          ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX1| 1<<MUX0);
          // Switching for ADC1 at the next TIMER1_OVF_vect
          mux = 1;
          break;

        case 1:
          // Select input channel ADC1 (voltage divider pin)
          ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX1); ADMUX|= (1<<MUX0);
          // Switching for ADC2 at the next TIMER1_OVF_vect
          mux=2;
          break;

        case 2:
          // Select input channel ADC2 (voltage divider pin)
          ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX0); ADMUX|= (1<<MUX1);
          // Switching for ADC8 at the next TIMER1_OVF_vect
          mux=0;
          break;
        }

        /* Set the target temperature with y axis of joystick for big increase/decrease.
           Adjust precisely temperature with the  x axis of the joystick.
        
                     -------------------
                     | Temperature set |
                     -------------------

                           + 2°C
                            | |          
                            | |        
                            | |       
                   ..................... x axis 
            -0.5°C ..................... +0.5°C
                            | | 
                            | | 
                            | | \
                           -2°C  \
                                  y axis 
        */


        /*
        If the user has selected the room 1 with the rotary encoder, 
        the lcd displays room1 target and the user can change temperature with the
        joystick as described previously.
        */
        if (room == 1){
            // Analysing y axis joystick position to increase/decrease 
            // temperature by step of ±2°C.

            // +2°C position on joystick 
            if (valuey > 900){
              temperature_room1 += 2;
            }
            // -2°C position on joystick 
            else if (valuey < 200){
              temperature_room1 -= 2;
            }

            // Analysing x axis joystick position to increase/decrease 
            // temperature by step of ±0.5°C.
            
            // -0.5°C position on joystick 
            if (valuex > 900){
              temperature_room1 -= 0.5;
            }
            // +0.5°C position on joystick 
            else if (valuex <200){
              temperature_room1 += 0.5; 
            }
            display_temperature_target(room, temperature_room1);
        }

        /*
        If the user has selected the room 2 with the rotary encoder, 
        the lcd displays room2 target and the user can change temperature with the
        joystick as described previously.
        */
        if (room == 2){
              // Analysing y axis joystick position to increase/decrease 
              // temperature by step of ±2°C.

              // +2°C position on joystick 
              if (valuey > 900){
                temperature_room2 += 2;
              }
              // -2°C position on joystick 
              else if (valuey < 200){
                temperature_room2 -= 2;
              }

              // Analysing x axis joystick position to increase/decrease 
              // temperature by step of ±0.5°C.
              
              // -0.5°C position on joystick 
              if (valuex > 900){
                temperature_room2 -= 0.5;
              }
              // +0.5°C position on joystick 
              else if (valuex <200){
                temperature_room2 += 0.5; 
              }
              display_temperature_target(room, temperature_room2);
          }
    }    
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect)
{
    /* In this case, (mux=1), that means that we have selected input channel ADC1. 
       So, we have to read the value from ADC and store it in our valuex variable.
       This value corresponds to x axis variations (range : 0-1023).
    */
    if(mux == 1){
      // String for converted numbers by itoa()
      char string[4];  
      // Display text on lcd.
      lcd_gotoxy(0,0);
      lcd_puts("val x");
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      valuex = ADC;
      // Convert "value" to "string" and display it
      itoa(valuex, string, 10);
      lcd_gotoxy(6,0);
      //To erease previous value on LCD.
      lcd_puts("     ");
      // Display value on lcd.
      lcd_gotoxy(6,0);
      lcd_puts(string);
    }

    /* In this case, (mux=0), that means that we have selected input channel ADC0. 
       So, we have to read the value from ADC and store it in our valuex variable.
       This value corresponds to y axis variations (range : 0-1023).
    */
    if(mux==0){
      // Select input channel ADC0 (voltage divider pin)
      ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX1| 1<<MUX0);

      char string[4];  // String for converted numbers by itoa()
      lcd_gotoxy(0,1);
      lcd_puts("val y");
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      valuey = ADC;
      // Convert "value" to "string" and display it
      itoa(valuey, string, 10);
      lcd_gotoxy(6,1);
      lcd_puts("     ");
      lcd_gotoxy(6,1);
      lcd_puts(string);
      
    }


    /* In this case, (mux=1), that means that we have selected input channel ADC2. 
       So, we have to read the value from ADC and store it in our valueclick variable.
       This value corresponds to button pressed from joystick (0 or 1023).
       --> If the button is not pressed : 1023 (HIGH state).
       --> If the button is pressed : <200 (LOW state).
    */
    if(mux == 2){
      char string[4];  // String for converted numbers by itoa()
      valueclick = ADC;
      // Button pressed 
      if (valueclick < 200){ 
          lcd_gotoxy(11,1);
          lcd_puts("click");
      }
      // Button not pressed
      else { 
          lcd_gotoxy(11,1);
          lcd_puts("     ");
      }
    }

}

/**********************************************************************
 * Function: Display target temperature on LCD. 
 * Purpose:  Display converted value on LCD screen.
 * @param temperature temperature target to display.
 * 
 **********************************************************************/
void display_temperature_target(uint8_t room, uint16_t temperature){
    
    char string[4];  // String for converted numbers by itoa()

    if(room == 1){
      lcd_clrscr();                   // We clear the display.
      lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
      lcd_puts("Room 1 - target");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      lcd_gotoxy(7, 1);
      itoa(temperature, string, 10);        
    }

    else if (room == 2){
      lcd_clrscr();                   // We clear the display.
      lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
      lcd_puts("Room 2 - target");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      lcd_gotoxy(7, 1);
      itoa(temperature, string, 10); 
    }
}