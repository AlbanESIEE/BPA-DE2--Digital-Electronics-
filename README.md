## Welcome to my GIT page
You will find on this repository the content of the DE2 course followed during my ERASMUS student mobility in BRNO University of Technology.

# Lab 5: Alban FERRACANI

### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** | **ADC value (measured, hex)** |
   | :-: | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 | 0 |
   | Up     | 0.495&nbsp;V | 101 |  |  |
   | Down   | 1.203&nbsp;V | 246 |  |  |
   | Left   | 1.970&nbsp;V |  |  |  |
   | Select | 3.181&nbsp;V |  |  |  |
   | none   |  |  |  |  |
   
   *Details about calculus :*

   $V_{PC0-Right} = 0V$, here there is no voltage divider.
   
   $V_{PC0-Up} = Vcc\times \frac{R_3}{R_3+R_2}=5\times \frac{330}{330+3000}=0,495V$
   
   $V_{PC0-Down} = Vcc\times \frac{(R_3+R_4)}{(R_3+R_4)+R_2}=5\times \frac{330+620}{330+620+3000}=1,203V$
   
   $V_{PC0-Left} = Vcc\times \frac{(R_3+R_4+R_5)}{(R_3+R_4+R_5)+R_2}=5\times \frac{330+620+1000}{330+620+1000+3000}=1,97V$
   
   $V_{PC0-Sel} = Vcc\times \frac{(R_3+R_4+R_5+R_6)}{(R_3+R_4+R_5+R_6)+R_2}=5\times \frac{330+620+1000+3300}{330+620+1000+3300+3000}=3,181V$
   
   I referred to the values of Tomas Fryza's EasyEda shematic. 
   <img width="497" alt="Capture d’écran 2022-10-25 à 18 09 06" src="https://user-images.githubusercontent.com/114081879/197825912-c5a5daf5-b22e-4886-9e6d-312d1c9442b5.png">

### Temperature meter

Consider an application for temperature measurement. Use analog temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, and a LED. Every 30 seconds, the temperature is measured and the value is displayed on LCD screen. When the temperature is too high, the LED will turn on.

2. Draw a schematic of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure]()

3. Draw two flowcharts for interrupt handler `TIMER1_OVF_vect` (which overflows every 1&nbsp;sec) and `ADC_vect`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()
