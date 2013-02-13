/* HelloKeypadTwo_I2C
      from original file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #

   Revised to use I2C i/o Feb 26/12 - G. D. Young
   Check for 2nd keypad implementation - HelloKeypadTwo - Feb 28/12

   April 5, 2012:
   With arduino-1.0, order of calling keypad functions seems to matter
   -- kpd2 needs to be first.
   
   April 9, 2012: The implementation shown here works fine -- apparently
   arduino-1.0 changed some aspect of Wire that makes timing more 
   critical, as I found some changes in behaviour with different values
   of SDA, SCL pullup resistors. So, here the Keypad library's event
   listener feature is used to create a busy flag for each keypad
   that blocks the call to scan the 'other' keypad as long as the first
   keypad remains busy. Thus, the I2C bus is completely finished with
   one keypad scanning before the other one starts using it.
   
   Aug 24/12 The problem above was fixed with version 3.0 of the
   Keypad library. This example now uses that library and retains the
   lockout feature (which is no longer necessary, but may be useful).

*/
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#define I2CADDR 0x21

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','.','='}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

#define I2CADDRT 0x22
const byte ROWSR = 4; //four rows
const byte COLSR = 3; //three columns
char keysR[ROWSR][COLSR] = {
  {'a','d','/'},
  {'b','e','%'},
  {'c','f','('},
  {'C','0',')'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPinsR[ROWSR] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPinsR[COLSR] = {4, 5, 6}; //connect to the column pinouts of the keypad

Keypad_I2C kpdR( makeKeymap(keysR), rowPinsR, colPinsR, ROWSR, COLSR, I2CADDRT );

boolean kpdbusy, kpdRbusy;
char key, keyR;

void setup(){
  Wire.begin( );
  kpdR.begin( makeKeymap(keysR) );
  kpd.begin( makeKeymap(keys) );
  kpdR.addEventListener( block_kpd );
  kpd.addEventListener( block_kpdR );
  kpdbusy = false;
  kpdRbusy = false;
  Serial.begin(9600);
  Serial.println( "start" );
}


void loop(){

  if( kpdRbusy == false ) key = kpd.getKey( );  
  if( kpdbusy == false ) keyR = kpdR.getKey( );

  if (key){
    Serial.println(key);
  }
  if( keyR ) {
    Serial.println( keyR );
  }
}

void block_kpd( KeypadEvent anykey ) {
  switch( kpdR.getState( ) ) {
    case PRESSED:
      kpdRbusy = true;
    break;
    case RELEASED:
      kpdRbusy = false;
    break;
  } // switch on right, kpdR state
} // block_kpd

void block_kpdR( KeypadEvent anykey ) {
  switch( kpd.getState( ) ) {
    case PRESSED:
      kpdbusy = true;
    break;
    case RELEASED:
      kpdbusy = false;
    break;
  } // switch on left kpd state
} // block_kpdR
