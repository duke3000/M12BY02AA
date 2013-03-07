/*
 M12BY02AA.h - I2C driver for Futaba M12BY02AA vacuum fluorescent display
 
 Author: Simon Schori <simon.schori@gmail.com>
 Version: 0.2
 
 Datasheet: http://docs-asia.electrocomponents.com/webdocs/0f06/0900766b80f06185.pdf
 
 Addressing of the display is done by hardwiring A0 through A2 (Pins 4-7) to GND.
 The pins are internally pulled-up, so leaving them open means address 7. 
*/

#ifndef M12BY02AA_h
#define M12BY02AA_h

#define ledYellowBit 0
#define ledGreenBit 1
#define ledRedBit 2
#define ledGreenOn B01000011
#define ledYellowOn B01000101
#define ledRedOn B01000110
#define ledAllOn B01000000
#define ledAllOff B01000111
#define vfdIdBase 80
#define vfdI2cId B10101110
#define vfdOutLed B01000101
#define vfdOutRam B01000100
#define vfdOutRamAutoIncrement B01000000
#define ramStartAddr B11000000

class Vfd {
  public:
    Vfd(byte addr);
	void init();
	void clear();
	void ledControl (char color, boolean enabled);
	void sendMessage(char* text, byte length);
	int brightness;

  private:
    byte _addr;
	byte _i2cAddr;
	void sendChar(char ch);
	void dataStart(byte offset);
	void dataStop();
	byte _ledStateRegister;
	//byte* _chars[95][3];
};

#endif