/*
||
|| @file Keypad_MCS17.cpp
|| @version 1.2
|| @author G. D. (Joe) Young  (with input from Alyx Vance for SPI version - issue #18
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
|| | resistors. The 23S17 also has more comprehensive support for separate
|| | 8-bit ports instead of a single 16-bit port. However, this library
|| | assumes configuration as 16-bit port--IOCON.BANK = 0.
|| #
||
|| @version SPI 1.2 January 14, 2025
|| | Keypad_MCS17 revised to allow for 8-bit and 16-bit versions of SPI
|| | port expanders (MCP23S08, MCP23S17, MCP23S18)
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

#include "Keypad_MCS17.h"

#define GPIO 0x09		//MCP23S08 GPIO reg
#define IODIR 0x00		//MCP23S08 direction control reg
#define IOCON 0x05		//MCP23SO8 configuration reg
#define GPPU 0x06		//MCP23S08 pullup resistors control

#if 0					//don't use 2-byte addresses
#define GPIOA 0x12		//MCP23S17 GPIO reg
#define GPIOB 0x13		//MCP23S17 GPIO reg
#define IODIRA 0x00		//MCP23S17 I/O direction register
#define IODIRB 0x01		//MCP23S17 I/O direction register
#define IOCON 0x0a		//MCP23S17 I/O configuration register
#define GPPUA 0x0c		//MCP23S17 pullup resistors control
#endif

//Note that the MCP23S17 register addresses align with 2X the MCP23S08 addresses


/////// Extended Keypad library functions. ////////////////////////////

void Keypad_MCS17::kpdSettings( SPISettings settings ){
	kpd_settings = settings;
}


// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad_MCS17::begin(char *userKeymap) {
    Keypad::begin(userKeymap);
	_begin( );
}

// Initialize MCS17
void Keypad_MCS17::begin(void) {
	_begin( );
}

word iodirec = 0xffff;            //direction of each bit - reset state = all inputs.

byte iocon = 0x18;                //bank=0, disable slew control, enable hardware addressing

// Initialize MCP23S17
// MCP23S17 byte registers act in word pairs

void Keypad_MCS17::_begin( void ) {

	kpd_settings = SPISettings(10000000, MSBFIRST, SPI_MODE0);
	pinMode( cs_port, OUTPUT );
//	if( width == 1 ) iocon = iocon | 0x20;		//turn off sequential addressing

//turn on hardware addressing of MCP23S17 - use pins 15,16,17 to select chip
// - pins 4, 5 of MCP23S08
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( IOCON<<(width-1) );			//address of control reg
	_spi->transfer( iocon );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction( );

//set initial port direction register for all inputs, enable pullups
	iodir_state = iodirec;
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( GPPU<<(width-1) ); // enable pullups on all inputs
	_spi->transfer( 0xff );
	if(width>1)_spi->transfer( 0xff );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction( );

	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( IODIR<<(width-1) ); // setup port direction - all inputs to start
	_spi->transfer( lowByte( iodirec ) );
	if(width>1)_spi->transfer( highByte( iodirec ) );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction( );

	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( GPIO<<(width-1) );	//point register pointer to gpio reg
	_spi->transfer( lowByte(iodirec) ); // make o/p latch agree with pulled-up pins
	if(width>1)_spi->transfer( highByte(iodirec) );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction( );

	pinState = iodirec;

} // _begin( )

// individual pin setup - modify pin bit in IODIR reg.
void Keypad_MCS17::pin_mode(byte pinNum, byte mode) {
	word mask = 0b0000000000000001 << pinNum;
	if( mode == OUTPUT ) {
		iodir_state &= ~mask;
	} else {
		iodir_state |= mask;
	} // if mode
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( IODIR<<(width-1) );
	_spi->transfer( lowByte( iodir_state ) );
	if(width>1)_spi->transfer( highByte( iodir_state ) );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction();
} // pin_mode( )

void Keypad_MCS17::pin_write(byte pinNum, boolean level) {
	word mask = 1<<pinNum;
	if( level == HIGH ) {
		pinState |= mask;
	} else {
		pinState &= ~mask;
	}
	port_write( pinState );
} // MC17xWrite( )


int Keypad_MCS17::pin_read(byte pinNum) {
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x41 | ( spichip<<1 ) );	//device opcode (read)
	_spi->transfer( GPIO<<(width-1) );
	word mask = 0x1<<pinNum;
	word pinVal = 0;
	pinVal = _spi->transfer( GPIO<<(width-1) );
	if(width>1)pinVal |= ( _spi->transfer( GPIO<<(width-1) )<<8 );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction();
	pinVal &= mask;
	if( pinVal == mask ) {
		return 1;
	} else {
		return 0;
	}
}

void Keypad_MCS17::port_write( word i2cportval ) {
// MCP23S17 requires a register address on each write
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( GPIO<<(width-1) );
	_spi->transfer( lowByte( i2cportval ) );
	if(width>1)_spi->transfer( highByte( i2cportval ) );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction();
	pinState = i2cportval;
} // port_write( )

word Keypad_MCS17::pinState_set( ) {
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x41 | ( spichip<<1 ) );	//device opcode (read)
	_spi->transfer( GPIO<<(width-1) );
	pinState = 0;
	pinState = _spi->transfer( GPIO<<(width-1) );
	if(width>1)pinState |= ( _spi->transfer( GPIO<<(width-1) )<<8 );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction();
	return pinState;
} // set_pinState( )

// access functions for IODIR state copy
word Keypad_MCS17::iodir_read( ) {
	return iodir_state;  // local copy is always same as chip's register
} // iodir_read( )

void Keypad_MCS17::iodir_write( word iodir ) {
	iodir_state = iodir;
	_spi->beginTransaction(kpd_settings);
	digitalWrite( cs_port, LOW );
	_spi->transfer( 0x40 | ( spichip<<1 ) );	//device opcode
	_spi->transfer( IODIR<<(width-1) );
	_spi->transfer( lowByte( iodir_state ) );
	if(width>1)_spi->transfer( highByte( iodir_state ) );
	digitalWrite( cs_port, HIGH );
	_spi->endTransaction();
} // iodir_write( )


/*
|| @changelog
|| |
|| | 1.2 2025-01-14 - Joe Young : allow for 8-bit MCP23S08, width parameter
|| | 1.1 2025-01-08 - Joe Young : SPI MCP23S17 version plus kpdSettings( )
|| | 2.0 2020-04-05 - Joe Young : MKRZERO compile error, Wire spec'd in Constructor
|| | 1.0 2014-05-18 - Joe Young : Derived from Keypad_MC16
|| #
*/
