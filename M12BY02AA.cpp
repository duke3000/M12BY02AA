/*
 M12BY02AA.cpp - I2C driver for Futaba M12BY02AA vacuum fluorescent display
 
 Author: Simon Schori <simon.schori@gmail.com>
 Version: 0.2
 
 Datasheet: http://docs-asia.electrocomponents.com/webdocs/0f06/0900766b80f06185.pdf
 
 Addressing of the display is done by hardwiring A0 through A2 (Pins 4-7) to GND.
 The pins are internally pulled-up, so leaving them open means address 7. 

 TODO:
  - make lowercase characters
  - move chars array into Vfd class
  - accept Strings as text
  - reduce vfd flickering at every update
  - store brightness in eeprom
  - clean up code

*/

#include "Arduino.h"
#include "M12BY02AA.h"
#include "Wire.h"

static byte chars[95][3] = {
  { B00000000, B00000000, 0 }, // space
  { B00000000, B01010000, 0 }, // !
  { B00010000, B00010000, 0 }, // "
  { B00000000, B00000000, 0 }, // #
  { B01101101, B10010101, 0 }, // $
  { B11100100, B10101110, 0 }, // %
  { B11100100, B10101110, 0 }, // &
  { B00000000, B00010000, 0 }, // '
  { B10000000, B00100000, 0 }, // (
  { B00000000, B00010010, 0 }, // )
  { B11000000, B11111111, 0 }, // *
  { B01000000, B11010101, 0 }, // +
  { B00000000, B10000010, 0 }, // ,
  { B01000000, B10000100, 0 }, // -
  { B00000000, B10000000, 0 }, // .
  { B00000000, B10100010, 0 }, // /
  { B00111111, B10100010, 0 }, // 0
  { B00110000, B00000000, 0 }, // 1
  { B01011011, B10000100, 0 }, // 2
  { B01111001, B10000000, 0 }, // 3
  { B01110100, B10000100, 0 }, // 4
  { B01101101, B10000100, 0 }, // 5
  { B01101111, B10000100, 0 }, // 6
  { B00111000, B00000000, 0 }, // 7
  { B01111111, B10000100, 0 }, // 8
  { B01111101, B10000100, 0 }, // 9
  { B00000000, B01000000, 0 }, // :
  { B00000000, B01000001, 0 }, // ;
  { B00111111, B00000000, 0 }, // <
  { B00000000, B00000000, 0 }, // =
  { B00000000, B00000000, 0 }, // >
  { B00001000, B10100001, 0 }, // ?
  { B01011111, B10100000, 0 }, // @
  { B01111110, B10000100, 0 }, // A
  { B01111001, B11010001, 0 }, // B
  { B00001111, B00000000, 0 }, // C
  { B00111001, B11010001, 0 }, // D
  { B01001111, B10000100, 0 }, // E
  { B01001110, B10000100, 0 }, // F
  { B01101111, B10000000, 0 }, // G
  { B01110110, B10000100, 0 }, // H 
  { B00001001, B11010001, 0 }, // I
  { B00110011, B00000000, 0 }, // J
  { B10000110, B10100000, 0 }, // K
  { B00000111, B00000000, 0 }, // L
  { B00110110, B10101000, 0 }, // M
  { B10110110, B10001000, 0 }, // N
  { B00111111, B00000000, 0 }, // O
  { B01011110, B10000100, 0 }, // P
  { B10111111, B00000000, 0 }, // Q
  { B11011110, B10000100, 0 }, // R
  { B01101101, B10000100, 0 }, // S
  { B00001000, B11010001, 0 }, // T
  { B00110111, B00000000, 0 }, // U
  { B00000110, B10100010, 0 }, // V
  { B10110110, B10000010, 0 }, // W
  { B10000000, B10101010, 0 }, // X
  { B01010100, B10000101, 0 }, // Y
  { B00001001, B10100010, 0 }, // Z
  { B00001111, B00000000, 0 }, // [
  { B00000000, B10100010, 0 }, // /
  { B00111001, B00000000, 0 }, // ]
  { B00000000, B00000000, 0 }, // ^
  { B00000001, B00000000, 0 }, // _
  { B00000000, B00001000, 0 }, // `
  { B01111110, B10000100, 0 }, // a
  { B01100111, B10000100, 0 }, // b
  { B00001111, B00000000, 0 }, // c
  { B00111001, B11010001, 0 }, // d
  { B01001111, B10000100, 0 }, // e
  { B01001110, B10000100, 0 }, // f
  { B01101111, B10000000, 0 }, // g
  { B01110110, B10000100, 0 }, // h 
  { B00001001, B11010001, 0 }, // i
  { B00110011, B00000000, 0 }, // j
  { B10000110, B10100000, 0 }, // k
  { B00000111, B00000000, 0 }, // l
  { B00110110, B10101000, 0 }, // m
  { B10110110, B10001000, 0 }, // n
  { B00111111, B00000000, 0 }, // o
  { B01011110, B10000100, 0 }, // p
  { B10111111, B00000000, 0 }, // q
  { B11011110, B10000100, 0 }, // r
  { B01101101, B10000100, 0 }, // s
  { B00001000, B11010001, 0 }, // t
  { B00110111, B00000000, 0 }, // u
  { B00000110, B10100010, 0 }, // v
  { B10110110, B10000010, 0 }, // w
  { B10000000, B10101010, 0 }, // x
  { B01010100, B10000101, 0 }, // y
  { B00001001, B10100010, 0 }, // z
  { B10000000, B10100000, 0 }, // {
  { B00000000, B11010001, 0 }, // |
  { B01000000, B10001010, 0 }, // }
  { B11111111, B11111111, 0 }  // ~
};  

Vfd::Vfd(byte addr) {
	_addr = addr;
	_i2cAddr = vfdIdBase + addr;
	_ledStateRegister = 7;
	Wire.begin(); // join i2c bus (address optional for master)
}

void Vfd::init() {
  if (brightness > 15) { 
    brightness = 15; 
  }
  if (brightness < 0) { 
    brightness = 0; 
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(vfdI2cId);
  Wire.write(B10010000 | brightness);
  Wire.endTransmission();
}

void Vfd::dataStart(byte offset) {
  Wire.beginTransmission(_i2cAddr);
  Wire.write(vfdI2cId);
  Wire.write(vfdOutRamAutoIncrement);
  Wire.write(ramStartAddr + offset);
}

void Vfd::dataStop() {
  Wire.endTransmission();
}

void Vfd::ledControl (char color, boolean enabled) {
  byte ledBit = 0;
  switch (color) {
  case 'g' :
    ledBit = ledGreenBit;
    break;
  case 'r' :
    ledBit = ledRedBit;    
    break;
  case 'y' :
    ledBit = ledYellowBit;
    break;
  }
  if (enabled) {
    // LED is on when it's respective bit is 0
    bitClear(_ledStateRegister, ledBit); 
  } 
  else {
    bitSet(_ledStateRegister, ledBit); 
  }
  Wire.beginTransmission(_i2cAddr);
  Wire.write(vfdI2cId);       
  Wire.write(vfdOutLed);
  Wire.write(_ledStateRegister);
  Wire.endTransmission();
  init();
}

void Vfd::sendMessage(char* text, byte length) {
  //Serial.println(sizeof(text));
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
  ch-=32; // Transform ASCII code to array index
  Wire.write(chars[ch][0]);
  Wire.write(chars[ch][1]);
  Wire.write(chars[ch][2]);
}