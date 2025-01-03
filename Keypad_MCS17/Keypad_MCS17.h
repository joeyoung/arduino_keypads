/*
||
|| @file Keypad_MCS17.h
|| @version 1.0 SPI
|| @author G. D. (Joe) Young / SPI Alyx Vance
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_MCS17 provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23S17 SPI port expanders. It 
|| | supports multiple keypads, user selectable pins, and user
|| | defined keymaps.
|| | The MCP23S17 is somewhat similar to the MCP23016 which is supported
|| | by the earlier library Keypad_MC16. The difference most useful for
|| | use with Keypad is the provision of internal pullup resistors on the
|| | pins used as inputs, eliminating the need to provide 16 external 
|| | resistors. The 23017 also has more comprehensive support for separate
|| | 8-bit ports instead of a single 16-bit port. However, this library
|| | assumes configuration as 16-bit port--IOCON.BANK = 0.
|| #
||
|| @version SPI 1.0 December 13, 2024
|| | Keypad_MCS17 is a SPI version of Keypad_MC17 from G. D. (Joe) Young
|| | SPI changes started by Alyx Vance.
|| #  
||
|| @version 2.0 - April 5, 2020
|| | MKRZERO, ESP32 compile error from inheriting TwoWire that was OK with
|| | original ATMEGA boards; possibly because newer processors can have
|| | multiple I2C WireX ports. Consequently, added the ability to specify
|| | an alternate Wire as optional parameter in constructor.
|| |
|| | Similarly, the SPI version allows for specification of an alternate
|| | SPI handler.
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

#ifndef KEYPAD_MCS17_H
#define KEYPAD_MCS17_H

#include "Keypad.h"
#include "SPI.h"

class Keypad_MCS17 : public Keypad {
public:
	Keypad_MCS17(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address, byte port, SPIClass *_aspi = &SPI ) :
		Keypad(userKeymap, row, col, numRows, numCols) { cs_port = port; spichip = address; _spi = _aspi; }

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
    byte cs_port;
    // I2C device address
    byte spichip;
	SPIClass *_spi;
	// I2C pin_write state persistant storage
	word pinState;
//	byte pin_iosetup( );
	// MCS17 setup
	word iodir_state;    // copy of IODIR register
	void _begin( void );
};


#endif // KEYPAD_MCS17_H

/*
|| @changelog
|| | 1.0 2024-12-29 - Joe Young : set up as complete library package, use arduino SPI commands
|| | 1.0 SPI 2024-12-13 - Alyx Vance : Made as SPI version for MCP23S17
|| | 2.0 2020-04-05 - Joe Young : MKRZERO compile error, Wire spec'd in Constructor
|| | 1.0 2014-05-18 - Joe Young : Convert from Keypad_MC16
|| #
*/
