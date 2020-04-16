/* @file Hello_I2Cboth.ino

   Revised to use I2C i/o Feb 26/12 - G. D. Young
   Re-check with Keypad 3.0, arduino 1.0.1 Jul 29/12
   Add port-type parameter to constructor for Paul
   Williamson's modification of the Keypad_I2C library Sept 2/13

   April 6/2020 - Keypad_I2C allows optional WireX parameter. Note
   that if WireX parameter is spec'd, then the otherwise optional
   'width' parameter is needed. OK to omit both options when using
   8 bit port chips on Wire

   April 10/2020 - Tested with MCP port on jwire, PCF8574 on Wire
   connected simultaneously to MKR ZERO board. 

|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad_I2C.h>
#include <Keypad_MCP.h>
#include <Keypad.h>
#include <Wire.h>
#include "wiring_private.h"
TwoWire jwire(&sercom3, 0, 1); // Create the new wire instance assigning it to sda 0 scl 1


#define I2CADDR 0x38          //for PCF8574 on Wire
#define I2CADDR2 0x21         //for MCP23008 on jwire

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

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );
Keypad_MCP kpdj( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR2, &jwire );

void setup(){
  Serial.begin(9600);
  while( !Serial ){ /*wait*/ }
  Wire.begin( );
  jwire.begin( );
  pinPeripheral(0, PIO_SERCOM);   //Assign SDA function to pin 0
  pinPeripheral(1, PIO_SERCOM);   //Assign SCL function to pin 1
//  kpd.begin( makeKeymap(keys) );
  kpd.begin( );
  kpdj.begin( );
  Serial.print( "start with kpd pinState = " );
  Serial.println( kpd.pinState_set( ), HEX );
}
void loop(){
  
  char key = kpd.getKey();
  char keyj = kpdj.getKey( );
  
  if (key){
    Serial.println(key);
  }
  if (keyj){
    Serial.println(keyj);
  }
}
