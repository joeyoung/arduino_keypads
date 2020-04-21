/*
||
|| @file Keypad_I2Ca.cpp
|| @version 4.0 - fix compile error, allow WireX in constructor
|| @version 3.0 - PCA9554, PCA9554A, PCA9555 support G. D. Young
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <jyoung@islandnet.com>
||
|| @description
|| | Keypad_I2Ca provides an interface for using matrix keypads that
|| | are attached with I2C totem-pole output port expanders. It supports
|| | multiple keypads, user selectable pins, and user defined keymaps.
|| #
||
|| @version 4.0 - April 5, 2020
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

#include "Keypad_I2Ca.h"

#define IREG 0x00	// input port location
#define OREG 0x01	// output port
#define VREG 0x02	// polarity inversion (not used, but configured)
#define CREG 0x03	// configuration reg - IODIR
#define IODIR 0x03	// common alias for config

// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_I2Ca::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
//	_wire->begin();
	_begin( );
}


// Initialize I2C
void Keypad_I2Ca::begin(void) {
//	_wire->begin();
	_begin( );
}

#if 0
// Initialize I2C
void Keypad_I2Ca::begin(byte address) {
	i2caddr = address;
	_wire->begin(address);
	_begin( );
}


// Initialize I2C
void Keypad_I2Ca::begin(int address) {
	i2caddr = address;
	_wire->begin(address);
	_begin( );
}
#endif

// configure port registers as if just power-on reset
void Keypad_I2Ca::_begin( ) {
	p_write( 0xffff, OREG );
	p_write( 0x0000, VREG );
	p_write( 0xffff, IODIR );
	iodir_state = 0xffff;
	pinState = pinState_set( );
} // _begin( )


void Keypad_I2Ca::pin_mode(byte pinNum, byte mode) {
	word mask = 1<<pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	}
	p_write( iodir_state, IODIR );
} // pin_mode( )


void Keypad_I2Ca::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	p_write( pinState, OREG );
} // I2CxWrite( )


int Keypad_I2Ca::pin_read(byte pinNum) {
	word mask = 0x1<<pinNum;
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IREG );
	_wire->endTransmission( );
	_wire->requestFrom((int)i2caddr, (int)i2cwidth);
	word pinVal = _wire->read( );
	if (i2cwidth > 1) {
		pinVal |= _wire->read( ) << 8;
	} 
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
} // pin_read( )

void Keypad_I2Ca::port_write( word i2cportval ) {
	p_write( i2cportval, OREG );
} // port_write( ) - public access


void Keypad_I2Ca::p_write( word i2cportval, byte reg ) {
	_wire->beginTransmission((int)i2caddr);
	_wire->write( reg<<(i2cwidth-1) );			//twice as many regs for 9555
	_wire->write( i2cportval & 0x00FF );
	if (i2cwidth > 1) {
		_wire->write( i2cportval >> 8 );
	}
	_wire->endTransmission();
//	if( reg == OREG) pinState = i2cportval;		//not quite right - re-read i/p??
	if( reg == OREG) pinState = pinState_set( );
} // p_write( ) - private


word Keypad_I2Ca::pinState_set( ) {
	_wire->beginTransmission( (int)i2caddr );
	_wire->write( IREG );
	_wire->endTransmission( );
	_wire->requestFrom( (int)i2caddr, (int)i2cwidth );
	pinState = _wire->read( );
	if (i2cwidth > 1) {
		pinState |= _wire->read( ) << 8;
	}
	return pinState;
} // set_pinState( )


word Keypad_I2Ca::iodir_read( ) {
//	_wire->requestFrom( (int)i2caddr, (int)i2cwidth );
//	iodir_state = _wire->read( );
//	if( i2cwidth > 1 ) {
//		iodir_state |= _wire->read( ) << 8;
//	}
	return iodir_state;
} // iodir_read( )


void Keypad_I2Ca::iodir_write( word iodir ) {
	p_write( iodir, IODIR );
	iodir_state = iodir;
} // iodir_write( )

/*
|| @changelog
|| |
|| | 4.0 2020-04-06 - GDY fix compile error, allow WireX in constructor
|| |
|| | 3.0 2014-05-22 - GDY support totem-pole output ports PCA9554(8-bit), PCA9555(16-bit)
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
