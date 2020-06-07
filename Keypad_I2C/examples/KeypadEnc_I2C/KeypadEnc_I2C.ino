/* KeypadEnc_I2C - using Keypad's Multikey for rotary encoder
|| rotary encoder  - GDY May 30, 2020
|| back-fit the I2C version to Keypad alone - GDY Jun 6/20
|| prepare this version for Keypad_I2C example - GDY Jun 7/20
||
|| The rotary encoder common is driven by a column signal, the quadrature signals
|| connect to two rows so that the encoder scans as key A and B in this example.
|| The push switch connects to produce key C.
||
|| Multikey is needed because half of the detent positions of the encoder has both
|| outputs switched to common, a condition which is handled by MultiKey. The switch
|| on key state detects direction by maintaining a pair of boolean kA and kB that
|| corrrespond to the state of the 'key' - true when pressed, false when released.
|| Then direction 'up' rotation is given by whether kA is true (or false) when kB
|| changes to true (or false). Vice versa for down direction detection.
||
|| The pins for the encoder cannot be shared with a keypad because if the encoder
|| stops on both closed, then two rows are connected and only first key encountered
|| is reported. Best to have the encoder declared as its own 1 X 3 keypad, not part
|| of a keypad matrix.
||
|| The decoding shown counts one per detent for the encoder type which advances
|| a half-cycle per detent. For an encoder which advances a full cycle between
|| each detent, the count will advance two times per detent. If one count/detent
|| is desired, the RELEASED case can be changed to the version in comments below.
||
|| @file MultiKey.ino
|| @version 1.0
|| @author Mark Stanley
|| @contact mstanley@technologist.com
||
|| @description
|| | The latest version, 3.0, of the keypad library supports up to 10
|| | active keys all being pressed at the same time. This sketch is an
|| | example of how you can get multiple key presses from a keypad or
|| | keyboard.
|| #
*/

#include <Keypad_I2C.h>
#include <Wire.h>
#define I2CADDR 0x38        // address of PCF8574 chip on I2C bus

const byte ROWS = 3; //four rows
const byte COLS = 1; //three columns
char keys[ROWS][COLS] = {
  {'A'},
  {'B'},
  {'C'}
 };
byte rowPins[ROWS] = {0, 1, 2, }; //connect to the row pinouts of the kpd
byte colPins[COLS] = {7}; //connect to the column pinouts of the kpd


Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );



void setup() {
  Serial.begin(9600);
  while( !Serial ){/*wait*/}   //for USB serial switching boards
  Wire.begin( );
  kpd.begin( );
  kpd.setDebounceTime(1);
}

unsigned long loopCount = 0;
unsigned long startTime = millis();
String msg = "";
int enccount, prevcount;

void loop() {

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd.getKeys())
  {
    bool kA, kB;
    for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
            case PRESSED:
                msg = " PRESSED.";
                if( kpd.key[i].kchar == 'B' ) {    // rotary encoder direction detection
                  if( kA ) enccount++;           //msg = " up";
                  kB = true;
                }
                if( kpd.key[i].kchar == 'A' ) {
                  if( kB ) enccount--;            //msg = " dn";
                  kA = true;
                }               // end encoder detection on make
                if( kpd.key[i].kchar == 'C' ) Serial.println( 'C' );
                break;
//            case RELEASED:
//                kA = false;
//                kB = false;
//                break;
            case RELEASED:
                msg = " RELEASED.";
                if( kpd.key[i].kchar == 'B' ) {    // rotary encoder direction detection
                  if( !kA ) enccount++;            //msg = " up";
                  kB = false;
                }
                if( kpd.key[i].kchar == 'A' ) {
                  if( !kB ) enccount--;            //msg = " dn";
                  kA = false;
                }               // end encoder detection on break
                break;
        }
 //       Serial.print("Key ");
 //       Serial.print(kpd.key[i].kchar);
 //        if( kpd.key[i].kchar == 'A' || kpd.key[i].kchar == 'B' ) Serial.println(msg);
      }
    }
  }

  if(enccount != prevcount ) {
    prevcount = enccount;
    Serial.println( enccount, DEC );
  }
  
}  // End loop
