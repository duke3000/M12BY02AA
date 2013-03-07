/*
 M12BY02AA_Library_Example.pde - Hello World for Futaba M12BY02AA vacuum fluorescent display library
 
 Author: duke3000
 Version: 0.1
 
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
  vfd.ledControl('r',0); // disable red LED
  vfd.ledControl('y',1); // enable yellow LED
  delay(time);
  vfd.ledControl('y',0); // disable yellow LED
  vfd.ledControl('g',1); // enable green LED
  delay(time);
  vfd.ledControl('g',0); // disable green LED
  vfd.ledControl('r',1); // enable red LED
  delay(time);
}


