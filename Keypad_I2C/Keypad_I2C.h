/*
||
|| @file Keypad_I2C.h
|| @version 3.0 - multiple WireX support
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_I2C provides an interface for using matrix keypads that
|| | are attached to I2C port expanders. It supports multiple keypads,
|| | user selectable pins, and user defined keymaps.
|| #
||
|| @version 3.0 - April 5, 2020
|| | MKRZERO, ESP32 compile error from inheriting TwoWire that was OK with
|| | original ATMEGA boards; possibly because newer processors can have
|| | multiple I2C WireX ports. Consequently, added the ability to specify
|| | an alternate Wire as optional parameter in constructor.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License or later versions.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, see
|| | <https://www.gnu.org/licenses/>
|| #
||
*/

#ifndef KEYPAD_I2C_H
#define KEYPAD_I2C_H

#include "Keypad.h"
#include "Wire.h"

#define	PCF8574	1	// PCF8574 I/O expander device is 1 byte wide
#define PCF8575 2	// PCF8575 I/O expander device is 2 bytes wide

class Keypad_I2C : public Keypad {
public:
	Keypad_I2C(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address,
               byte width = 1, TwoWire * awire=&Wire ) 
		: Keypad(userKeymap, row, col, numRows, numCols) { i2caddr=address; i2cwidth=width; _wire=awire; } 
	

	// Keypad function
	void begin(char *userKeymap);
	// Wire function
	void begin(void);
	// Wire function

	void pin_mode(byte pinNum, byte mode) {}
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole byte or word (depending on the port expander chip) to i2c port
	void port_write( word i2cportval );

private:
    // I2C device address
    byte i2caddr;
    // I2C port expander device width in bytes (1 for 8574, 2 for 8575)
    byte i2cwidth;
	// I2C pin_write state persistant storage
	// least significant byte is used for 8-bit port expanders
	word pinState;
	TwoWire *_wire;
};



#endif // KEYPAD_I2C_H

/*
|| @changelog
|| |
|| | 3.0 2020-04-06 - Joe Young : support multiple I2C port WireX objects in constructor
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
