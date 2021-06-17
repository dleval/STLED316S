/**
 * @file STLED316S.cpp
 * @brief Arduino Library for STLED316S LED controller with keyscan
 * @author David Leval
 * @version 1.0.2
 * @date 17/06/2021
 * 
 * STLED316S library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * <http://www.gnu.org/licenses/>.
 */
 
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "STLED316S.h"

/******************************************************************************/
/* Constructor Functions                                                      */
/******************************************************************************/
STLED316S_Common::STLED316S_Common(uint8_t nbrOfDigit)
{
	_nbrOfDigit = nbrOfDigit;
}

STLED316S_SPI::STLED316S_SPI(uint8_t nbrOfDigit) : STLED316S_Common(nbrOfDigit)
{
	pinMode(_STBpin, OUTPUT);

	//Set chip select to default state
	digitalWrite(_STBpin, HIGH);
	//Initialize the Arduino SPI library
	SPI.begin();
}

STLED316S_SPI::STLED316S_SPI(uint8_t nbrOfDigit, uint8_t STBpin) : STLED316S_Common(nbrOfDigit)
{
	pinMode(STBpin, OUTPUT);
	_STBpin = STBpin;

	//Set chip select to default state
	digitalWrite(_STBpin, HIGH);
	//Initialize the Arduino SPI library
	SPI.begin();
}

STLED316S::STLED316S(uint8_t nbrOfDigit, uint8_t STBpin, uint8_t CLKpin, uint8_t DATApin) : STLED316S_Common(nbrOfDigit)
{
	_STBpin = STBpin;
	_CLKpin = CLKpin;
	_DATApin = DATApin;

	pinMode(_STBpin, OUTPUT);
	pinMode(_CLKpin, OUTPUT);
	pinMode(_DATApin, OUTPUT);
	//Set pin to default state
	digitalWrite(_STBpin, HIGH);
	digitalWrite(_CLKpin, HIGH);
	digitalWrite(_DATApin, LOW);
}

/******************************************************************************/
/* Class STLED316S_SPI Public Functions                                       */
/******************************************************************************/
/**
 * @brief Sending data to the STLED316S with SPI
 * 
 * @param *data : Address of the first byte to send
 * @param lenght : Number of data to send
 */
void STLED316S_SPI::writeData(uint8_t *data, uint8_t lenght)
{
    uint8_t i;
    uint8_t* p_data = data;
    uint8_t SendByte;
    
	SPI.beginTransaction(SPISettings(100000, LSBFIRST, SPI_MODE0)); //Send LSB first
    digitalWrite(_STBpin, LOW);//STB_L();
    delayMicroseconds(2);   
    for(i=0;i<lenght;i++) {
        SendByte = *p_data; 
		//SendByte = SwapBit(*p_data); //if SPI send MSB first
        SPI.transfer(SendByte);
        p_data++;
    } 
    delayMicroseconds(2);
    digitalWrite(_STBpin, HIGH);//STB_H();
	SPI.endTransaction();
}

/******************************************************************************/
/* Class STLED316S_SPI Private Functions                                      */
/******************************************************************************/
/**
 * @brief Endian converter\n
 * Swap every bit in 8-bit data\n
 * b7 b6 b5 b4 b3 b2 b1 b0 -> b0 b1 b2 b3 b4 b5 b6 b7
 * 
 * @param byte : Data to convert
 * @return : Result
 */
uint8_t STLED316S_SPI::SwapBit(uint8_t byte)
{
    return
            (byte & 0x80) >>7 | (byte & 0x40) >>5 | (byte & 0x20) >>3 | (byte & 0x10) >>1 |
            (byte & 0x08) <<1 | (byte & 0x04) <<3 | (byte & 0x02) <<5 | (byte & 0x01) <<7;
}

/******************************************************************************/
/* Class STLED316S Public Functions                                           */
/******************************************************************************/
/**
 * @brief Sending data to the STLED316S without SPI
 * 
 * @param *data : Address of the first byte to send
 * @param lenght : Number of data to send
 */
void STLED316S::writeData(uint8_t *data, uint8_t lenght)
{
	uint8_t i;
	uint8_t* p_data = data;
    uint8_t SendByte;

	digitalWrite(_CLKpin, HIGH);
	digitalWrite(_STBpin, LOW);//STB_L();
	delayMicroseconds(2); 
	for(i=0;i<lenght;i++) {
		SendByte = *p_data; 
		sendSW_SPI(SendByte);
		p_data++;
	}
	delayMicroseconds(2);
    digitalWrite(_STBpin, HIGH);//STB_H();
}

/**
 * @brief Reading data byte from STLED316S
 * 
 * @param address : Address of the data
 * @return data : Data read
 */
