/*
||
|| @file Keypad_MCP.h
|| @version 1.0
|| @author G. D. (Joe) Young
|| @contact "G. D. (Joe) Young" <gdyoung@telus.net>
||
|| @description
|| | Keypad_MCP provides an interface for using matrix keypads that
|| | are attached with Microchip MCP23008 I2C port expanders. It 
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

#include "Keypad_MCP.h"

#define IODIR 0x00		//MCP23008 I/O direction register
#define IOCON 0x05		//MCP23008 I/O configuration register
#define GPPU 0x06		//MCP23008 pin pullup resistor reg 1==>enable pullup
#define GPIO 0x09		//MCP23008 GPIO reg

/////// Extended Keypad library functions. ////////////////////////////


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_MCP::begin(char *userKeymap) {
	TwoWire::begin();
    Keypad::begin(userKeymap);
	_begin( );
	pinState = pinState_set( );
}


/////// Extended Keypad_MCP library functions. ////////////////////////

// Initialize MCP
void Keypad_MCP::begin(void) {
	TwoWire::begin();
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

// Initialize MCP
void Keypad_MCP::begin(byte address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
}

void Keypad_MCP::begin(int address) {
	i2caddr = address;
	TwoWire::begin(address);
	_begin( );
//	pinState = 0xff;
	pinState = pinState_set( );
} // begin( int )

byte iodirec = 0xff;            //direction of each bit - reset state = all inputs.
byte MCPconfig[11] = { IODIR, iodirec, 0, 0, 0, 0, 0x00, iodirec, 0, 0, 0xff };

// Initialize MCP23008
// MCP23008 has set of 10 registers to configure before it can
//  act like the simple PCF port
void Keypad_MCP::_begin( void ) {
	iodir_state = iodirec;
	MCPconfig[1] = iodirec;		// direction register
	MCPconfig[7] = iodirec;		// pullup resistors register--pullup inputs
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IOCON ); // turn on register address incrementing
	TwoWire::write( 0x00 );
	TwoWire::endTransmission( );
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( MCPconfig, 11 ); // setup port--leave adr incr ON
	TwoWire::endTransmission( );
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( IOCON ); // turn OFF register address incrementing
	TwoWire::write( 0x20 );
	TwoWire::endTransmission( );
	TwoWire::beginTransmission( (int)i2caddr );
	TwoWire::write( GPIO );	//point register pointer to gpio reg
	TwoWire::endTransmission( );
} // _begin( )

// individual pin setup - modify pin bit in IODIR reg. Don't change pullups
void Keypad_MCP::pin_mode(byte pinNum, byte mode) {
	byte mask = 0b00000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {     // here, could check for pullup ON/OFF - omitted for now
		iodir_state |= mask;
	} // if mode
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( IODIR );
	TwoWire::write( iodir_state );
	TwoWire::endTransmission();
	TwoWire::beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	TwoWire::write( GPIO );
	TwoWire::endTransmission();
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
	TwoWire::requestFrom((int)i2caddr, 1);
	byte pinVal = TwoWire::read( );
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MCP::port_write( byte i2cportval ) {
// MCP23008 requires a register address on each write
	TwoWire::beginTransmission((int)i2caddr);
	TwoWire::write( GPIO );
	TwoWire::write( i2cportval );
	TwoWire::endTransmission();
	pinState = i2cportval;
} // port_write( )

byte Keypad_MCP::pinState_set( ) {
	TwoWire::requestFrom( (int)i2caddr, 1 );
	pinState = TwoWire::read( );
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
byte Keypad_MCP::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MCP::iodir_write( byte iodir ) {
	iodir_state = iodir;
	TwoWire::beginTransmission((int)i2caddr);   // read current IODIR reg 
	TwoWire::write( IODIR );
	TwoWire::write( iodir_state );
	TwoWire::endTransmission();
	TwoWire::beginTransmission((int)i2caddr);	// reset adr pointer to gpio
	TwoWire::write( GPIO );
	TwoWire::endTransmission();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 1.0 2012-07-29 - Joe Young : Initial Release
|| #
*/
