/* file CustomKeypad_MC17 Feb 2/13
||@file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
    Use with I2C i/o G. D. (Joe) Young Feb 28/12

    Use with MCP23008 I2C i/o G. D. (Joe) Young Jul 29/12
    Use with MCP23016 I2C i/o G. D. (Joe) Young Feb 2/13
    Use with MCP23017 I2C i/o G. D. (Joe) Young May 19/14
*/
#include <Keypad_MC17.h>
#include <Keypad.h>        // GDY120705
#include <Wire.h>

#define I2CADDR 0x24

const byte ROWS = 4; //four rows
const byte COLS = 5; //five columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3','W'},
  {'4','5','6','7','X'},
  {'8','9','A','B','Y'},
  {'C','D','E','F','Z'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6, 5, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad_MC17 customKeypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, I2CADDR); 

void setup(){
//  Wire.begin( );
  customKeypad.begin( );        // GDY120705
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey != NO_KEY){
    Serial.println(customKey);
  }
}
