/*
||
|| @file Keypad_I2Ca.h
|| @version 3.0 - PCA9554, PCA9554A, PCA9555 support G. D. Young
|| @version 2.0 - PCF8575 support added by Paul Williamson
|| @author G. D. (Joe) Young, ptw
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
||
|| @description
|| | Keypad_I2Ca provides an interface for using matrix keypads that
|| | are attached with I2C totem-pole output port expanders. It supports
|| | multiple keypads, user selectable pins, and user defined keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
	TwoWire::begin();
	_begin( );
}


// Initialize I2C
void Keypad_I2Ca::begin(void) {
	TwoWire::begin();
	_begin( );
}


// Initialize I2C
void Keypad_I2Ca::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
}


// Initialize I2C
void Keypad_I2Ca::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
}


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
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IREG );
	TwoWire::endTransmission( );
	TwoWire::requestFrom((int)i2caddr, (int)i2cwidth);
	word pinVal = TwoWire::read( );
	if (i2cwidth > 1) {
		pinVal |= TwoWire::read( ) << 8;
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
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( reg<<(i2cwidth-1) );			//twice as many regs for 9555
	TwoWire::write( i2cportval & 0x00FF );
	if (i2cwidth > 1) {
		TwoWire::write( i2cportval >> 8 );
	}
	TwoWire::endTransmission();
//	if( reg == OREG) pinState = i2cportval;		//not quite right - re-read i/p??
	if( reg == OREG) pinState = pinState_set( );
} // p_write( ) - private


word Keypad_I2Ca::pinState_set( ) {
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IREG );
	TwoWire::endTransmission( );
	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
	pinState = TwoWire::read( );
	if (i2cwidth > 1) {
		pinState |= TwoWire::read( ) << 8;
	}
	return pinState;
} // set_pinState( )


word Keypad_I2Ca::iodir_read( ) {
//	TwoWire::requestFrom( (int)i2caddr, (int)i2cwidth );
//	iodir_state = TwoWire::read( );
//	if( i2cwidth > 1 ) {
//		iodir_state |= TwoWire::read( ) << 8;
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
|| | 3.0 2014-05-22 - GDY support totem-pole output ports PCA9554(8-bit), PCA9555(16-bit)
|| |
|| | 2.0 2013-08-31 - Paul Williamson : Added i2cwidth parameter for PCF8575 support
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
