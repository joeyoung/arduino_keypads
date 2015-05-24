/* @file HelloKeypad.pde

   May 22, 2015 - use keypad_I2c, LiquidCrystal_I2C. helloKeypLcd
                - use TinyWireM for use with ATtiny85 helloKeyPLcdtw
   
   Revised to use I2C i/o Feb 26/12 - G. D. Young
   Re-check with Keypad 3.0, arduino 1.0.1 Jul 29/12
   Add port-type parameter to constructor for Paul
   Williamson's modification of the Keypad_I2C library Sept 2/13

|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad_tw.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
//#include <Wire.h>
#include <TinyWireM.h>

#define I2CADDR 0x38  // keypad on PCF8574A
#define LCD_ADR 0x20  // set the LCD address to 0x20
#define LCD_CHRS 16   // for a 16 chars ...
#define LCD_LINS 2    //   and 2 line display

LiquidCrystal_I2C lcd( LCD_ADR, LCD_CHRS, LCD_LINS );

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','<'},
  {'*','0','#','>'}
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

Keypad_tw kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );

void setup(){
//  Wire.begin( );
  lcd.init( );    //includes init for TinyWire
//  kpd.begin( makeKeymap(keys) );    //redundant here--done in constructor
  kpd.begin(  );
  lcd.backlight();
  lcd.print( "start" );
  lcd.setCursor( 0, 1 );
}
void loop(){
  
  char key = kpd.getKey();
  
  if (key){
    lcd.print(key);
  }
}
