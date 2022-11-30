/***********************************************************************
 * @author Tomas Fryza
 * @author Alban Ferracani - 252498@vutbr.cz
 * @author Lucas Lacroix - 252503@vutbr.cz 
 * 
 * @brief Digital multi-rooms thermostat with rotary encoder, joystick 
 * and LCD.
 * @date 11-30-2022 
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
#include <twi.h>            // I2C/TWI library for AVR-GCC

/* Declaration of variables -------------------------------------------*/
uint8_t mux = 0;            // Variable uint8_t mux for switching ADC mux.
uint32_t password = 0;      // Password entered is set here.

uint16_t valuex = 512;            // Value for x axis of Joystick
uint16_t valuey = 512;            // Value for y axis of Joystick
uint16_t valueclick;        // Value for click from Joystick

double temperature_room1 = 20.0;   // Value with wanted temperature room1
double temperature_room2 = 10.0;   // Value with wanted temperature room2
uint8_t room = 1;                  // Select the room

uint8_t rotary_CLK_state = 0; 
uint8_t rotary_DATA_state = 0; 
uint8_t rotary_PUSH_state;
uint8_t rotary_CLK_last_state; 
uint8_t rotary_counter = 0; 
uint8_t joystick_PUSH_state=0;

// Declaration of "air" variable with structure "Air_parameters_structure"
struct Air_parameters_structure {
    uint8_t humid_int;
    uint8_t humid_dec;
    uint8_t temp_int;
    uint8_t temp_dec;
    uint8_t checksum;
} air;


/* Defines -----------------------------------------------------------*/
#define ROTARY_CLK PB4             // Output signal CLK from rotary encoder
#define ROTARY_DATA PB5            // Output signal DATA from rotary encoder
#define ROTARY_PUSH PB3            // Output signal PUSH from rotary encoder
#define JOYSTICK_PUSH PD2          // Output signal PUSH from rotary encoder

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
uint8_t customCharMiddle[8] = {
        0b11111,
        0b11111,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b11111,
        0b11111
};
uint8_t customCharEnd[8] = {
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
      lcd_puts("Room 1 - kitchen");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      itoa(temperature_room1, string, 10);
      lcd_gotoxy(8, 1);
      lcd_puts(string);
    }

    else if (room == 2){
      lcd_clrscr();                   // We clear the display.
      lcd_gotoxy(0, 0);               // Place cursor line 0, column 0.
      lcd_puts("Room 2 - bedroom");
      lcd_gotoxy(0, 1);               // Place cursor line 1, column 0.
      lcd_puts("Temp : ");
      itoa(temperature_room2, string, 10);
      lcd_gotoxy(8, 1);
      lcd_puts(string);
    }
}

/**********************************************************************
 * Function: Read value of rotary encoder
 * Purpose:  Read value of rotary encoder and select the room in  
 *           function of rotation.
 **********************************************************************/
