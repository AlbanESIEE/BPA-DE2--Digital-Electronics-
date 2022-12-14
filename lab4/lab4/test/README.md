# Lab 4: Alban FERRACANI

### Stopwatch

1. Draw a flowchart for `TIMER2_OVF_vect` interrupt service routine which overflows every 16&nbsp;ms but it updates the stopwatch LCD screen approximately every 100&nbsp;ms (6 x 16&nbsp;ms = 100&nbsp;ms). Display tenths of a second, seconds, and minutes and let the stopwatch counts from `00:00.0` to `59:59.9` and then starts again. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

First of all, in the main loop, we have to initialize LCD display. Moreover, we have to set timer 2 on 16ms (using functions we created in the previous labs). We don't forget to *enable* the timer 2 overflow. After that, we have to enable interrupts globally with `sei` block. Then, it's the timer 2 which will interrupt the loop and go to the **interrupt service routine**. 

With a conditionnal statement, we test if `number`is greater than 6, if it is, that means that we have $16\times 6 = 100ms$, so we increment the diferent variables (tenths, secs, min) in function they are greater to 9 for tenths, 59 for seconds and 59 for minutes. If not, we continue incrementing under-branch variable. 
Finally, if minutes are greater to 59, that means that we reset the timer to 0 minutes, 0 seconds and 0 tenths and we count again. 

>I designed this flowchart on *Wondershare EdrawMax software*. 

<img width="978" alt="Capture d’écran 2022-10-20 à 13 30 32" src="https://user-images.githubusercontent.com/114081879/196937044-2793dfd4-fa65-4596-8788-4fd83d16f2c8.png">



### Kitchen alarm

2. Draw a schema of the kitchen alarm application that counts down the time with an LCD, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking. The image can be drawn on a computer or by hand. Always name all components and their values.

For the kitchen alarm, I have represented the electrical diagram on the following scheme (I have made it with EasyEda). We have 3 push buttons, with a pull down resistor. Moreover, I added one LED which will start blinking at the end of the counter. The calculus of resistors values are the same than in the previous labs. I represented also the 16*2 LCD display. the particularity here is that I used a transistor (as a relay) to control the LCD backlight. This backlight command is provided with `PB2` of the ATMEGA328P microcontroller. 
  ![Sheet_1](https://user-images.githubusercontent.com/114081879/196925510-eedbc530-bb83-4efd-9dfc-e487e89c82b2.svg)

>As we learned in 25 October lecture, I also could use only one analog port on the ATMEGA328P microcontroller (for the three push-buttons) by using a voltage devider configuration. It just assumes that you don't have to press several buttons at the same time, but that's not the case with our configuration.
This may be a great issue if we use a lot of pins and we have not enough Analog pins available, here just one would be sufficient.
