
// STLED316S display sketch (c) 2021 David Leval
// Released under the GPLv3 license to match the rest of the
// STLED316S library

#include <Arduino.h>
#include <STLED316S.h>

//Number of LED digit connected on driver:
#define NBR_OF_DIGIT  6
//Pinout :
#define STB_PIN       8
#define CLK_PIN       13
#define DATA_PIN      11

//STLED316S driver object
STLED316S stled(NBR_OF_DIGIT, STB_PIN, CLK_PIN, DATA_PIN);

void setup() {
  //Use the parameters to configure the connection of the digits to the driver:
  stled.begin(SEG1, SEG2, SEG3, SEG4, SEG5, SEG6, SEG7, SEG8);
  //begin(uint8_t digA, uint8_t digB, uint8_t digC, uint8_t digD, uint8_t digE, uint8_t digF, uint8_t digG, uint8_t digDP)
  //digA : Output pin of STLED316S for segment a, digB : Output pin of STLED316S for segment b ...
  //Or do not use a parameter to use the default connection:
  //stled.begin();

  //Configure Brightness
  stled.setBrightness(DIGITall, 2);
  stled.setBrightnessLED(LEDall, 1);

  //Set default state
  stled.clearDisplay();
  stled.setLED(LEDall, false);
}

void loop() {
  uint32_t unbr = 0;
  int32_t nbr = 0;
  float fnbr = -1.23;

  //Display raw data:
  stled.dispRAW(DIGITall,0x20);
  delay(1000);
  stled.dispRAW(DIGITall,0x80);
  delay(1000);
  stled.dispRAW(DIGITall,0x02);
  delay(1000);
  stled.clearDisplay();

  //Display unsigned decimal number:
  for(unbr=0;unbr<22;unbr++) {
    stled.dispUdec(unbr);
    delay(200);
  }
  delay(1000);
  stled.clearDisplay();

  //Display signed decimal number:
  for(nbr=-12;nbr<14;nbr++) {
    stled.dispDec(nbr);
    delay(200);
  }
  delay(1000);
  stled.clearDisplay();

  //Display float number:
  for(uint8_t i=0;i<32;i++) {
    stled.dispFloat(fnbr, 2);
    fnbr += 0.07;
    delay(200);
  }
  delay(1000);
  stled.clearDisplay();

  //Display hexadecimal number:
  for(uint8_t i=0;i<32;i++) {
    stled.dispHex(i);
    delay(200);
  }
  delay(1000);
  stled.clearDisplay();

  //Set LED
  for(uint8_t i=0;i<8;i++) {
    stled.setLED(LEDall, true);
    delay(500);
    stled.setLED(LEDall, false);
    delay(500);
  }
}
