/***********************************************************************
 * @author Tomas Fryza
 * @author Alban Ferracani - 252498@vutbr.cz
 * @author Lucas Lacroix - 252503@vutbr.cz 
 * 
 * @brief Digital multi-rooms thermostat with rotary encoder, joystick 
 * and LCD.
 * @date 11-28-2022 
 * 
 * @remarks (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * This program has been updaped by Alban FERRACANI and Lucas LACROIX. 
 * The libraries used has been provided by Tomas Fryza during 
 * semester labs of BPA-DE2 units.
 * 
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <util/delay.h>     // Delays library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions
#include <uart.h>           // Peter Fleury's UART library

/* Declaration of variables -------------------------------------------*/
uint8_t mux = 0;            // Variable uint8_t mux for switching ADC mux.
uint32_t password = 0;      // Password entered is set here.

uint16_t valuex = 512;            // Value for x axis of Joystick
uint16_t valuey = 512;            // Value for y axis of Joystick
uint16_t valueclick;        // Value for click from Joystick

uint32_t temperature_room1 = 20;   // Value with wanted temperature room1
uint32_t temperature_room2 = 10;   // Value with wanted temperature room2
uint8_t room = 1;                  // Select the room

uint8_t rotary_CLK_state = 0; 
uint8_t rotary_DATA_state = 0; 
uint8_t rotary_PUSH_state = 0; 
uint8_t rotary_CLK_last_state; 
uint8_t rotary_counter = 0; 


/* Defines -----------------------------------------------------------*/
#define ROTARY_CLK PB4             // Output signal CLK from rotary encoder
#define ROTARY_DATA PB5            // Output signal DATA from rotary encoder
#define ROTARY_PUSH PB3            // Output signal PUSH from rotary encoder

// Custom character definition using https://omerk.github.io/lcdchargen/
uint8_t customCharBegin[8] = {
        0b11111,
        0b11111,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11111,
        0b11111
};
uint8_t customCharMiddle[7] = {
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111
};
uint8_t customCharEnd[6] = {
        0b11111,
        0b11111,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b11111,
        0b11111
};


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Display target temperature on LCD. 
 * Purpose:  Display converted value on LCD screen.
 * @param temperature temperature target to display.
 **********************************************************************/
void display_temperature_target(uint8_t room, uint16_t temperature){
    
    char string[4];  // String for converted numbers by itoa()

    if(room == 1){
      lcd_clrscr();                   // We clear the display.
      lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
      lcd_puts("Room 1 - target");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      itoa(temperature_room1, string, 10);
      lcd_gotoxy(8, 1);
      lcd_puts(string);
    }

    else if (room == 2){
      lcd_clrscr();                   // We clear the display.
      lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
      lcd_puts("Room 2 - target");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      itoa(temperature_room2, string, 10);
      lcd_gotoxy(8, 1);
      lcd_puts(string);
    }
}

/**********************************************************************
 * Function: Display instructions on LCD
 * Purpose:  Display instructions on LCD if the push button of the 
 *           rotary encoder is pressed. 
 **********************************************************************/

void display_instructions(){
    lcd_clrscr();                   // We clear the display.
    lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
    lcd_puts("Joy y: ±2, x:±0,5");
    lcd_gotoxy(0, 1);               // Place cursor line 0, column 0.
    lcd_puts("Rot: select room");
    _delay_ms(1000);
}


/**********************************************************************
 * Function: Read value of rotary encoder
 * Purpose:  Read value of rotary encoder and select the room in  
 *           function of rotation.
 **********************************************************************/
void read_rotary_encoder(){
    // Reading current value of CLK, DATA and PUSH signals from rotary encoder.
    rotary_CLK_state = GPIO_read(&PINB, ROTARY_CLK);      
    rotary_DATA_state = GPIO_read(&PORTB, ROTARY_DATA);   
    rotary_PUSH_state = GPIO_read(&PORTB, ROTARY_PUSH);

    // If the initial clock value is not the same than the previous, that means 
    // the encoder has been turned (the direction of rotation is not yet taken into account).
    // To avoid double count, we react to only 1 state change. 
    if(rotary_CLK_state != rotary_CLK_last_state){ //&& rotary_CLK_state == 1
        // If the DATA state is different from CLK state, we are turning clockwise.
        // So, we switch the counter (which corresponds to the room number to 1).
        // If we had more than two rooms, we should increment or decrement the counter to have
        // more than two states.
        if(rotary_DATA_state != rotary_CLK_state){
          //rotary_counter = 1; 
          room = 1;
        }
        // If the DATA state is equal to CLK state, we are turning counterclockwise.
        // So, we switch the counter (which corresponds to the room number to 0).
        else {
          //rotary_counter = 0; 
          room = 2; 
        }
    }
    // We update the value of CLK last state by the new one read.
    rotary_CLK_last_state = rotary_CLK_state; 

    // If the push button of rotary encoder is pressed
    // (active low : when the knob is pressed down, the voltage goes to 0.)
    if(rotary_PUSH_state == 0){
      display_instructions();
    }
}

