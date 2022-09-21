# Lab 1: FERRACANI Alban

### Morse code

1. Listing of C code which repeats one "dot" and one "comma" *(here i use a dash, maybe it's simpler to visualize)* (BTW, in Morse code it is letter `A`) on a LED. Always use syntax highlighting, meaningful comments, and follow C guidelines:

In this part of code, I defined the constants (delays) which are used after to emit the morse code. 

>*We have to note that in comparaison with the original template you have given us, I added ```LONG_DELAY```constant, I could multiply 
the ```SHORT_DELAY```constant but I thought it's simpler to adjust by adding this constant.* 

```c
//////////////////////////////////////////////////////////////////
////////// DECLARATION OF CONSTANTS FOR MORSE CODE ///////////////
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
```

In this part of the code, I created a ```main``` function (execution in loop) and a ```blink``` function which is called in the ```blink``` function.

```c
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
```

2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()

### Additional comments
1. In this part, you will find the simulation that I did on
[tinkercad](https://www.google.com](https://www.tinkercad.com/things/2sn0hOOQTmw-start-simulating/editel?lessonid=EHD2303J3YPUS5Z&projectid=OT2JZ1PL20FZRMO&collectionid=undefined&sharecode=D2L9wMkQyfJz_Y5SIL3fVYiO3MtQYJY7FO_ZcHH1aUc) (by clicking on the link you can access to my simulation online, because I think that ```git```doesn't support videos). 


2. Here is the morse "alphabet" I extracted from this webpage and I used to generate the ```DE2```word.
![International_Morse_Code](https://user-images.githubusercontent.com/114081879/191604578-2fcbe14d-88e2-4801-b795-4e4eabc882fb.png)
