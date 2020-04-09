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
   Modified for Keypad_MC17 G. D. (Joe) Young May 18/14
   Modified to HelloKpd64_MC17 for full 64 keys G. D. (Joe) Young Mar 25/20
   Using loop counter to assess scanning times with one or two 64-key instances:
    one instance loop count ~ 20000/sec
    two instance loop count = 10/sec !! 
    adjusting debounce time of kpd alone with 2 instances:
      10 msec  10/sec  same if both set       27/sec wi Wire clock 400
      100 msec  2080/sec  10700/sec with both  23180/sec wi Wire clock 400
      1000 msec  5390/sec  33250/sec with both  33700/sec wi Wire clock 400
    Mar 27/20 adding more instances with 100 msec debounce time:
      3 instances, 100 KHz bus --> 3435/sec loop count
                   400             14500/sec
      4 instances, 100                7/sec
                   400              9800/sec     72000/sec with MKR ZERO
*/
#include <Wire.h>
#include <Keypad.h>
#include <Keypad_MC17.h>

#define I2CADDR 0x24
#define I2CADDR2 0x27
#define I2CADDR3 0x26
#define I2CADDR4 0x25

const byte ROWS = 8; //eight rows
const byte COLS = 8; //eight columns
char keys[ROWS][COLS] = {
  {'0','1','2','3','4','5','6','7'},
  {'8','9',':',';','<','=','>','?'},
  {'@','A','B','C','D','E','F','G'},
  {'H','I','J','K','L','M','N','O'},
  {'P','Q','R','S','T','U','V','W'},
  {'X','Y','Z','[','\\',']','^','_'},
  {'`','a','b','c','d','e','f','g'},
  {'h','i','j','k','l','m','n','o'}
};
byte rowPins[ROWS] = {0, 1, 2, 3, 4, 5, 6, 7}; //connect to the row bit numbers conected to keypad
byte colPins[COLS] = {8, 9, 10, 11, 12, 13, 14, 15}; //connect to the column bit numbers

Keypad_MC17 keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
Keypad_MC17 keypad2( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR2 );
Keypad_MC17 keypad3( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR3 );
Keypad_MC17 keypad4( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR4 );

unsigned long loopCount = 0;
unsigned long timer_m = 0;

void setup(){
  Serial.begin(9600);
  while( !Serial ){ /*wait*/ }
  Wire.begin( );
  Serial.println( "starting init" );
  keypad.begin( );
  keypad2.begin( );
  keypad3.begin( );
  keypad4.begin( );
  keypad.setDebounceTime( 100 );
  keypad2.setDebounceTime( 100 );
  keypad3.setDebounceTime( 100 );
  keypad4.setDebounceTime( 100 );
  Serial.println( " entering loop " );
  Wire.setClock( 400000 );
}
  
void loop(){
  char key = keypad.getKey();
  char key2 = keypad2.getKey( );
  char key3 = keypad3.getKey( );
  char key4 = keypad4.getKey( );
  
  if ((millis() - timer_m) > 1000) {
    Serial.print("Your loop code ran ");
    Serial.print(loopCount);
    Serial.println(" times over the last second");
    loopCount = 0;
    timer_m = millis();
  }
  loopCount++;
  if (key){
    Serial.print(key);
    Serial.print( "  " );
    Serial.println( key - 48 );
  }
  if( key2 ) {
    Serial.print( key2 );
    Serial.print( "  " );
    Serial.println( key2 - 48 + 64 );
  }
  if( key3 ) {
    Serial.print( key3 );
    Serial.print( "  " );
    Serial.println( key3 - 48 + 128 );
  }
  if( key4 ) {
    Serial.print( key4 );
    Serial.print( "  " );
    Serial.println( (int)(key4) - 48 + 192 );
  }
  
}