uint8_t STLED316S::readData(uint8_t address)
{
	uint8_t j;
    uint8_t SendByte = STLED316S_DATA_RD|STLED316S_READ_PAGE|address;
	uint8_t ReadByte = 0;

	digitalWrite(_CLKpin, HIGH);
	digitalWrite(_STBpin, LOW);//STB_L();
	delayMicroseconds(2); 

	sendSW_SPI(SendByte);

	pinMode(_DATApin, INPUT_PULLUP);
	for(j=0;j<8;j++) {
		digitalWrite(_CLKpin, LOW);
		delayMicroseconds(1);
		digitalWrite(_CLKpin, HIGH);
		if(digitalRead(_DATApin)) ReadByte |= (0x01 << j);
		delayMicroseconds(1);
	}

	
	delayMicroseconds(2);
    digitalWrite(_STBpin, HIGH);//STB_H();
	pinMode(_DATApin, OUTPUT);

	return ReadByte;
}

/******************************************************************************/
/* Class STLED316S Private Functions                                          */
/******************************************************************************/
/**
 * @brief Simulate SPI transfer with delay
 * 		  (freq. = 500kHz)
 * @param data : byte to send
 */
void STLED316S::sendSW_SPI(uint8_t data)
{
	uint8_t j;
	
	for(j=0;j<8;j++) {
		if(data & (0x01 << j)) digitalWrite(_DATApin, HIGH);
		else digitalWrite(_DATApin, LOW);
		digitalWrite(_CLKpin, LOW);
		delayMicroseconds(1);
		digitalWrite(_CLKpin, HIGH);
		delayMicroseconds(1);
	}
}

/******************************************************************************/
/* Class STLED316S_Common Public Functions                                    */
/******************************************************************************/
/**
 * @brief Initializes the STLED316S driver
 * 
 */
void STLED316S_Common::begin(void)
{
	//Wait until everything is initialized
	delayMicroseconds(20);
	//Initialize the first data of display buffer
	_dispDataBuffer[0] = STLED316S_DATA_WR | STLED316S_DIG_PAGE | STLED316S_ADDR_INC;
	//Puts STLED316S in default state (default brightness, clear, ON)
	setBrightness(DIGITall, STLED316S_DEFAULT_BRIGHTNESS);
	setBrightnessLED(LEDall, STLED316S_DEFAULT_BRIGHTNESS);
	clearDisplay();
	displayON();
}

/**
 * @brief Initializes the STLED316S driver with segments parameters
 * 
 * @param digA : Output pin of STLED316S for segment a
 * @param digB : Output pin of STLED316S for segment b
 * @param digC : Output pin of STLED316S for segment c
 * @param digD : Output pin of STLED316S for segment d
 * @param digE : Output pin of STLED316S for segment e
 * @param digF : Output pin of STLED316S for segment f
 * @param digG : Output pin of STLED316S for segment g
 * @param digDP : Output pin of STLED316S for segment DP
 */
void STLED316S_Common::begin(uint8_t digA, uint8_t digB, uint8_t digC, uint8_t digD, uint8_t digE, uint8_t digF, uint8_t digG, uint8_t digDP)
{
	_digDP = digDP;

	//b7->SEG8 / b6->SEG7 / b5->SEG6 / b4->SEG5 / b3->SEG4 / b2->SEG3 / b1->SEG2 / b0->SEG1
	_digit_table[0x0] = digA | digB | digC | digD | digE | digF;
	_digit_table[0x1] = digB | digC;
	_digit_table[0x2] = digA | digB | digD | digE | digG;
	_digit_table[0x3] = digA | digB | digC | digD | digG;
	_digit_table[0x4] = digB | digC | digF | digG;
	_digit_table[0x5] = digA | digC | digD | digF | digG;
	_digit_table[0x6] = digA | digC | digD | digE | digF | digG;
	_digit_table[0x7] = digA | digB | digC;
	_digit_table[0x8] = digA | digB | digC | digD | digE | digF | digG;
	_digit_table[0x9] = digA | digB | digC | digD | digF | digG;
	_digit_table[0xA] = digA | digB | digC | digE | digF | digG;
	_digit_table[0xB] = digC | digD | digE | digF | digG;
	_digit_table[0xC] = digA | digD | digE | digF;
	_digit_table[0xD] = digB | digC | digD | digE | digG;
	_digit_table[0xE] = digA | digD | digE | digF | digG;
	_digit_table[0xF] = digA | digE | digF | digG;
	
	begin();
}

/**
 * @brief Turn ON the STLED316S display
 * 
 */
void STLED316S_Common::displayON(void)
{
    uint8_t data = STLED316S_DISP_ON_CMD;
    writeData(&data,1);
}

/**
 * @brief Turn OFF the STLED316S display
 * 
 */
void STLED316S_Common::displayOFF(void)
{
    uint8_t data = STLED316S_DISP_OFF_CMD; 
    writeData(&data,1);
}

