// Hello MCP23S09
//
// started: January 18, 2025  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised:
//
// Hello Keypad example for the SPI interface MCP23S09 8-bit port IC
//
// This chip does not have hardware addressing, so does not mix easily with other MCP23Sxx
// supported by the Keypad_MCS17 library, it needs to be selected with unique chip select
//
//
#include <Keypad_MCS17.h>
#include <SPI.h>
#include <Keypad.h>


#define CHIPNR 0x00      //S09 hardware addressing does not use hardware address, constr needs 0
#define CSpin2 5         //for other devices

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row bit numbers conected to keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column bit numbers

Keypad_MCS17 kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, CHIPNR, CSpin2, MCP23S09 );

void setup(){
  Serial.begin(9600);
  while( !Serial ){/*wait*/}
  SPI.begin( );
  kpd.begin( );
//  keypad.setDebounceTime( 100 );

}//setup()
  
void loop(){
  
  char key = kpd.getKey( );
  
  if (key){
    Serial.println(key);
  }//if key found this scan


}//loop()
