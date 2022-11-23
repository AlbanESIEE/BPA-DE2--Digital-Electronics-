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

uint8_t mux = 0;

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
          // Switching for ADC0 at the next TIMER1_OVF_vect
          mux=0;
          break;
        }
        // Start ADC conversion
        ADCSRA = ADCSRA | (1<<ADSC);
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
      // Read value for x axis of Joystick
      uint16_t valuex;
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

      // Read value for y axis of Joystick
      uint16_t valuey;
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
      uint16_t valueclick;
      char string[4];  // String for converted numbers by itoa()
      valueclick = ADC;
      // Button pressed 
      if (valueclick < 200){ 
          lcd_gotoxy(11,0);
          lcd_puts("click");
      }
      // Button not pressed
      else { 
          lcd_gotoxy(11,0);
          lcd_puts("     ");
      }
    }





    /*
    // Read converted value
    // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
    value = ADC;
    // Convert "value" to "string" and display it
    itoa(value, string, 10);
    lcd_gotoxy(8,0);
    lcd_puts("      ");
    lcd_gotoxy(8,0);
    lcd_puts(string);
    //hexa 
    itoa(value, string, 16);
    lcd_gotoxy(13,0);
    lcd_puts("      ");
    lcd_gotoxy(13,0);
    lcd_puts(string);

    if(value <= 5) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("Right");
    }
    if(value <= 110 && value >= 90) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("Up");
    }
    if(value <= 300 && value >= 200) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("Down");
    }
    if(value <= 420 && value >= 385) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("Left");
    }
    if(value <= 700 && value >= 600) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("Select");
    }
    if(value >= 900) {
      lcd_gotoxy(8,1);
      lcd_puts("       ");
      lcd_gotoxy(8,1);
      lcd_puts("none");
    }*/
}
