/*
||
|| @file Keypad_I2C.h
|| @version 1.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
||
|| @description
|| | Keypad_I2C provides an interface for using matrix keypads that
|| | are attached with I2C port expanders. It supports multiple keypads,
|| | user selectable pins, and user defined keymaps.
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

#include "Keypad_I2C.h"

// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_I2C::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
	TwoWire::begin();
	pinState = pinState_set( );
}


// Initialize I2C
void Keypad_I2C::begin(void) {
	TwoWire::begin();
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize I2C
void Keypad_I2C::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize I2C
void Keypad_I2C::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
//	pinState = 0xff;
	pinState = pinState_set( );
}


void Keypad_I2C::pin_write(byte pinNum, boolean level) {
	byte mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // I2CxWrite( )


int Keypad_I2C::pin_read(byte pinNum) {
	byte mask = 0x1<<pinNum;
	TwoWire::requestFrom((int)i2caddr, 1);
	byte pinVal = TwoWire::read( );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_I2C::port_write( byte i2cportval ) {
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( i2cportval );
	TwoWire::endTransmission();
	pinState = i2cportval;
} // port_write( )

byte Keypad_I2C::pinState_set( ) {
	TwoWire::requestFrom( (int)i2caddr, 1 );
	pinState = TwoWire::read( );
} // set_pinState( )


/*
|| @changelog
|| |
|| | 1.0 2012-07-12 - Joe Young : Initial Release
|| #
*/
