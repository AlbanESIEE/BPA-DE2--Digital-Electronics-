# Lab 7: Alban FERRACANI

### Arduino Uno pinout

1. In the picture of the Arduino Uno board, mark the pins that can be used for the following functions/operations:
   * PWM generators from Timer0, Timer1, Timer2
   * analog channels for ADC
   * UART pins
   * I2C pins
   * SPI pins
   * external interrupt pins INT0, INT1
  
<img width="1217" alt="Capture d’écran 2022-11-09 à 17 48 25" src="https://user-images.githubusercontent.com/114081879/200890616-075761e8-1e01-47a7-93bc-0be7fc0f7b2a.png">


### I2C communication

2. Draw a timing diagram of I2C signals when calling function `rtc_read_years()`. Let this function reads one byte-value from RTC DS3231 address `06h` (see RTC datasheet) in the range `00` to `99`. Specify when the SDA line is controlled by the Master device and when by the Slave device. Draw the whole request/receive process, from Start to Stop condition. The image can be drawn on a computer (by [WaveDrom](https://wavedrom.com/) for example) or by hand. Name all parts of timing diagram.

>I draw the timing diagram with [WaveDrom](https://wavedrom.com/), as for the previous lab.

![wavedrom](https://user-images.githubusercontent.com/114081879/200950462-fb356518-a88a-473f-804b-ef3c54345281.svg)

>Here is the javascript code I used to generate the timing diagram.
```c
{signal: [

  
  {name: 'SDA', wave: '101.010........1.0..101.010..10....1.0.1.01'},
  {},
  {name: 'SCL', wave: '1.n..................0p..................1.'},
  
  { name: "transmission typology",        wave: "x5444444453444444453x544444445344444444453x", data: ["start","A6", "A5", "A4", "A3","A2","A1","A0","R/W","ACK","A6", "A5", "A4", "A3","A2","A1","A0","R/W","ACK","start","A6", "A5", "A4", "A3","A2","A1","A0","R/W","ACK","D7","D6", "D5", "D4", "D3","D2","D1","D0","R/W","NACK", "stop"] },

  { name: "transmission typology",        wave: "x65.......75.......7x65.......75........76x", data: ["start","Slave address + W (0xD0) ", "ACK", "Register address + W (0x0C)","ACK","start","Slave address + R (0xD1)", "ACK","Read 1 byte from slave", "NACK", "stop"] },
  { name: "master / slave control",        wave: "x5........55.......5x5.........5........5.x", data: ["M","S","M","S","S","S","M"] },

]
}
```


### Meteo station

Consider an application for temperature and humidity measurements. Use sensor DHT12, real time clock DS3231, LCD, and one LED. Every minute, the temperature, humidity, and time is requested from Slave devices and values are displayed on LCD screen. When the temperature is above the threshold, turn on the LED.

3. Draw a flowchart of `TIMER1_OVF_vect` (which overflows every 1&nbsp;sec) for such Meteo station. The image can be drawn on a computer or by hand. Use clear description of individual algorithm steps.

![Capture d’écran 2022-11-09 à 23 29 57](https://user-images.githubusercontent.com/114081879/200955991-f812e497-d681-43be-9f6c-4f2c47690b24.png)

