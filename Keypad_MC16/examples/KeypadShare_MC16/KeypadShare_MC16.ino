/* keypad with indicator pin on I2C bus
 *
 * created: July 6, 2012 G. D. Young
 *
 * revised: Feb 3, 2013 - modified for MCP2016 16-bit port chip
 *
 * Illustrate sharing MCP23016 I2C port chip with 4X3 keypad and
 * one bit i/o
 */

#include <Wire.h>
#include <Keypad_MC16.h>
#include <Keypad.h>

#define I2CADDR 0x26

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
	};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad
const word ledPin = 0x0080;  //assign one 'unused' I2C port pin to led indicator
	
Keypad_MC16 kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

boolean blink = false;
unsigned long blinktimer;

word portState;   // copy of I2C port word

void setup( ){
  Serial.begin( 9600 );
  Wire.begin( );
  kpd.begin( );  // also starts Wire
  kpd.iodir_write( kpd.iodir_read( ) & 0xff7f );  // make bit an output
  portState = kpd.pinState_set( );
  portState |= ledPin;    // set led pin
  kpd.port_write( portState );
  kpd.addEventListener( keypadEvent ); //add an event listener for this keypad
  blinktimer = millis( );
}

bool togg = true;
void toggleLEDpin( void ) {
  portState = kpd.pinState_set( );
  if( togg ) {
    portState &= ~ledPin;
    togg = false;
  } else {
    portState |= ledPin;
    togg = true;
  }
  kpd.port_write( portState );
} // toggleLEDpin( )

  
void loop(){
  char key = kpd.getKey( );
  
  if( key ) {
    Serial.println( key );
  }
  if( millis( ) - blinktimer > 100 ) {
    blinktimer += 100;
    if( blink ) {
      toggleLEDpin( );
    } // if blink
  } // if time up
}// loop

//take care of some special events
void keypadEvent( KeypadEvent key ){
  switch( kpd.getState( ) ) {
    case PRESSED:
      switch( key ) {
        case '#': toggleLEDpin( ); break;
        case '*': 
          toggleLEDpin( );
        break;
      }
    break;
    case RELEASED:
      switch( key ){
        case '*': 
          toggleLEDpin( );
          blink = false;
        break;
      }
    break;
    case HOLD:
      switch( key ){
        case '*': blink = true; break;
      }
    break;
  }
}
