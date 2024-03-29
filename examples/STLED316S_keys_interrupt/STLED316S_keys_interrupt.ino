
// STLED316S keys interrup sketch (c) 2021 David Leval
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
#define INT_PIN       2

//STLED316S driver object
STLED316S stled(NBR_OF_DIGIT, STB_PIN, CLK_PIN, DATA_PIN);

bool doReadKeys = false;

void dispHello(void);
void dispInterrupt(void);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("\nSTLED316S Keys interrupt test program");
  
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

  //Display Hello
  dispHello();
  delay(1000);
  stled.clearDisplay();

  //Interrupt configuration
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), dispInterrupt, FALLING);
}

void loop() 
{
  uint16_t data;
  
  if(doReadKeys)
  {
    data = stled.readKeyScan();
    Serial.print("data readed : ");
    Serial.println(data);
    stled.dispHex(data);
    doReadKeys = false;
  }
}

void dispHello(void)
{
  uint8_t rawHelloData[NBR_OF_DIGIT] = {0x3F, 0x38, 0x38, 0x79, 0x76, 0x00};
  stled.dispRAW(rawHelloData);
}

void dispInterrupt(void)
{
  doReadKeys = true;
}
