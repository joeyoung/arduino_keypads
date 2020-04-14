/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
   Modified for Keypad_MCP G. D. (Joe) Young July 29/12
   Modified for Keypad_MC16 G. D. (Joe) Young Jan 26/13
*/
#include <Keypad_MC16.h>
#include <Keypad.h>
#include <Wire.h>

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

Keypad_MC16 keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

void setup(){
  Serial.begin(9600);
  while(!Serial){/*wait*/}
  Wire.begin( );
  keypad.begin( );
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
