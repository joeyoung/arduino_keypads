/*
||
|| @file Keypad_MC17.h
|| @version 2.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_MC17 provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23017 I2C port expanders. It 
|| | supports multiple keypads, user selectable pins, and user
|| | defined keymaps.
|| | The MCP23017 is somewhat similar to the MCP23016 which is supported
|| | by the earlier library Keypad_MC16. The difference most useful for
|| | use with Keypad is the provision of internal pullup resistors on the
|| | pins used as inputs, eliminating the need to provide 16 external 
|| | resistors. The 23017 also has more comprehensive support for separate
|| | 8-bit ports instead of a single 16-bit port. However, this library
|| | assumes configuration as 16-bit port--IOCON.BANK = 0.
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

#ifndef KEYPAD_MC17_H
#define KEYPAD_MC17_H

#include "Keypad.h"
//#include "../Wire/Wire.h"
#include "Wire.h"

class Keypad_MC17 : public Keypad {
public:
	Keypad_MC17(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address, TwoWire *_awire=&Wire ) :
		Keypad(userKeymap, row, col, numRows, numCols) { i2caddr = address; _wire=_awire; }

	// Keypad function
	void begin( char *userKeymap );
	void begin( );

//	void beginnewwire( TwoWire * );

	void pin_mode(byte pinNum, byte mode);
	void pin_write(byte pinNum, boolean level);
	int  pin_read(byte pinNum);
	// read initial value for pinState
	word pinState_set( );
	// write a whole word to i2c port
	void port_write( word i2cportval );
	// access functions for IODIR state copy
	word iodir_read( );
	void iodir_write( word iodir );

private:
    // I2C device address
    byte i2caddr;
	TwoWire *_wire;
	// I2C pin_write state persistant storage
	word pinState;
//	byte pin_iosetup( );
	// MC17 setup
	word iodir_state;    // copy of IODIR register
	void _begin( void );
};


#endif // KEYPAD_MC17_H

/*
|| @changelog
|| |
|| | 2.0 2020-04-05 - Joe Young : MKRZERO compile error, Wire spec'd in Constructor
|| | 1.0 2014-05-18 - Joe Young : Convert from Keypad_MC16
|| #
*/
