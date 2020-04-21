/*
||
|| @file Keypad_MC17.cpp
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

#include "Keypad_MC17.h"

#define GPIOA 0x12		//MCP23017 GPIO reg
#define GPIOB 0x13		//MCP23017 GPIO reg
#define IODIRA 0x00		//MCP23017 I/O direction register
#define IODIRB 0x01		//MCP23017 I/O direction register
#define IOCON 0x0a		//MCP23017 I/O configuration register
#define GPPUA 0x0c		//MCP23017 pullup resistors control

/////// Extended Keypad library functions. ////////////////////////////


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_MC17::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
	_begin( );
	pinState = pinState_set( );
}

// Initialize MC17
void Keypad_MC17::begin(void) {
	_begin( );
	pinState = pinState_set( );
}

word iodirec = 0xffff;            //direction of each bit - reset state = all inputs.
byte iocon = 0x10;                // reset state for bank, disable slew control

// Initialize MCP23017
// MCP23017 byte registers act in word pairs

void Keypad_MC17::_begin( void ) {
	iodir_state = iodirec;
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IOCON ); // same as when reset
	_wire->write( iocon );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPPUA ); // enable pullups on all inputs
	_wire->write( 0xff );
	_wire->write( 0xff );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IODIRA ); // setup port direction - all inputs to start
	_wire->write( lowByte( iodirec ) );
	_wire->write( highByte( iodirec ) );
	_wire->endTransmission( );
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( GPIOA );	//point register pointer to gpio reg
	_wire->write( lowByte(iodirec) ); // make o/p latch agree with pulled-up pins
	_wire->write( highByte(iodirec) );
	_wire->endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void Keypad_MC17::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	_wire->beginTransmission((int)i2caddr);
	_wire->write( IODIRA );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
} // pin_mode( )

void Keypad_MC17::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MC17xWrite( )


int Keypad_MC17::pin_read(byte pinNum) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->endTransmission( );
	word mask = 0x1<<pinNum;
	_wire->requestFrom((int)i2caddr, 2);
	word pinVal = 0;
	pinVal = _wire->read( );
	pinVal |= ( _wire->read( )<<8 );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MC17::port_write( word i2cportval ) {
// MCP23017 requires a register address on each write
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->write( lowByte( i2cportval ) );
	_wire->write( highByte( i2cportval ) );
	_wire->endTransmission();
	pinState = i2cportval;
} // port_write( )

word Keypad_MC17::pinState_set( ) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( GPIOA );
	_wire->endTransmission( );
	_wire->requestFrom( (int)i2caddr, 2 );
	pinState = 0;
	pinState = _wire->read( );
	pinState |= (_wire->read( )<<8);
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word Keypad_MC17::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MC17::iodir_write( word iodir ) {
	iodir_state = iodir;
	_wire->beginTransmission((int)i2caddr);   // read current IODIR reg 
	_wire->write( IODIRA );
	_wire->write( lowByte( iodir_state ) );
	_wire->write( highByte( iodir_state ) );
	_wire->endTransmission();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 2.0 2020-04-05 - Joe Young : MKRZERO compile error, Wire spec'd in Constructor
|| | 1.0 2014-05-18 - Joe Young : Derived from Keypad_MC16
|| #
*/
