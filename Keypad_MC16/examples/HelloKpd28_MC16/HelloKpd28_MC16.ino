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
     also HelloKpd28 for 28-pin keypad, using high bit columns
*/
#include <Keypad_MC16.h>
#include <Wire.h>
#include <Keypad.h>

#define I2CADDR 0x20

const byte ROWS = 4; //four rows
const byte COLS = 7; //seven columns
char keys[ROWS][COLS] = {
  {'1','2','3', '0','1','2','3'},
  {'4','5','6', '4','5','6','7'},
  {'7','8','9', '8','9','A','B'},
  {'*','0','#', 'C','D','E','F'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row bit numbers conected to keypad
byte colPins[COLS] = {4, 5, 6, 7, 8, 9, 10}; //connect to the column bit numbers

Keypad_MC16 keypad = Keypad_MC16( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

void setup(){
  Serial.begin(9600);
  keypad.begin( );
//  keypad.setDebounceTime( 100 );
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }
}
