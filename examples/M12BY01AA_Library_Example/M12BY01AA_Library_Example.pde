/*
 M12BY02AA_Library_Example.pde - Hello World for Futaba M12BY02AA vacuum fluorescent display library
 
 Author: Simon Schori <simon.schori@gmail.com>
 Version: 0.2
 
 Datasheet: http://docs-asia.electrocomponents.com/webdocs/0f06/0900766b80f06185.pdf
 
 Addressing of the display is done by hardwiring A0 through A2 (Pins 4-7) to GND.
 The pins are internally pulled-up, so leaving them open simply means address 7. 
*/

#include <Wire.h>
#include <M12BY02AA.h>

#define time 250

Vfd vfd(7);  //create new Vfd instance with display address 7

void setup() {
  vfd.brightness = 15; // valid brightness values are 0 through 15
  vfd.sendMessage("Hello World!",12); // send a message, 12 characters long
}

void loop() {
  vfd.ledControl(RED,0); // disable red LED
  vfd.ledControl(YELLOW,1); // enable yellow LED
  delay(time);
  vfd.ledControl(YELLOW,0); // disable yellow LED
  vfd.ledControl(GREEN,1); // enable green LED
  delay(time);
  vfd.ledControl(GREEN,0); // disable green LED
  vfd.ledControl(RED,1); // enable red LED
  delay(time);
}


