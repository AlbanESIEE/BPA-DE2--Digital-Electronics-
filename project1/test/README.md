# Project 1 - Multiroom digital thermostat

Application of analog joy-stick (2 ADC channels, 1 push button), rotary encoder, and Digilent PmodCLP LCD module.

## Instructions

The goal of the project is cooperation in pairs, further study of the topic, design of own solutions, implementation, creation of project documentation and presentation of results. The division of roles and tasks within the team is up to their members.

* Students work on a project in the labs during the 10th and 11th weeks of the semester.

* Through the BUT e-learning, students submit a link to the GitHub repository, which contains the complete project in PlatfomIO and a descriptive README file. The submission deadline is the day before the next laboratory, i.e. in 2 weeks.

* The source code for the AVR must be written in C and/or Assembly and must be implementable on Arduino Uno board using toolchains from the semester, ie PlatformIO and not the Arduino-style. No other development tools are allowed.

## Recommended GitHub repository structure

   ```c
   YOUR_PROJECT        // PlatfomIO project
   ├── include         // Included files
   ├── lib             // Libraries
   ├── src             // Source file(s)
   │   └── main.c
   ├── test            // No need this
   ├── platformio.ini  // Project Configuration File
   └── README.md       // Report of this project
   ```

## Recommended README.md file structure

### Team members

* Alban FERRACANI, 252498@vutbr.cz
* Lucas LACROIX, 252503@vutbr.cz

## Hardware description

Insert descriptive text and schematic(s) of your implementation.

>Here is a schematic of the project implementation we designed with `EasyEda`. For this project, we use one analog joy-stick (2 ADC channels, 1 push button), rotary encoder (with active low push button), and one Digilent PmodCLP LCD module.
>The LCD module is wired in the same way as in [Tomas Fryza PCB shields schematic](https://oshwlab.com/tomas.fryza/arduino-shields).

![project](https://user-images.githubusercontent.com/114081879/203870031-c122c04f-a4bb-45ab-9847-a770181ee708.svg)

>In the following tab, we have mapped sensors pins to Arduino Board pins. The Arduino Uno board pinout is [accessible here](https://docs.arduino.cc/static/6ec5e4c2a6c0e9e46389d4f6dc924073/4ef49/Pinout-UNOrev3_latest.png). We also describe from the `ATMEGA328` point of view if connections are configured as inputs or outputs (I/O).
>We use severals documentations and datashets : 
>- Documentation of Digilent PmodCLP LCD module is [available here](https://digilent.com/reference/_media/pmod:pmod:pmodCLP_rm.pdf).
>- Documentation of rotary encoder (with very clear animations and schematics) is [available here](https://lastminuteengineers.com/rotary-encoder-arduino-tutorial/).
>- Documentation of analog joystick (with very clear animations and schematics) is [available here](https://lastminuteengineers.com/joystick-interfacing-arduino-processing/).


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




## Software description

Put flowchats of your algorithm(s). Write descriptive text of your libraries and source files. Put direct links to these files in `src` or `lib` folders.

## Video

Insert a link to a short video with your practical implementation example (1-3 minutes, e.g. on YouTube).

## References

1. Write your text here.
2. ...