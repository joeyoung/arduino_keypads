/*
||
|| @file Keypad_MCP.cpp
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

#include "Keypad_MCP.h"

#define IODIR 0x00		//MCP23008 I/O direction register
#define IOCON 0x05		//MCP23008 I/O configuration register
#define GPPU 0x06		//MCP23008 pin pullup resistor reg 1==>enable pullup
#define GPIO 0x09		//MCP23008 GPIO reg

/////// Extended Keypad library functions. ////////////////////////////


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_MCP::begin(char *userKeymap) {
//	_wire->begin();
    Keypad::begin(userKeymap);
	_begin( );
	pinState = pinState_set( );
}


/////// Extended Keypad_MCP library functions. ////////////////////////

// Initialize MCP
void Keypad_MCP::begin(void) {
//	_wire->begin();
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

#if 0
// Initialize MCP
void Keypad_MCP::begin(byte address) {
	i2caddr = address;
	_wire->begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

void Keypad_MCP::begin(int address) {
	i2caddr = address;
	_wire->begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
} // begin( int )
#endif

byte iodirec = 0xff;            //direction of each bit - reset state = all inputs.
byte MCPconfig[11] = { IODIR, iodirec, 0, 0, 0, 0, 0x00, iodirec, 0, 0, 0xff };

// Initialize MCP23008
// MCP23008 has set of 10 registers to configure before it can
//  act like the simple PCF port
void Keypad_MCP::_begin( void ) {
	iodir_state = iodirec;
	MCPconfig[1] = iodirec;		// direction register
	MCPconfig[7] = iodirec;		// pullup resistors register--pullup inputs
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // turn on register address incrementing
	_wire->write( 0x00 );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( MCPconfig, 11 ); // setup port--leave adr incr ON
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // turn OFF register address incrementing
	_wire->write( 0x20 );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPIO );	//point register pointer to gpio reg
	_wire->endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg. Don't change pullups
void Keypad_MCP::pin_mode(byte pinNum, byte mode) {
	byte mask = 0b00000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {     // here, could check for pullup ON/OFF - omitted for now
		iodir_state |= mask;
	} // if mode
	_wire->beginTransmission((int)i2caddr);
	_wire->write( IODIR );
	_wire->write( iodir_state );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GPIO );
	_wire->endTransmission();
} // pin_mode( )

void Keypad_MCP::pin_write(byte pinNum, boolean level) {
//	static byte pinState;  // allocated in constructor GDY120704
	byte mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MCPxWrite( )


int Keypad_MCP::pin_read(byte pinNum) {
	byte mask = 0x1<<pinNum;
	_wire->requestFrom((int)i2caddr, 1);
	byte pinVal = _wire->read( );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MCP::port_write( byte i2cportval ) {
// MCP23008 requires a register address on each write
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIO );
	_wire->write( i2cportval );
	_wire->endTransmission();
	pinState = i2cportval;
} // port_write( )

byte Keypad_MCP::pinState_set( ) {
	_wire->requestFrom( (int)i2caddr, 1 );
	pinState = _wire->read( );
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
byte Keypad_MCP::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MCP::iodir_write( byte iodir ) {
	iodir_state = iodir;
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( IODIR );
	_wire->write( iodir_state );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GPIO );
	_wire->endTransmission();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 2.0 2020-04-10 - Joe Young : Fix compile err, add optional Wire param in constructor
|| |
|| | 1.0 2012-07-29 - Joe Young : Initial Release
|| #
*/