/**
 * @brief Set the LED display brightness.
 * 
 * @param DIGITnum : DIGITall->All or Digit number (DIGITn1..DIGITn6)
 * @param brightness : (3 significant bits, valid range 0..7 (1/16 .. 14/16 dutycycle) 
 */
void STLED316S_Common::setBrightness(DIGITnum_t DIGITnum, uint8_t brightness)
{
    uint8_t data[5];
    
    data[0] = STLED316S_DATA_WR | STLED316S_CONF1_PAGE | STLED316S_ADDR_INC;
    //data[1] = (brightness<<5)|STLED316S_CONF_BRT_CONSTANT|STLED316S_CONF_DIGIT;
    data[1] = STLED316S_CONF_BRT_VARIABLE|(_nbrOfDigit-1);
	switch(DIGITnum)
	{
		case DIGITall:
			_digit_brightness[0] = (brightness<<4) | brightness;;
			_digit_brightness[1] = _digit_brightness[0];
			_digit_brightness[2] = _digit_brightness[0];
			break;
		case DIGITn1:
			_digit_brightness[0] = (_digit_brightness[0] & 0xF0) | brightness;
			break;
		case DIGITn2:
			_digit_brightness[0] = (brightness<<4) | (_digit_brightness[0] & 0x0F);
			break;
		case DIGITn3:
			_digit_brightness[1] = (_digit_brightness[1] & 0xF0) | brightness;
			break;
		case DIGITn4:
			_digit_brightness[1] = (brightness<<4) | (_digit_brightness[1] & 0x0F);
			break;
		case DIGITn5:
			_digit_brightness[2] = (_digit_brightness[2] & 0xF0) | brightness;
			break;
		case DIGITn6:
			_digit_brightness[2] = (brightness<<4) | (_digit_brightness[2] & 0x0F);
			break;
	}
    data[2] = _digit_brightness[0];
    data[3] = _digit_brightness[1];
    data[4] = _digit_brightness[2];
    
    writeData(&data[0],5);
}

/**
 * @brief Clear display
 * 
 */
void STLED316S_Common::clearDisplay(void)
{
	//_dispDataBuffer[0] = STLED316S_DATA_WR | STLED316S_DIG_PAGE | STLED316S_ADDR_INC;
	for(uint8_t i=0;i<_nbrOfDigit;i++) _dispDataBuffer[i+1] = 0x00;
    
    writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}

/**
 * @brief Controls the LEDs of a digit with raw data
 * 
 * @param DIGITnum : DIGITall->All or Digit number (DIGITn1..DIGITn6)
 * @param raw : b7->SEG8 / b6->SEG7 / b5->SEG6 / b4->SEG5 / b3->SEG4 / b2->SEG3 / b1->SEG2 / b0->SEG1
 */
void STLED316S_Common::dispRAW(DIGITnum_t DIGITnum, uint8_t raw)
{
	uint8_t i;

    if(DIGITnum > _nbrOfDigit) return;

	if(DIGITnum==DIGITall) for(i=0;i<_nbrOfDigit;i++) _dispDataBuffer[i+1] = raw;
    else _dispDataBuffer[DIGITnum] = raw;
    
    writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}

/**
 * @brief Controls the LEDs of all digit with raw data buffer
 * 
 * @param *raw : Address of the first byte
 * b7->SEG8 / b6->SEG7 / b5->SEG6 / b4->SEG5 / b3->SEG4 / b2->SEG3 / b1->SEG2 / b0->SEG1
 */
void STLED316S_Common::dispRAW(uint8_t *raw)
{
	uint8_t i;
	uint8_t* raw_data = raw;
	
	for(i=0;i<_nbrOfDigit;i++) {
		_dispDataBuffer[i+1] = *raw_data;
		raw_data++;
	}
	writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}

/**
 * @brief Display a unsigned number
 * 
 * @param nbr : Unsigned Decimal Number
 */
void STLED316S_Common::dispUdec(uint32_t nbr)
{
	if(nbr<100000) _dispDataBuffer[6] = 0x00 | (_dispDataBuffer[6]); //Hide the 0 of the upper digit
    else _dispDataBuffer[6] = _digit_table[(nbr/100000)%10];
	if(nbr<10000) _dispDataBuffer[5] = 0x00 | (_dispDataBuffer[5]); //Hide the 0 of the upper digit
    else _dispDataBuffer[5] = _digit_table[(nbr/10000)%10];
	if(nbr<1000) _dispDataBuffer[4] = 0x00 | (_dispDataBuffer[4]); //Hide the 0 of the upper digit
    else _dispDataBuffer[4] = _digit_table[(nbr/1000)%10];
	if(nbr<100) _dispDataBuffer[3] = 0x00 | (_dispDataBuffer[3]); //Hide the 0 of the upper digit
    else _dispDataBuffer[3] = _digit_table[(nbr/100)%10];
    if(nbr<10) _dispDataBuffer[2] = 0x00 | (_dispDataBuffer[2]); //Hide the 0 of the upper digit
    else _dispDataBuffer[2] = _digit_table[(nbr/10)%10];
    _dispDataBuffer[1] = _digit_table[nbr%10];
	
	writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}

