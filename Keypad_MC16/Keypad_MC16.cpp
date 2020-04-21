/*
||
|| @file Keypad_MC16.cpp
|| @version 2.0
|| @version 1.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_MC16 provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23016 I2C port expanders. It 
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

#include "Keypad_MC16.h"

#define GP0 0x00		//MCP23016 GPIO reg
#define GP1 0x01		//MCP23016 GPIO reg
#define IODIR0 0x06		//MCP23016 I/O direction register
#define IODIR1 0x07		//MCP23016 I/O direction register
#define IOCON0 0x0a		//MCP23016 I/O configuration register
#define IOCON1 0x0b		//MCP23016 I/O configuration register

/////// Extended Keypad library functions. ////////////////////////////


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_MC16::begin(char *userKeymap) {
//	_wire->begin();
    Keypad::begin(userKeymap);
	_begin( );
	pinState = pinState_set( );
}


/////// Extended Keypad_MC16 library functions. ////////////////////////

// Initialize MC16
void Keypad_MC16::begin(void) {
//	_wire->begin();
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}


word iodirec = 0xffff;            //direction of each bit - reset state = all inputs.
byte iocon = 0x01;                // fast mode

// Initialize MCP23016
// MCP23016 byte registers act in word pairs

void Keypad_MC16::_begin( void ) {
	iodir_state = iodirec;
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON0 ); // set fast mode
	_wire->write( iocon );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IODIR0 ); // setup port direction - all inputs to start
	_wire->write( lowByte( iodirec ) );
	_wire->write( highByte( iodirec ) );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GP0 );	//point register pointer to gpio reg
	_wire->write( lowByte(iodirec) ); // make o/p latch agree with pulled-up pins
	_wire->write( highByte(iodirec) );
	_wire->endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void Keypad_MC16::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	_wire->beginTransmission((int)i2caddr);
	_wire->write( IODIR0 );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GP0 );
	_wire->endTransmission();
} // pin_mode( )

void Keypad_MC16::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MC16xWrite( )


int Keypad_MC16::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	_wire->requestFrom((int)i2caddr, 2);
	word pinVal = _wire->read( );
	pinVal |= ( _wire->read( )<<8 );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MC16::port_write( word i2cportval ) {
// MCP23016 requires a register address on each write
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GP0 );
	_wire->write( lowByte( i2cportval ) );
	_wire->write( highByte( i2cportval ) );
	_wire->endTransmission();
	pinState = i2cportval;
} // port_write( )

word Keypad_MC16::pinState_set( ) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GP0 );
	_wire->endTransmission( );
	_wire->requestFrom( (int)i2caddr, 2 );
	pinState = 0;
	pinState = _wire->read( );
	pinState |= (_wire->read( )<<8);
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word Keypad_MC16::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MC16::iodir_write( word iodir ) {
	iodir_state = iodir;
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( IODIR0 );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
	_wire->beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	_wire->write( GP0 );
	_wire->endTransmission();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 2.0 2020-04-11 - Joe Young : Fix MKR compile err, add optional Wire param in constructor
|| |
|| | 1.0 2013-01-25 - Joe Young : Derived from Keypad_MCP
|| #
*/
