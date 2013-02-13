/* Keypad_MC16 example
 *
 * Converted from Keypad_I2C (8-bit port) to 16-bit MCP2016 Feb 3/13
 * 
 * The following functions are extended by Keypad_MC16. They have the same
 * parameters and return values as the standard Arduino functions pinMode(),
 * digitalWrite(), and digitalRead().
 *
 *    pin_mode( )        Configures pin as input or output
 *    pin_write( )       Writes a HIGH or LOW to a specified pin.
 *    pin_read( )        Reads a HIGH or LOW from a specified pin.
 *
 * NOTE: As these are used by the library the sketch author shouldn't have to use
 * them directly.
 *
 *  << Constructor >>
 *  Keypad_MC16 kpd(char* userKeymap, byte* row, byte* col, byte numRows, byte numCols, byte address)
 *
 *
 *  Keypad function: See Keypad examples.
 *
 *    void kpd.begin(char *userKeymap);
 *
 *
 *  Wire function:
 *
 *	void Wire.begin(byte address);
 *
 * Stores the I2C address that is needed by pin_read and pin_write. Also, the I2C
 * address can be changed at any time by using the following code;
 *
 *    kpd.begin(i2caddress)
 *
 *
 * The Keypad_MC16 library inherits and extends the Keypad library and the Wire library.
 * That means you MUST #include "Keypad.h" and #include "Wire.h" to make it work.
 *
 */
#include <Keypad_MC16.h>
#include <Keypad.h>
#include <Wire.h>


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

int i2caddress = 0x20; //see hardware info to choose other adresses

Keypad_MC16 kpd = Keypad_MC16( makeKeymap(keys), rowPins, colPins, ROWS, COLS, i2caddress );

void setup(){
  Serial.begin(9600);
  kpd.begin();
}
  
void loop(){
  char key = kpd.getKey();
  
  if (key){
    Serial.println(key);
  }
}
