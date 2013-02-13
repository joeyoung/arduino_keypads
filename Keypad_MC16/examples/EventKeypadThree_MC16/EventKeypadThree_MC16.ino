/* @file EventKeypadThree_MC16.ino

   version for trying keypad (not _I2C) with two keypads
   April 8/12

   Revised to use I2C i/o Feb 26/12 - G. D. Young
   Check for 2nd keypad implementation - HelloKeypadTwo - Feb 28/12

   With arduino-1.0, order of calling keypad functions seems to matter
   -- kpd2 needs to be first.

   July 30/12 test 3 keypads on two MCP23008 ICs: one 4X3 (0x20) and
   two 2X2 on the 0x21 chip.
   
   Feb 4/13 all 3 on one MCP23016 IC 

*/
//#include <Keypad_I2C.h>
//#include <Wire.h>
//#define I2CADDR 0x21

#include <Keypad.h>
#include <Keypad_MC16.h>
#include <Wire.h>


const byte ROWS = 2; // use 4X4 keypad for both 2X2 instances
const byte COLS = 2;
char keys[ROWS][COLS] = {
  {'0','1'},
  {'4','5'}
};
// Digitran keypad-row pinlabels1..4, col4..8, bit numbers of i/o port 0..15
byte rowPins[ROWS] = {0, 1}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8}; //connect to the column pinouts of the keypad

Keypad_MC16 kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, 0x20 );

//#define I2CADDRT 0x22
const byte ROWSR = 2;
const byte COLSR = 2;
char keysR[ROWSR][COLSR] = {
  {'a','b'},
  {'e','f'}
};

byte rowPinsR[ROWSR] = {2, 3}; //connect to the row pinouts of the keypad
byte colPinsR[COLSR] = {8, 9}; //connect to the column pinouts of the keypad

Keypad_MC16 kpdR( makeKeymap(keysR), rowPinsR, colPinsR, ROWSR, COLSR, 0x20 );

const byte ROWSUR = 4;
const byte COLSUR = 3;
char keysUR[ROWSUR][COLSUR] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPinsUR[ROWSUR] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPinsUR[COLSUR] = {4, 5, 6}; //connect to the column pinouts of the keypad

Keypad_MC16 kpdUR( makeKeymap(keysUR), rowPinsUR, colPinsUR, ROWSUR, COLSUR, 0x20 );


void setup(){
//  Wire.begin( );
  kpdUR.begin( makeKeymap(keysUR) );
  kpdR.begin( makeKeymap(keysR) );
  kpd.begin( makeKeymap(keys) );
  kpdUR.addEventListener(keypadEvent); //add an event listener for this keypad
 // since chip is common to all keypads, all must have same iodir copy
  kpdUR.iodir_write( kpdUR.iodir_read( ) & 0xf7ff );
  kpdR.iodir_write( kpdR.iodir_read( ) & 0xf7ff );
  kpd.iodir_write( kpd.iodir_read( ) & 0xf7ff );
  Serial.begin(9600);
  Serial.print( "start. iodir=" );
  Serial.print( kpdUR.iodir_read( ), HEX );
  Serial.print( "  port_state=" );
  Serial.println( kpdUR.pinState_set( ), HEX );
}

char key, keyR, keyUR;
word portState;
boolean blink = false;
word ledpin = 0x0800;

void toggleLED( ) {
    portState = kpdUR.pinState_set( );
    if( portState & ledpin ) {
      kpdUR.port_write( portState & 0xf7ff );
      kpdR.pinState_set( );  // update other objects pinState
      kpd.pinState_set( );
// alternative update method
//      kpdR.port_write( portState & 0xf7ff );
//      kpd.port_write( portState & 0xf7ff );
    } else {
      kpdUR.port_write( portState | 0x0800 );
      kpdR.pinState_set( );
      kpd.pinState_set( );
//      kpdR.port_write( portState | 0x0800 );
//      kpd.port_write( portState | 0x0800 );
    }
} // toggleLED( )

void loop(){

  key = kpd.getKey( );  
  keyUR = kpdUR.getKey( );
  keyR = kpdR.getKey( );
  
  if (key){
    Serial.println(key);
  }
  if( keyR ) {
    Serial.println( keyR );
  }
  if( keyUR ) {
    Serial.println( keyUR );
  }
  if (blink){
//    digitalWrite(ledPin,!digitalRead(ledPin));
    toggleLED( );
    delay(100);
  }
}

//take care of some special events
void keypadEvent(KeypadEvent keyUR){
  switch (kpdUR.getState()){
    case PRESSED:
      switch (keyUR){
        case '#': 
//          digitalWrite(ledPin,!digitalRead(ledPin));
          toggleLED( );
          break;
        case '*': 
//          digitalWrite(ledPin,!digitalRead(ledPin));
          toggleLED( );
        break;
      }
    break;
    case RELEASED:
      switch (keyUR){
        case '*': 
//          digitalWrite(ledPin,!digitalRead(ledPin));
          toggleLED( );
          blink = false;
        break;
      }
    break;
    case HOLD:
      switch (keyUR){
        case '*': blink = true; break;
      }
    break;
  }
}

