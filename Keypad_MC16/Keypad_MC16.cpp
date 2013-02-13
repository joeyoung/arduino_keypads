/*
||
|| @file Keypad_MC16.h
|| @version 1.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
||
|| @description
|| | Keypad_MC16 provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23016 I2C port expanders. It 
|| | supports multiple keypads, user selectable pins, and user
|| | defined keymaps.
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
	TwoWire::begin();
    Keypad::begin(userKeymap);
	_begin( );
	pinState = pinState_set( );
}


/////// Extended Keypad_MC16 library functions. ////////////////////////

// Initialize MC16
void Keypad_MC16::begin(void) {
	TwoWire::begin();
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize MC16
void Keypad_MC16::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

void Keypad_MC16::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
} // begin( int )

word iodirec = 0xffff;            //direction of each bit - reset state = all inputs.
byte iocon = 0x01;                // fast mode

// Initialize MCP23016
// MCP23016 byte registers act in word pairs

void Keypad_MC16::_begin( void ) {
	iodir_state = iodirec;
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IOCON0 ); // set fast mode
	TwoWire::write( iocon );
	TwoWire::endTransmission( );
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IODIR0 ); // setup port direction - all inputs to start
	TwoWire::write( lowByte( iodirec ) );
	TwoWire::write( highByte( iodirec ) );
	TwoWire::endTransmission( );
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( GP0 );	//point register pointer to gpio reg
	TwoWire::write( lowByte(iodirec) ); // make o/p latch agree with pulled-up pins
	TwoWire::write( highByte(iodirec) );
	TwoWire::endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void Keypad_MC16::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( IODIR0 );
	TwoWire::write( lowByte( iodir_state ) );
	TwoWire::write( highByte( iodir_state ) );
	TwoWire::endTransmission();
	TwoWire::beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	TwoWire::write( GP0 );
	TwoWire::endTransmission();
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
	TwoWire::requestFrom((int)i2caddr, 2);
	word pinVal = TwoWire::read( );
	pinVal |= ( TwoWire::read( )<<8 );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MC16::port_write( word i2cportval ) {
// MCP23016 requires a register address on each write
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( GP0 );
	TwoWire::write( lowByte( i2cportval ) );
	TwoWire::write( highByte( i2cportval ) );
	TwoWire::endTransmission();
	pinState = i2cportval;
} // port_write( )

word Keypad_MC16::pinState_set( ) {
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( GP0 );
	TwoWire::endTransmission( );
	TwoWire::requestFrom( (int)i2caddr, 2 );
	pinState = 0;
	pinState = TwoWire::read( );
	pinState |= (TwoWire::read( )<<8);
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word Keypad_MC16::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MC16::iodir_write( word iodir ) {
	iodir_state = iodir;
	TwoWire::beginTransmission((int)i2caddr);   // read current IODIR reg 
	TwoWire::write( IODIR0 );
	TwoWire::write( lowByte( iodir_state ) );
	TwoWire::write( highByte( iodir_state ) );
	TwoWire::endTransmission();
	TwoWire::beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	TwoWire::write( GP0 );
	TwoWire::endTransmission();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 1.0 2013-01-25 - Joe Young : Derived from Keypad_MCP
|| #
*/
