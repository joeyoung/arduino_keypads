/*
||
|| @file Keypad_MCP.h
|| @version 2.0
|| @version 1.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_MCP provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23008 I2C port expanders. It 
|| | supports multiple keypads, user selectable pins, and user
|| | defined keymaps.
|| #
||
|| @version 2.0 - April 5, 2020
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
|| | 2.1 of the License or any later version.
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

#ifndef KEYPAD_MCP_H
#define KEYPAD_MCP_H

#include "Keypad.h"
//#include "../Wire/Wire.h"
#include "Wire.h"

class Keypad_MCP : public Keypad {
public:
	Keypad_MCP(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols,
               byte address, TwoWire * awire=&Wire ) 
		       : Keypad(userKeymap, row, col, numRows, numCols) { i2caddr = address; _wire = awire; }

	// Keypad function
	void begin(char *userKeymap);
	// Wire function
	void begin(void);
	// Wire function

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	byte pinState_set( );
	// write a whole byte to i2c port
	void port_write( byte i2cportval );
	// access functions for IODIR state copy
	byte iodir_read( );
	void iodir_write( byte iodir );

private:
    // I2C device address
    byte i2caddr;
	// I2C pin_write state persistant storage
	byte pinState;
	byte pin_iosetup( );
	// MCP setup
	byte iodir_state;    // copy of IODIR register
	void _begin( void );
	TwoWire * _wire;
};


#endif // KEYPAD_MCP_H

/*
|| @changelog
|| |
|| | 2.0 2020-04-10 - Joe Young : fix compile err on MKR from importing TwoWire in constructor
|| |                              add optional specification of Wire port for newer boards
|| | 1.0 2012-07-29 - Joe Young : Initial Release
|| #
*/

