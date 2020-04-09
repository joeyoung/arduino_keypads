/* @file HelloKeypad.ino

   Revised to use I2C i/o Feb 26/12 - G. D. Young
   Re-check with Keypad 3.0, arduino 1.0.1 Jul 29/12
   Add port-type parameter to constructor for Paul
   Williamson's modification of the Keypad_I2C library Sept 2/13

   April 6/2020 - Keypad_I2C allows optional WireX parameter. Note
   that if WireX parameter is spec'd, then the otherwise optional
   'width' parameter is needed. OK to omit both options when using
   8 bit port chips on Wire

|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#define I2CADDR 0x38

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

TwoWire *jwire = &Wire;   //test passing pointer to keypad lib
Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574, jwire );
//Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

void setup(){
  Serial.begin(9600);
  while( !Serial ){ /*wait*/ }
//  Wire.begin( );
  jwire->begin( );
//  kpd.begin( makeKeymap(keys) );
  kpd.begin( );
  Serial.print( "start with pinState = " );
  Serial.println( kpd.pinState_set( ), HEX );
}
void loop(){
  
  char key = kpd.getKey();
  
  if (key){
    Serial.println(key);
  }
}
