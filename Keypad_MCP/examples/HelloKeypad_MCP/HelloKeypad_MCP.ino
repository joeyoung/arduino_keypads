/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
   Modified for Keypad_MCP G. D. (Joe) Young July 29/12
*/
#include <Keypad_MCP.h>
#include <Wire.h>
#include <Keypad.h>
#include "wiring_private.h"
TwoWire jwire(&sercom3, 0, 1);   // Create the new wire instance assigning it to sda 0 scl 1

#define I2CADDR 0x21

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

Keypad_MCP keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, &jwire );

void setup(){
  Serial.begin(9600);
  while( !Serial ){/*wait*/}
//  Wire.begin( );
  jwire.begin( );
  pinPeripheral(0, PIO_SERCOM);   //Assign SDA function to pin 0
  pinPeripheral(1, PIO_SERCOM);   //Assign SCL function to pin 1
  keypad.begin( );
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
