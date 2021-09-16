
// STLED316S spi sketch (c) 2021 David Leval
// Released under the GPLv3 license to match the rest of the
// STLED316S library

// Reading the STLED316S registers is not possible in hardware SPI mode. Therefore the program cannot read the Keyscan status

#include <Arduino.h>
#include <STLED316S.h>

//Number of LED digit connected on driver:
#define NBR_OF_DIGIT  6
//Pinout :
#define STB_PIN       8

//STLED316S driver object
STLED316S_SPI stled(NBR_OF_DIGIT, STB_PIN);

uint32_t unbr = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("\nSTLED316S SPI test program");
  
  stled.begin();
  // Or use the parameters to configure the connection of the digits to the driver:
  // stled.begin(SEG1, SEG2, SEG3, SEG4, SEG5, SEG6, SEG7, SEG8);

  //Configure Brightness
  stled.setBrightness(DIGITall, 2);

  //Set default state
  stled.clearDisplay();
}

void loop() {
  stled.dispUdec(unbr++);
  delay(200);
}