/**
 * @brief Display a hexadecimal data
 * 
 * @param data : Hex
 */
void STLED316S_Common::dispHex(uint32_t data)
{
	_dispDataBuffer[1] = _digit_table[data&0x0000000F];
	_dispDataBuffer[2] = _digit_table[(data&0x000000F0)>>4];
	
	writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}

/**
 * @brief Activation or deactivation of the DP of a digit
 * 
 * @param DIGITnum : DIGITall->All or Digit number (DIGITn1..DIGITn6)
 * @param state : 0->OFF, 1->ON
 */
void STLED316S_Common::setDP(DIGITnum_t DIGITnum, uint8_t state)
{
	uint8_t i;

	if(DIGITnum > _nbrOfDigit) return;

	if(state) {
		if(DIGITnum==DIGITall) for(i=0;i<_nbrOfDigit;i++) _dispDataBuffer[i+1] |= _digDP;
		else _dispDataBuffer[DIGITnum] |= _digDP;
	} else {
		if(DIGITnum==DIGITall) for(i=0;i<_nbrOfDigit;i++) _dispDataBuffer[i+1] &= ~_digDP;
		else _dispDataBuffer[DIGITnum] &= ~_digDP;
	}
    
    writeData(&_dispDataBuffer[0],_nbrOfDigit+1);
}


/**
 * @brief Set the LED brightness.
 * 
 * @param LEDnum : LEDall->All or LED number (LEDn1..LEDn8)
 * @param brightness : (3 significant bits, valid range 0..7 (1/16 .. 14/16 dutycycle) 
 */
void STLED316S_Common::setBrightnessLED(LEDnum_t LEDnum, uint8_t brightness)
{
	uint8_t data[5];
	switch(LEDnum)
	{
		case LEDall:
			_LED_brightness[0] = (brightness<<4) | brightness;
			_LED_brightness[1] = _LED_brightness[0];
			_LED_brightness[2] = _LED_brightness[0];
			_LED_brightness[3] = _LED_brightness[0];
			break;
		case LEDn1:
			_LED_brightness[0] = (_LED_brightness[0] & 0xF0) | brightness;
			break;
		case LEDn2:
			_LED_brightness[0] = (brightness<<4) | (_LED_brightness[0] & 0x0F);
			break;
		case LEDn3:
			_LED_brightness[1] = (_LED_brightness[1] & 0xF0) | brightness;
			break;
		case LEDn4:
			_LED_brightness[1] = (brightness<<4) | (_LED_brightness[1] & 0x0F);
			break;
		case LEDn5:
			_LED_brightness[2] = (_LED_brightness[2] & 0xF0) | brightness;
			break;
		case LEDn6:
			_LED_brightness[2] = (brightness<<4) | (_LED_brightness[2] & 0x0F);
			break;
		case LEDn7:
			_LED_brightness[3] = (_LED_brightness[3] & 0xF0) | brightness;
			break;
		case LEDn8:
			_LED_brightness[3] = (brightness<<4) | (_LED_brightness[3] & 0x0F);
			break;
	} 
	
	data[0] = STLED316S_DATA_WR | STLED316S_BRT_LED_PAGE | STLED316S_ADDR_INC;
    data[1] = _LED_brightness[0];
    data[2] = _LED_brightness[1];
    data[3] = _LED_brightness[2];
    data[4] = _LED_brightness[3];
	
	writeData(&data[0],5);
}

/**
 * @brief Set the LED state.
 * 
 * @param LEDnum : LEDall->All or LED number (LEDn1..LEDn8)
 * @param state : true or false
 */
void STLED316S_Common::setLED(LEDnum_t LEDnum, bool state)
{
	uint8_t data[2];
    
	if(LEDnum == LEDall) {
		if(state) _LED_state = 0xFF;
		else _LED_state = 0x00;
	} else {
		if(state) _LED_state |= LEDnum;
		else _LED_state &= ~LEDnum;
	}
    
    data[0] = STLED316S_DATA_WR | STLED316S_LED_PAGE;
    data[1] = _LED_state;
    writeData(&data[0],2);
}

/**
 * @brief default implementation of the writing function 
 */
void STLED316S_Common::writeData(uint8_t *data, uint8_t lenght)
{

}

/**
 * @brief default implementation of the reading function 
 */
uint8_t STLED316S_Common::readData(uint8_t address)
{
	return 0;
}
