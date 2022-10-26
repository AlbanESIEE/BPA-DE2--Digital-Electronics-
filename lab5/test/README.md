# Lab 5: Alban FERRACANI

### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 | 000 |
   | Up     | 0.495&nbsp;V | 101 | 99 | 063 |
   | Down   | 1.203&nbsp;V | 246 | 239 | 0EF |
   | Left   | 1.970&nbsp;V | 403 | 409 | 199|
   | Select | 3.181&nbsp;V | 650 | 640 | 280 |
   | none   | 5.0&nbsp;V| 1023 | 1023 | 3FF |
   
   **Details about calculus :**

   $V_{PC0-Right} = 0V$, here there is no voltage divider.
   
   $V_{PC0-Up} = Vcc\times \frac{R_3}{R_3+R_2}=5\times \frac{330}{330+3000}=0,495V$
   
   $V_{PC0-Down} = Vcc\times \frac{(R_3+R_4)}{(R_3+R_4)+R_2}=5\times \frac{330+620}{330+620+3000}=1,203V$
   
   $V_{PC0-Left} = Vcc\times \frac{(R_3+R_4+R_5)}{(R_3+R_4+R_5)+R_2}=5\times \frac{330+620+1000}{330+620+1000+3000}=1,97V$
   
   $V_{PC0-Sel} = Vcc\times \frac{(R_3+R_4+R_5+R_6)}{(R_3+R_4+R_5+R_6)+R_2}=5\times \frac{330+620+1000+3300}{330+620+1000+3300+3000}=3,181V$
   
   $V_{PC0-none} = 5V$, here there is no voltage divider but a pull up configuration with $R_2$ connected to $V_{CC}=5V$ and $PC0$.
   
   On the `ATMEGA328P` microcontroller, the internal analog to digital converter is single ended (*lecture 6 - 25/10/22*). This means that $ADC=\frac{V_{IN}}{V_{REF}}\times (2^n-1). With $V_{IN}$ the analog signal level, $V_{REF}$ the reference level and $n$ the number of bits.
   On `ATMEGA328P` microcontroller, we have $V_{REF}=5V$ and $n=10$ bits.
   
   $ADC_{PC0-Right} = \frac{V_{PC0-none}}{5}\times (2^{10}-1) = \frac{0}{5}\times 1023 = 0$
   
   $ADC_{PC0-Up} = \frac{V_{PC0-Up}}{5}\times (2^{10}-1) = \frac{0,495}{5}\times 1023 = 101,277 \rightarrow 101$
   
   $ADC_{PC0-Down} = \frac{V_{PC0-Down}}{5}\times (2^{10}-1) = \frac{1,203}{5}\times 1023 = 246,133 \rightarrow 246$
   
   $ADC_{PC0-Left} = \frac{V_{PC0-Left}}{5}\times (2^{10}-1) = \frac{1,970}{5}\times 1023 = 403,062 \rightarrow 403$
   
   $ADC_{PC0-Sel} = \frac{V_{PC0-Sel}}{5}\times (2^{10}-1) = \frac{3,181}{5}\times 1023 = 650,833 \rightarrow 650$
   
   $ADC_{PC0-none} = \frac{V_{PC0-none}}{5}\times (2^{10}-1) = \frac{5}{5}\times 1023 = 1023$
   
   >I referred to the resistors values of Tomas Fryza's EasyEda shematic (screenshot source : https://oshwlab.com/tomas.fryza/arduino-shields).
   >
   ><img width="250" alt="Capture d’écran 2022-10-25 à 18 09 06" src="https://user-images.githubusercontent.com/114081879/197825912-c5a5daf5-b22e-4886-9e6d-312d1c9442b5.png">

 
### Temperature meter

Consider an application for temperature measurement. Use analog temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, and a LED. Every 30 seconds, the temperature is measured and the value is displayed on LCD screen. When the temperature is too high, the LED will turn on.

2. Draw a schematic of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.
  
 >Here, I drawn this schematic with EasyEda software. 
 >We use an `ATMEGA328P` microcontroller, a `TC1046`temperature sensor, a $16\times2$ LCD display and a LED which will be turn on if the temperature is too high. After reading the datasheet of the [TC1046 sensor](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), I learned that it's possible to power the sensor in $5V$ and that the output voltage of the sensor is proportional to the temperature (it will be useful when we will program this application).
 
 ![Schematic_lab5_2022-10-26](https://user-images.githubusercontent.com/114081879/198086474-52801d91-a0e3-4e24-bccf-2e8c497a2af4.svg)


3. Draw two flowcharts for interrupt handler `TIMER1_OVF_vect` (which overflows every 1&nbsp;sec) and `ADC_vect`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.
   
   >As we know, is we use `TIMER1`, we can use directly the 256 prescaler on `TIMER1` to overflow every 1s. As we 'refresh' the temperature value every 30 seconds, it's not a problem to have a refresh rate of the LCD display greater than 1 second. 
   
   | **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
   | Timer/Counter0 | 8  | 16u | 128u | -- | 1m | -- | 4m | 16m |
   | Timer/Counter1 | 16 | 4m | 33m | -- | 262m | -- | **1s** | 4.2s |
   | Timer/Counter2 | 8  | 16u | 128u | 512u | 1m | 2m | 4m | 16m |
   
   ![your figure]()
