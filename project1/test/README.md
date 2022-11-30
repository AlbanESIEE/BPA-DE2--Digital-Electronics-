# Project 1 - Multiroom digital thermostat

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 10th and 11th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester, ie PlatformIO and not the Arduino-style. No other development tools are allowed.

## Our project

### Team members
* Alban FERRACANI, 252498@vutbr.cz
* Lucas LACROIX, 252503@vutbr.cz

### Description of the projet
The goal of this project is to develop a multiroom digital setpoint thermostat. To do this, we use :
- a rotary encoder to select the room (rotation) and display air temperature (push button), 
- a 2-axis joystick to set the temperature set-point and to display air humidity (push button), 
- one LCD display module which is used as UI (user interface) to read data entered.
- one DHT12 sensor (with I2C commmunication) to read air temperature and humidity.

>The project is running thanks to `ATMEGA328` microcontroller and it is written in C. The implementation on microcontroller is performed with PlatformIO and Microsoft Visual Studio Code.

## Hardware description

Insert descriptive text and schematic(s) of your implementation.

>Here is a schematic of the project implementation we designed with `EasyEda`. For this project, we use one analog joy-stick (2 ADC channels, 1 push button), rotary encoder (with active low push button), one Digilent PmodCLP LCD module and ond DHT12 I2C sensor.
>The LCD module is wired in the same way as in [Tomas Fryza PCB shields schematic](https://oshwlab.com/tomas.fryza/arduino-shields).

![Schematic_Digital electronics 2 (DE2)_2022-11-30](https://user-images.githubusercontent.com/114081879/204892318-fbbd6490-a683-4474-9b9c-97f51b8b66cd.svg)

>In the following tab, we have mapped sensors pins to Arduino Board pins. The Arduino Uno board pinout is [accessible here](https://docs.arduino.cc/static/6ec5e4c2a6c0e9e46389d4f6dc924073/4ef49/Pinout-UNOrev3_latest.png). We also describe from the `ATMEGA328` point of view if connections are configured as inputs or outputs (I/O).
>We use severals documentations and datashets : 
>- Documentation of `Digilent PmodCLP LCD` module is [available here](https://digilent.com/reference/_media/pmod:pmod:pmodCLP_rm.pdf).
>- Documentation of `rotary encoder` (with very clear animations and schematics) is [available here](https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/).
>- Documentation of `analog joystick` (with very clear animations and schematics) is [available here](https://lastminuteengineers.com/joystick-interfacing-arduino-processing/).
>- Documentation of `DHT12 sensor`(provided by Tomas Fryza) is [available here](https://github.com/tomas-fryza/digital-electronics-2/blob/master/docs/dht12_manual.pdf).

|Sensor|Sensor PIN |Arduino PIN | Arduino I/O|
| :-- | :-: | :-: | :-- |
| Joystick| `VRX` | `PC0` | Input - analog |
| Joystick| `VRY` | `PC1` | Input - analog |
| Joystick| `SW` | `PC2` | Input (analog or digital) |
| Rotary encoder| `CLK` | `PB4` | Input - digital |
| Rotary encoder| `DAT` | `PB5` | Input - digital|
| Rotary encoder| `SW` | `PB6` |Input - digital|
| LCD module| `E` | `PB1` | Output - digital|
| LCD module| `R` | `GND` | |
| LCD module| `RS` | `PB0` | Output - digital |
| LCD module| `DB4` | `PB4` | Output - digital |
| LCD module| `DB5` | `PB5` | Output - digital |
| LCD module| `DB6` | `PB6` | Output - digital |
| LCD module| `DB7` | `PB7` | Output - digital |
| DHT12 sensor| `SDA` | `PC4` | I/O |
| DHT12 sensor| `SCL` | `PC5` | I/O |

>Please note we didn't mentionned VCC and GND connections in this tab for mor clarity.

## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

For the project, we use the following repository structure :
 ```c
   DE2-Project                    // PlatfomIO project
   ├── include                    // Included files
   │    └── timer.h
   ├── lib                        // Libraries
   │    └── gpio                  
   │    |    └── gpio.c     
   │    |    └── gpio.h
   │    └── lcd
   │    |    └── lcd.c
   │    |    └── lcd.h
   │    |    └── lcd_definitions.h
   |    └── uart                  // Only used to debug code
   |    |       └── uart.c
   |    |       └── uart.h
   |    └── twi                   // Used for DHT12 sensor I2C communication
   |    |       └── twi.c
   |    |       └── twi.h
   ├── src                         // Source file(s)
   │   └── main.c
   ├── test                        // Report of this project
   ├── platformio.ini              // Project Configuration File
   └── README.md                   
   ```

To can continue the project at home beetween the two labs, we have used `SimulIDE` software to simulate our implementation. All the components were available (Arduino UNO board, joystick, LCD, rotary encoder except the DHT12 sensor). 
To flash the microcontroller, we used the `firmware.hex` file generated by Visual Studio Code `(.pio/build/uno/firmware.hex)`.
Here is a quick video of the simulation (without DHT12 sensor): 

https://user-images.githubusercontent.com/114081879/204880199-a9cc6c6b-8720-4e2a-addc-4d74591a4109.mov


Here is a flowchart of our [`main.c`](https://github.com/AlbanESIEE/BPA-DE2--Digital-Electronics-/blob/master/project1/src/main.c) program. We used Wondershare EdrawMax software to design it.
- Direct link to [`lib`](https://github.com/AlbanESIEE/BPA-DE2--Digital-Electronics-/tree/master/project1/lib) folder.
- Direct link to [`main.c`](https://github.com/AlbanESIEE/BPA-DE2--Digital-Electronics-/blob/master/project1/src/main.c) file.

<img width="1102" alt="flowchart_part_1" src="https://user-images.githubusercontent.com/114081879/204890552-585a6b1e-f3be-4e28-ab48-4da607fbcb3a.png">

<img width="1101" alt="flowchart_part_2" src="https://user-images.githubusercontent.com/114081879/204890571-d17871d9-dbc4-4a6a-bd25-51bcf7643a55.png">

### Reading joystick analog values

The joystick works with two potentiometers (one per axis). 
Let's mention that to know the joystick position, we need to read two analog values (for `axis x` and `axis y`). That means when we set parameters of ADC, we have to switch beetween ADC inputs (to read the fist axis analog value, then the second axis analog value) because we can't read both without changing input configuration.
To answer this requirement, we have decided to use a `switch/case` structure to set the ADC input (`ADMUX` register, by setting `MUX3`, `MUX2`, `MUX1` and `MUX0` bits). This process is described in [ATMEGA328 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf), section 24.9 "Register Description".

>To check our values measured with the microcontroller, we have used the following diagram, we extracted from [this website](https://lastminuteengineers.com/joystick-interfacing-arduino-processing/). Before using joystick to set temperature setpoint, we have displayed the analog values for both axis on LCD module (to check and understand them).

![PS2-Joystick-Module-Movement-Analog-Values-on-Arduino](https://user-images.githubusercontent.com/114081879/204052074-d7b06fc1-0512-4b54-9ec8-2c7e04a5071b.png)

Our implementation of the joystick in the project can be resumed by the following figure. 
In fact, if the user...
- push the joystick **up** : we increase the room temperature of 5°C.
- push the joystick **down** : we decrease the room temperature of 5°C.
- push the joystick **right** : we decrease the room temperature of 1°C.
- push the joystick **left** : we increase the room temperature of 1°C.
```c 
              ---------------------------------
              | Temperature set with joystick |
              ---------------------------------

                           + 5°C
                            | |          
                            | |        
                            | |       
                   ..................... x axis 
            +1.0°C ..................... -1.0°C
                            | | 
                            | | 
                            | | \
                           -5°C  \
                                  y axis 
```

### Reading rotary encoder digital values
The room is selected with the rotary encoder. When the user turns the encoder... 
- **clockwise**, we select room 1.
- **counterclockwise**, we select room 0.
The rotary encoder has two digital output signals. The first one is the `CLOCK`, it is active when the user turns the encoder.
The second one is the `DATA`. It is also active during rotation but there is an offset (phase shift) between the clock signal and the data signal. Depending on whether we have a phase advance or phase late delay, we can easily deduce the direction of rotation.
>For software implementation of rotary encoder, we need to read the both signals and to read the `DATA` pin and compare it with the current state of the `CLOCK` pin. As we use digital I/O ports, we don't have the same problem that for ADC conversion where we had to change ADC input in time.

### Reading temperature and humidity values by I2C
We use the I2C temperature and humidity sensor. This sensor has a nice precision in comparison with the embedded temperature sensor of `ATMEGA328` which has a ±10°C precision.
>We have tried to use the embedded temperature sensor of `ATMEGA328` following process described in microcontroller [datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf), *(Table 24-4, page 258)*.
- Firstly, we have to switch beetween ADC ports to `ADC8` Multiplexer Selection Register. 
- After, we have to change the reference voltage to 1.1V (ie. `REFS1 = 1` and `REFS0 = 1`), that is a problem because to read other analog values (from joystick), we use $AV_{CC}$ with external capacitor at AREF pin (ie. `REFS1 = 0` and `REFS0 = 1`).
- Then, we have to read the analog value and convert it in voltage according to the reference voltage (1.1V).
- Finally, the documentation provides us only three characteristics points we have in this tab :

|Temperature / °C| Voltage / mV |
|:--:|:--:|
|-45°C|242mV|
|+25°C|314mV|
|+85°C|380mV|

Using Microsoft Excel, we have determined the equation of the affine function considering a linear characterictic (ie $f(x)=ax+b$). By replacing $x$ by the voltage read by ADC8, we can deduce the temperature with a ±10°C precision.

![ATMEGA328_characteristics_excel](https://user-images.githubusercontent.com/114081879/204874390-2506165e-e058-4256-9605-b3826ff0e890.svg)

>So, as we saw it was not usefull to use this embedded sensor with the ±10°C precision and the need to change reference voltage. We have decided to use (with the agreement of Tomas Fryza) the I2C DHT12 sensor.

As we saw in lab 7 of Digital Electronics 2, according to the [DHT12 manual](https://github.com/tomas-fryza/digital-electronics-2/blob/master/docs/dht12_manual.pdf), the internal DHT12 data registers have the following structure. As we want to read humidity and temperature, we start at address `0x00` and the program will continue automatically to the next address while using `nack` instruction. 

|Register address|	Description|
|:--|:--|
|`0x00`| Humidity integer part|
|`0x01`| Humidity decimal part|
|`0x02`| Temperature integer part|
|`0x03`| Temperature decimal part|
|`0x04`| Checksum |

So, if the user : 
- pushes the **rotary encoder button**, we display the **temperature** of the room (as on *figure 1* below).
- pushes the **joystick switch**, we display the **humidity** of the room (as on *figure 2* below).

|Displaying temperature *(figure 1)*| Displaying humidity *(figure 2)*|
|:--:|:--:|
|![IMG_6263 Grande](https://user-images.githubusercontent.com/114081879/204875703-c43922aa-0f49-48d7-9b26-dcd16da57875.jpeg)|![IMG_6265 Grande](https://user-images.githubusercontent.com/114081879/204875736-96c4d15a-27e8-4252-b8a2-e597e7bf7feb.jpeg)|


## Documentation of the project

We used [doxygen](https://doxygen.nl/) tool to generate $\LaTeX$ and HTML documentation of the project.
The documentation is available in PDF by following [this link](https://github.com/AlbanESIEE/BPA-DE2--Digital-Electronics-/files/10087620/refman.pdf). 

To generate documentation in PDF with $\LaTeX$, we have to be in the doxygen working repository and in the folder latex.
After, we use the command `make pdf` to generate the PDF file.


## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Write your text here.
2. ...