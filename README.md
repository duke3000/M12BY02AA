M12BY02AA
=========

 I2C driver for Futaba M12BY02AA vacuum fluorescent display
 
 Author: Simon Schori <simon.schori@gmail.com>
 Version: 0.3
 
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
  - ...