void read_rotary_encoder(){
    // Reading current value of CLK, DATA and PUSH signals from rotary encoder.
    rotary_CLK_state = GPIO_read(&PINB, ROTARY_CLK);      
    rotary_DATA_state = GPIO_read(&PINB, ROTARY_DATA);   
    rotary_PUSH_state = GPIO_read(&PINB, PB3);
    joystick_PUSH_state = GPIO_read(&PIND, PB2);

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
    // we have checked this with the logical analyser.

    // Debugging the rotary encoder push button signal 
    char string_dec[4];        
    itoa(rotary_PUSH_state, string_dec, 10);
    uart_puts(string_dec);

    if(rotary_PUSH_state == 0){
      display_temperature_DHT12();    // We display air temperature 
    }

    // If the push button of joystick is pressed
    // (active low : when the knob is pressed down, the voltage goes to 0.)
    // we have checked this with the logical analyser.
    if(joystick_PUSH_state == 0){
      //display_instructions();
      display_humidity_DHT12();       // We display air humidity
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
 * @name Displaying indoor temperature on LCD.
 * @brief Displaying indoor temperature on LCD, mesures are done with 
 * DH12 I2C temperature sensor.
 * @note Address of the I2C DHT12 has been found during Lab 7. 
 * @return none
 **********************************************************************/

void display_temperature_DHT12(){
    static uint8_t sla = 8;      // I2C Slave address
    uint8_t ack;                 // ACK response from Slave
    char string_int[3];          // String for converting numbers by itoa()
    char string_dec[3];          // String for converting numbers by itoa()
    
    // Read temperature from 0x5c
    sla = 0x5c;
    twi_start(sla, TWI_WRITE);
    
    // We start reading integer and decimal part of temperature only if the 
    // sensor is ready to communicate (ie ack = 0).
    if (ack == 0){   
      // Writing in 0x00 (address of humid_int), then stopping the communication 
      // and starting it again by reading the value in this register. 
      // The program will automatically shift the adress of register to the next one 
      // when using again ack() while the nack() instruction.

      // 0x00	Humidity integer part
      // 0x01	Humidity decimal part
      // 0x02	Temperature integer part
      // 0x03	Temperature decimal part

      twi_write(0x00); 
      twi_stop();
      twi_start(sla, TWI_READ);
      air.humid_int = twi_read_ack();
      air.humid_dec = twi_read_ack();
      air.temp_int = twi_read_ack();
      air.temp_dec = twi_read_nack();

      // Convert values to string
      itoa(air.temp_int, string_int, 10);
      itoa(air.temp_dec, string_dec, 10);

      // Display temperature on LCD
      lcd_clrscr();
      lcd_gotoxy(0,0);
      lcd_puts("Temperature in");
      lcd_gotoxy(0,1);
      lcd_puts("degrees");
      lcd_gotoxy(8,1);
      lcd_puts(string_int);
      lcd_gotoxy(10,1);
      lcd_puts(".");
      lcd_gotoxy(11,1);
      lcd_puts(string_dec);

      _delay_ms(2000);                // Wait 2s before clearing the display
    }
}

/**********************************************************************
 * @name Displaying indoor humidity on LCD.
 * @brief Displaying indoor temperature on LCD, mesures are done with 
 * DH12 I2C temperature sensor.
 * @note Address of the I2C DHT12 has been found during Lab 7. 
 * @note Reading registers is done in display_temperature_DHT12() function.
 * @return none
 **********************************************************************/
void display_humidity_DHT12(){

      char string_int_hum[3];          // String for converting numbers by itoa()
      char string_dec_hum[3];          // String for converting numbers by itoa()

      itoa(air.humid_int, string_int_hum, 10);    // Converting numbers by itoa()
      itoa(air.humid_dec, string_dec_hum, 10);    // Converting numbers by itoa()

      // Display humidity on LCD
      lcd_clrscr();
      lcd_gotoxy(0,0);
      lcd_puts("Humidity in the");
      lcd_gotoxy(0,1);
      lcd_puts("room");
      lcd_gotoxy(9,1);
      lcd_puts(string_int_hum);
      lcd_gotoxy(11,1);
      lcd_puts(".");
      lcd_gotoxy(12,1);
      lcd_puts(string_dec_hum);
      lcd_gotoxy(13,1);
      // We display the % sigle. To do that, we see the table of characters of LCD display.
      // the percentage sigle is represented by 0b0010_0101 so 0x25 in hexa (cf.datasheet).
      lcd_putc(0x25);                         
      _delay_ms(2000);                        // Wait 2s before clearing the display
}

/**********************************************************************
 * @name Main function where the program execution begins
 * @brief Use Timer/Counter1 and start ADC conversion every 100 ms.
 * When AD conversion ends, send converted value to LCD screen.
 * @return none
 **********************************************************************/
int main(void)
{
    // Initialize display 
    lcd_init(LCD_DISP_ON);

    // Initialize USART to asynchronous, 8N1, 9600
    // In this project, we only use UART communication with computer for 
    // program developement (helps to display few data we record). 
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Initialize I2C (TWI)
    // The I2C communication is used with one slave (DHT12 temperature and humidity sensor).
    twi_init();

    // Generating the initialisation message on LCD display
    // We display a message "Digital heating system" with a short animation
    // based on custom characters (progression bar).
    lcd_clrscr();                                         // We clear LCD.
    lcd_CreateCustomChar(customCharBegin, 0);             // Creating custom char of the begining, at position 0 in CGRAM.
    lcd_CreateCustomChar(customCharMiddle, 1);            // Creating custom char of the middle, at position 1 in CGRAM.
    lcd_CreateCustomChar(customCharEnd, 2);               // Creating custom char of the end, at position 2 in CGRAM.

    lcd_gotoxy(0,0);    
    lcd_puts("Digital heating");                          // Displaying name of system.
    lcd_gotoxy(0,1);
    lcd_puts("control");

    lcd_gotoxy(8,1);
    lcd_putc(0x00);                                       // Display symbol with Character code 0.
    _delay_ms(400);                                       // Wait for 400ms (using delay is not a problem here as we 
                                                          // are nor waiting for interactions [ie. buttons, etc].
    for (int i=9; i<15; i++){
      lcd_gotoxy(i,1);
      lcd_putc(0x01);                                     // Display symbol with Character code 1.
      _delay_ms(400);                                     // 6 times with one column shift between each.  
    }
    _delay_ms(400);
    lcd_gotoxy(15,1);   
    lcd_putc(0x02);                                       // Display symbol with Character code 2.

    /*
    // Analog embedded temperature sensor of ATMEGA328P.
    // NOTE : Finally, we don't use this embedded sensor (cf report on my github for reasons).
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

    // Set GPIO pin of joystick (push button) as input (with embedded input pullup resistor)
    GPIO_mode_input_pullup(&DDRD, JOYSTICK_PUSH);

    // Reading initial state of CLK signal from rotary encoder
    // this value is used in read_rotary_encoder() function.
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
 * @name Timer/Counter1 overflow interrupt
 * @brief Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    static uint8_t no_of_overflows = 0;
    no_of_overflows++;
    // Do this every 2 x 33 ms = 66 ms
    if (no_of_overflows >= 2)
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
        
        // We call the read_rotary_encoder() function to read the rotary encoder 
        // position. The function selects the room according to the direction of rotation.
        read_rotary_encoder(); 

        
        // If the user has selected the room 1 with the rotary encoder, 
        // the lcd displays room1 target and the user can change temperature with the
        // joystick as described previously.
        
        if (room == 1){
            // String for converted numbers by itoa()
            char string[4];  

            // Analysing x and y axis positions of joystick to increase/decrease 
            // temperature by step of ±5°C (y axis) and ±1°C (x axis).
            // The analog values for different positions of joystick are described in joystick's datasheet.

            if(valuex >=500 && valuex<= 520 && valuey >=500 && valuey<= 520){
              temperature_room1 = temperature_room1; 
              temperature_room2 = temperature_room2; 
            }
            // Joystick pushed up (north direction), 
            // we increase temperature of selected room by 5°C.
            if(valuey<100 && valuex >490){
              temperature_room1 += 3;
            }
            // Joystick pushed down (south direction),
            // we decrease temperature of selected room by 5°C.
            if(valuey>800 && valuex <530 && valuex > 490){
              temperature_room1 -= 3;
            }
            // Joystick pushed right (east direction),
            // we decrease temperature of selected room by 1°C.
            if(valuex>800 && valuey >480 && valuey<530){
              temperature_room1 -=1;
            }
            // Joystick pushed left (west direction),
            // we increase temperature of selected room by 1°C.
            if(valuex<200 & valuey>490 && valuey<530){
              temperature_room1 +=1;
            }

            // Call of display temperature target function to display the target
            // which has been set on LCD. The parameters are : 
            // - the room (1 or 2);
            // - the temperature target; 
            display_temperature_target(room, temperature_room1);  
        }

        /*
        If the user has selected the room 2 with the rotary encoder, 
        the lcd displays room2 target and the user can change temperature with the
        joystick as described previously.
        */
        if (room == 2){
            // String for converted numbers by itoa()
            char string[4];  

            // Analysing x and y axis positions of joystick to increase/decrease 
            // temperature by step of ±5°C (y axis) and ±1°C (x axis).
            // The analog values for different positions of joystick are described in joystick's datasheet.

            if(valuex >=500 && valuex<= 520 && valuey >=500 && valuey<= 520){
              temperature_room1 = temperature_room1; 
              temperature_room2 = temperature_room2; 
            }
            // Joystick pushed up (north direction), 
            // we increase temperature of selected room by 5°C.
            if(valuey<100 && valuex >490){
              temperature_room2 += 3;
            }
            // Joystick pushed down (south direction),
            // we decrease temperature of selected room by 5°C.
            if(valuey>800 && valuex <530 && valuex > 490){
              temperature_room2 -= 3;
            }
            // Joystick pushed right (east direction),
            // we decrease temperature of selected room by 1°C.
            if(valuex>800 && valuey >480 && valuey<530){
              temperature_room2 -=1;
            }
            // Joystick pushed left (west direction),
            // we increase temperature of selected room by 1°C.
            if(valuex<200 & valuey>490 && valuey<530){
              temperature_room2 +=1;
            }

            // Call of display temperature target function to display the target
            // which has been set on LCD. The parameters are : 
            // - the room (1 or 2);
            // - the temperature target; 
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
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      valuex = ADC;
    }

    /* In this case, (mux=0), that means that we have selected input channel ADC0. 
       So, we have to read the value from ADC and store it in our valuex variable.
       This value corresponds to y axis variations (range : 0-1023).
    */
    if(mux==0){
      char string[4];  // String for converted numbers by itoa()
      // Read converted value
      // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
      valuey = ADC; 
    }
}

/*
Please note : 
- to simulate on simulIDE, we use the firmware.hex file generated during compilation. 
  This file is not human readeable (cf lecture of Tomas FRYZA). 
- On simulIDE, the button (SW) of joystick is active high, but for the real one we have, it's an active low configuration.
- On simulIde, the x and y axis are inverted in comparaison with the real joystick.  
*/
