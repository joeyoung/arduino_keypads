/* @file HelloPortShare_MCS17
||    Modified HelloKeypad to add expansion port on SPI, share port pin for LED blink
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
|| @contact G. D. (Joe) Young <jyoung@islandnet.com> Jan 1/25
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
   Modified for Keypad_MCP G. D. (Joe) Young July 29/12
   Modified for Keypad_MC16 G. D. (Joe) Young Jan 26/13
     also HelloKpd28 for 28-pin keypad, using high bit columns
   Modified for Keypad_MC17 G. D. (Joe) Young May 18/14
   Test Keypad_MCS17 - SPI interface
*/
#include <Keypad_MCS17.h>
#include <SPI.h>
#include <Keypad.h>

//#define I2CADDR 0x24
#define CHIPNR 0x04      //S17 hardware addressing uses this symbol
#define CSpin 10         //this group (up to 8) of MCP23S17 select pin

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

Keypad_MCS17 keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, CHIPNR, CSpin );

int expLED = 15;    //expansion port pin not used by keypad scanning
long timMsec;       //a timer for blinking LED

void setup(){
  Serial.begin(9600);
  while( !Serial ){/*wait*/}
//  Wire.begin( );
  SPI.begin( );
  keypad.begin( );
//  keypad.setDebounceTime( 100 );

  keypad.pin_mode( expLED, OUTPUT );    //set pin to be an output for led
  keypad.pin_write( expLED, HIGH );    //turn on LED
  timMsec = millis( );
  
}//setup()
  
void loop(){
  
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
  }//if key found this scan

  if( millis( ) - timMsec > 1000 ) {    //1 second timer
    timMsec = millis( );
    if( keypad.pin_read( expLED ) ){    //toggle LED when time up
      keypad.pin_write( expLED, LOW );
    } else {
      keypad.pin_write( expLED, HIGH );
    }
  }//timer

}//loop()