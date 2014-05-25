/* @file HelloKeypad.ino

   * Revised to use I2C i/o Feb 26/12 - G. D. Young
   * Re-check with Keypad 3.0, arduino 1.0.1 Jul 29/12
   * Add port-type parameter to constructor for Paul
     Williamson's modification of the Keypad_I2C library Sept 2/13
   * Use Keypad_I2Ca support for PCA9554, PCA9555 port chips GDY 14-5-23

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
#define I2CADDR 0x3f

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

Keypad_I2Ca kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCA9554 );

void setup(){
  Wire.begin( );
  kpd.begin( makeKeymap(keys) );
  Serial.begin(9600);
  Serial.println( "start" );
}
void loop(){
  
  char key = kpd.getKey();
  
  if (key){
    Serial.println(key);
  }
}
