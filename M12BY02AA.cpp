/*
 M12BY02AA.cpp - I2C driver for Futaba M12BY02AA vacuum fluorescent display
 
 Author: Simon Schori <simon.schori@gmail.com>
 Version: 0.3
 
 Datasheet: http://docs-asia.electrocomponents.com/webdocs/0f06/0900766b80f06185.pdf
 
 Addressing of the display is done by hardwiring A0 through A2 (Pins 4-7) to GND.
 The pins are internally pulled-up, so leaving them open means address 7. 

 TODO:
  - make lowercase characters
  - general characters cleanup
  - move chars array into Vfd class
  - accept text as String object
  - reduce vfd flickering at every update (how?)
  - store brightness in eeprom
  - clean up code

*/

#include "Arduino.h"
#include "M12BY02AA.h"
#include "Wire.h"

Vfd::Vfd(byte addr) {
	this->_addr = addr;
	this->_i2cAddr = vfdIdBase + addr;
  this->_vfdI2cId = _i2cAddr << 1;
	this->_ledStateRegister = 7;
	Wire.begin(); // join i2c bus (address optional for master)
}

void Vfd::init() {
  /* note: init() is needed after every Wire.endTransmission(), else the display stays off.
     Excessive use of init() leads to more flickering.
  */
  if (brightness > 15) { 
    brightness = 15; 
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(_vfdI2cId);
  Wire.write(B10010000 | brightness);
  Wire.endTransmission();
}

void Vfd::dataStart(byte offset) {
  /* @param offset sets the RAM offset in bytes. Every characters occupies 3 bytes
  */
  Wire.beginTransmission(_i2cAddr);
  Wire.write(_vfdI2cId);
  Wire.write(vfdOutRamAutoIncrement);
  Wire.write(vfdRamStartAddr + offset);
}

void Vfd::dataStop() {
  Wire.endTransmission();
}

void Vfd::ledControl (byte color, boolean enabled) {
  if (enabled) {
    // LED is on when it's respective bit is 0
    bitClear(_ledStateRegister, color); 
  } 
  else {
    bitSet(_ledStateRegister, color); 
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(_vfdI2cId);       
  Wire.write(vfdOutLed);
  Wire.write(_ledStateRegister);
  Wire.endTransmission();
  init();
}

void Vfd::sendMessage(char* text, byte length) {
  byte length2 = 0;
  if (length > 10) {
    length2 = length;
    length = 10;
  }
  dataStart(0);
  for (byte i = 0; i < length; i++) {
    sendChar(text[i]);
  }
  dataStop();

  if (length2) {
    dataStart(30);
    for (byte j = 10; j < length2; j++) {
      sendChar(text[j]);
    }
    dataStop();
  }
  init();
}

void Vfd::clear() {
  sendMessage("            ",12);
}

void Vfd::sendChar(char ch) {
  if (ch < 32 | ch > ASCIIMAX) {
    ch = 126;
  }
  ch-=32; // Transform ASCII code to array index
  Wire.write(chars[ch][0]);
  Wire.write(chars[ch][1]);
  Wire.write(chars[ch][2]);
}

void Vfd::sendCharPos(char ch, byte pos) {
  dataStart(pos*3);
  sendChar(ch);
  dataStop();
  init();
}
