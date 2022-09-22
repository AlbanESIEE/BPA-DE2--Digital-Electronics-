/***********************************************************************
 * 
 * Blink a LED in Arduino-style and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2022 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Defines -----------------------------------------------------------*/
#define LED_GREEN PB5   // PB5 is AVR pin where green on-board LED 
                        // is connected
#define LED_RED PB0

//////////////////////////////////////////////////////////////////
////////// DECLARATION OF VARIABLES FOR MORSE CODE ///////////////
//////////////////////////////////////////////////////////////////
/*
In this part, we define the delay in morse code to express one dot and 
one dash. 
--> Short delay is the duration of a dot
--> Long delay is the duration of a dash
*/
#define SHORT_DELAY 200 // Delay in milliseconds for a dot (.)
#define LONG_DELAY 700 // Delay in milliseconds for a dash (__)

/*
In this part, we define the delay in morse code to express one letter.
*/
#define SPACE_SAME_LETTER 2000
//////////////////////////////////////////////////////////////////



#ifndef F_CPU
# define F_CPU 16000000 // CPU frequency in Hz required for delay funcs
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>     // AVR device-specific IO definitions
#include <util/delay.h> // Functions for busy-wait delay loops


// -----
// This part is needed to use Arduino functions but also physical pin
// names. We are using Arduino-style just to simplify the first lab.
#include "Arduino.h"
#define PB5 13          // In Arduino world, PB5 is called "13"
#define PB0 8 
// -----


/* Blink function ----------------------------------------------*/
/**********************************************************************
 * Function: Function with a string in parameter : "SHORT" or "LONG"
 * --> This function permit to blink the led (ON, delay, OFF, delay) defined 
 *     previously with SHORT_DELAY and LONG_DELAY. 
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none (void)
 **********************************************************************/

void blink (String delay_short_long){
    /*
    If the parameter sent is "SHORT", then we use a short delay which corresponds 
    to a dot. We make the led blink only once per function "blink" call.
    */
    if (delay_short_long == "SHORT"){
        digitalWrite(LED_RED, HIGH);
        delay(SHORT_DELAY);
        digitalWrite(LED_RED, LOW);
        delay(SHORT_DELAY);
    }
    /*
    If the parameter sent is "LONG", then we use a long delay which corresponds 
    to a dash. We make the led blink only once per function "blink" call.
    */
    if (delay_short_long == "LONG"){
        digitalWrite(LED_RED, HIGH);
        delay(LONG_DELAY);
        digitalWrite(LED_RED, LOW);
        delay(LONG_DELAY);
    }
}

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none
 **********************************************************************/


int main(void)
{
    uint8_t led_value = LOW;  // Local variable to keep LED status

    // Set pin where on-board LED is connected as output
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    // Infinite loop
    while (1)
    {   
        /*
        In this part, we generate the sequence 'DE2' : (.__  .  ..__ __ __). 
        * Beetween two leters, we use the SPACE_SAME_LETTER delay. 
        * To generate a brief flash (dot .), we call the blink() function with "SHORT" parameter.
        * To generate a long flash (dash __), we call the blink() function with "LONG" parameter.
        */

        //Generating the letter 'D' (.__)
        blink("SHORT");
        blink("LONG");
        delay(SPACE_SAME_LETTER);

        //Generating the letter 'E' (.)
        blink("SHORT");
        delay(SPACE_SAME_LETTER);

        //Generating the letter '2' (..__ __ __)
        blink("SHORT");
        blink("SHORT");
        blink("LONG");
        blink("LONG");
        blink("LONG");

        //Then, we wait 5 seconds before repeating the morse sequence 'DE2' (to simplify visual interpretation of the morse code)
        delay(5000);  
    }

    // Will never reach this
    return 0;
}