/**********************************************************************
 * @name Creating a custom char for LCD from pixel map.
 * @brief  Push the custom char in memory of CGRAM from pixel matrix.
 * @note we can create maximum 8 custom characters
 * @param customCharToCreate pointer containing the address of the pattern
 * @param locationCGRAM position of custom char in CGRAM (0 to 7) 
 * @note Works for HD44780 controller (for LCD)
 * @return none
 **********************************************************************/

void lcd_CreateCustomChar(unsigned char *customCharToCreate, const char locationCGRAM)
{
    lcd_command((1 << LCD_CGRAM) + locationCGRAM*8);  //Set the addressing to CGRAM (Character Generator RAM)
                                                    // ie to individual lines of character patterns                      

    for (uint8_t i=0; i<8; i++){
      lcd_data (customCharToCreate[i]);             //Pass the bytes of pattern on LCD 
    }
    lcd_command(1<<LCD_DDRAM);       // Set addressing back to DDRAM (Display Data RAM)
                                     // ie to character codes
}


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

    // Generating the initialisation message on LCD display
    lcd_clrscr();                                         // We clear LCD.
    lcd_CreateCustomChar(customCharBegin, 0);             // Creating custom char of the begining, at position 0 in CGRAM.
    lcd_CreateCustomChar(customCharMiddle, 1);            // Creating custom char of the middle, at position 1 in CGRAM.
    lcd_CreateCustomChar(customCharEnd, 2);               // Creating custom char of the end, at position 2 in CGRAM.

    lcd_gotoxy(0,0);    
    lcd_puts("Digital heating");                          // Displaying name of system.
    lcd_gotoxy(0,1);
    lcd_puts("control");

    lcd_gotoxy(8,1);
    lcd_putc(0x00);                      // Display symbol with Character code 0.
    _delay_ms(400);

    for (int i=9; i<15; i++){
      lcd_gotoxy(i,1);
      lcd_putc(0x01);                    // Display symbol with Character code 1
      _delay_ms(400);                    // 6 times with one column shift between each.  
    }
    _delay_ms(400);
    lcd_gotoxy(15,1);
    lcd_putc(0x02);                      // Display symbol with Character code 2

    /*
    // Analog embedded temperature sensor of ATMEGA328P.
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

    // Set GPIO pins of rotary encoder as input (with embedded input pullup resistor)
    GPIO_mode_input_pullup(&DDRB, ROTARY_CLK);
    GPIO_mode_input_pullup(&DDRB, ROTARY_DATA);
    GPIO_mode_input_pullup(&DDRB, ROTARY_PUSH);

    // Reading initial state of CLK signal from rotary encoder
    rotary_CLK_last_state = GPIO_read(&PORTB, ROTARY_CLK);

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
          mux=0;
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
        
            
              ---------------------------------
              | Temperature set with joystick |
              ---------------------------------

                           + 5°C
                            | |          
                            | |        
                            | |       
                   ..................... x axis 
            -1.0°C ..................... +1.0°C
                            | | 
                            | | 
                            | | \
                           -5°C  \
                                  y axis 
        */
        
        
        
        /* Calling the function read_rotary_encoder() which defines the room to display and set in 
        function of the rotation direction of encoder.

              -----------------------------------
              | Select room with rotary encoder |
              -----------------------------------
        

                    ROTATION      |  
                                  |  --> Select room n°1
                    clockwise     |  
              >>>------>>>--------->>>--------->>>

                    ..............
                  ..................
                 ...              ...
                ....    ROTARY    ....
                ....    ENCODER   ....
                 ...              ...
                  ..................
                    ..............
                  

                    ROTATION         |  
                                     |  --> Select room n°0
                    counterclockwise |  
              <<<------<<<---------<<<---------<<<
        */
        read_rotary_encoder(); 
        
        /*
        If the user has selected the room 1 with the rotary encoder, 
        the lcd displays room1 target and the user can change temperature with the
        joystick as described previously.
        */
        if (room == 1){

            // String for converted numbers by itoa()
            char string[4];  

            // Analysing y axis joystick position to increase/decrease 
            // temperature by step of ±2°C.

            if(valuex >=500 && valuex<= 520 && valuey >=500 && valuey<= 520){
              temperature_room1 = temperature_room1; 
              temperature_room2 = temperature_room2; 
            }
            
            if(valuex<100 && valuey >500){
              temperature_room1 += 5;
            }
            if(valuex>1000 && valuey <520 && valuey > 500){
              temperature_room1 -= 5;
            }
            if(valuey<10 && valuex >500 && valuex<520){
              temperature_room1 -=1;
            }
            if(valuey>1000 & valuex>500 && valuex<520){
              temperature_room1 +=1;
            }
            /*
            // A DECOMMENTER POUR LE JOYSTICK REEL

            if(valuey<100 && valuex >500){
              temperature_room1 += 5;
            }
            if(valuey>1000 && valuex <520 && valuex > 500){
              temperature_room1 -= 5;
            }
            if(valuex<10 && valuey >500 && valuey<520){
              temperature_room1 -=1;
            }
            if(valuex>1000 & valuey>500 && valuey<520){
              temperature_room1 +=1;
            }
            */
            //valuex = 512;
            //valuey = 512;


          
            /*
            // +2°C position on joystick 
            if (valuey > 700){
              temperature_room1 += 5;
              valuey = 512;
              valuex = 512;
            }
            // -2°C position on joystick 
            else if (valuey < 300){
              temperature_room1 -= 5;
              valuey = 512;
              valuex = 512;
            }

            // Analysing x axis joystick position to increase/decrease 
            // temperature by step of ±0.5°C.
            
            // -0.5°C position on joystick 
            if (valuex > 700){
              temperature_room1 -= 1;
              valuey = 512;
              valuex = 512;
            }
            // +0.5°C position on joystick 
            else if (valuex <300){
              temperature_room1 += 1;
              valuey = 512;
              valuex = 512;
            }
            */
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

              // String for converted numbers by itoa()
              char string[4];  

              // +2°C position on joystick 
              if (valuey > 700){
                temperature_room2 += 5;
                valuey = 512;
                valuex = 512;
              }
  
              // -2°C position on joystick 
              else if (valuey < 300){
                temperature_room2 -= 5;
                valuey = 512;
                valuex = 512;
              }

              // Analysing x axis joystick position to increase/decrease 
              // temperature by step of ±0.5°C.
              
              // -0.5°C position on joystick 
              if (valuex > 700){
                temperature_room2 -= 1; 
                valuex = 512;
                valuey = 512;
              }
              // +0.5°C position on joystick 
              else if (valuex <300){
                temperature_room2 += 1; 
                valuey = 512;
                valuex = 512;
              }
              display_temperature_target(room, temperature_room2);
          }
    }    
    // Start ADC conversion
    ADCSRA = ADCSRA | (1<<ADSC);
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
      //lcd_gotoxy(0,0);
      //lcd_puts("val x");
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      valuex = ADC;
      
      /*
      // Convert "value" to "string" and display it
      itoa(valuex, string, 10);
      lcd_gotoxy(6,0);
      //To erease previous value on LCD.
      lcd_puts("     ");
      // Display value on lcd.
      lcd_gotoxy(6,0);
      lcd_puts(string);
      */
    }

    /* In this case, (mux=0), that means that we have selected input channel ADC0. 
       So, we have to read the value from ADC and store it in our valuex variable.
       This value corresponds to y axis variations (range : 0-1023).
    */
    if(mux==0){
      // Select input channel ADC0 (voltage divider pin)
      //ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2| 1<<MUX1| 1<<MUX0);
      
      
      char string[4];  // String for converted numbers by itoa()
      //lcd_gotoxy(0,1);
      //lcd_puts("val y");
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      
      valuey = ADC;
      // Convert "value" to "string" and display it
      
      /*
      itoa(valuey, string, 10);
      lcd_gotoxy(6,1);
      lcd_puts("     ");
      lcd_gotoxy(6,1);
      lcd_puts(string);
      */
      
    }

    
  
    /* In this case, (mux=2), that means that we have selected input channel ADC2. 
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
          //lcd_gotoxy(11,1);
          //lcd_puts("click");
      }
      // Button not pressed
      else { 
          //lcd_gotoxy(11,1);
          //lcd_puts("     ");
      }
    }
}
