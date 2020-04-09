/* @file HelloKeypad.ino

   * Revised to use I2C i/o Feb 26/12 - G. D. Young
   * Re-check with Keypad 3.0, arduino 1.0.1 Jul 29/12
   * Add port-type parameter to constructor for Paul
     Williamson's modification of the Keypad_I2C library Sept 2/13
   * Use Keypad_I2Ca support for PCA9554, PCA9555 port chips GDY 14-5-23
     HelloKeypad_I2Ca set to use PCA9555 with keypad on IO1 bits,
     Sparkfun keypad.
   * Revised to illustrate optional alternate wire port. Note that the 'width'
     optional parameter must be included if the optional wire is used. OK to
     omit both for 8-bit i/o expanders on Wire. 

|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad_I2Ca.h>
#include <Keypad.h> 
#include <Wire.h>
#define I2CADDR 0x77

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {13, 8, 9, 11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {12, 14, 10}; //connect to the column pinouts of the keypad
TwoWire* jwire = &Wire;
Keypad_I2Ca kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCA9555, jwire );

void setup(){
  Serial.begin(9600);
  while( !Serial ){/*wait*/}
//  Wire.begin( );
  jwire->begin( );
//  kpd.begin( makeKeymap(keys) );
  Serial.println( "start" );
  Serial.println( kpd.pinState_set( ), HEX );
  Serial.println( kpd.iodir_read( ), HEX );
  kpd.iodir_write( 0xa5a5 );
  Serial.println( kpd.iodir_read( ), HEX );
  kpd.iodir_write( 0x7fff );
  kpd.port_write( kpd.pinState_set( ) & 0x7fff );
}
void loop(){
  
  char key = kpd.getKey();
  
  if (key){
    Serial.println(key);
  }
}
