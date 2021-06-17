# STLED316S

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![version](https://img.shields.io/badge/version-1.0.2-blue)

:star: Star us on GitHub — it helps!

Arduino Library for STLED316S LED controller with keyscan


[Link to STLED316S product page](https://www.st.com/en/power-management/stled316s.html)

## Features

- Adjustment of the correspondence between the driver outputs and the display segments
- Drive individualy each Digit LED display
- Drive individualy each LED output
- Display of raw data, decimal number or hexadecimal number
- Brightness adjustment of each digit and LED
- Keyscan reading (possibility on interruption)
- (option) Hardware SPI -> Development in progress (keyscan not yet available)

## Version

- Version 1.0.0 -- Intial version
- Version 1.0.1 -- Addition of a private variable to save the state of the LEDs.
- Version 1.0.2 -- Fix vtable linker error 

## Arduino Compatibility

- Arduino Uno (5V) : Tested
- Arduino Mega (5V) : Tested
- Arduino Due : Not tested
- STM32 : Not tested
- ESP8266 : Not tested
- ESP32 : Not tested
- ...

## Schematic example

See the electronic diagram available in the 'extras' directory.

## Installation

Download STLED316S from this Github repository, un-zip or extract the files and place the STLED316S directory in your _.../Arduino/libraries/..._ folder. 
The _.../Arduino/..._ folder is the one where your Arduino IDE sketches are usually located.

## Usage

### STLED316S Library

Include the STLED316S.h file at the beginning of your sketch:

```Cpp
#include <STLED316S.h>
```

The object should be created with only the parameters:
- Number of digit
- STB pin (chip select)
- Clock pin 
- DIN/DOUT pin

Example:
```Cpp
#define NBR_OF_DIGIT  2
#define STB_PIN       8
#define CLK_PIN       13
#define DATA_PIN      11

STLED316S stled(NBR_OF_DIGIT, STB_PIN, CLK_PIN, DATA_PIN);
```

### Device Initialisation

To initialise the STLED316S it is necessary to call the begin() function.

Without parameters :
```Cpp
stled.begin();
```

With parameters -> Adjustment of the correspondence between the driver outputs and the display segments:
begin(uint8_t digA, uint8_t digB, uint8_t digC, uint8_t digD, uint8_t digE, uint8_t digF, uint8_t digG, uint8_t digDP)
with :
- digA : Output pin of STLED316S for segment a
- digB : Output pin of STLED316S for segment b
- digC : Output pin of STLED316S for segment c
- digD : Output pin of STLED316S for segment d
- digE : Output pin of STLED316S for segment e
- digF : Output pin of STLED316S for segment f
- digG : Output pin of STLED316S for segment g
- digDP : Output pin of STLED316S for segment DP
Values : (STLED316S outputs) SEG1, SEG2, SEG3, SEG4, SEG5, SEG6, SEG7 or SEG8

Example:
```Cpp
stled.begin(SEG6, SEG5, SEG3, SEG2, SEG1, SEG7, SEG8, SEG4);
```

---
## __Example Code__
- __STLED316S_display.ino__ : Control LED and digit
- __STLED316S_keyscan.ino__ : Read status of Keyscan

## License

You can redistribute it and/or  modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.


