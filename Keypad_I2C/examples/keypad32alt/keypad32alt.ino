// https://github.com/joeyoung/arduino_keypads
// Coded by Pol Clota, credits go to Jow Young for he's library.
//
// Illustrate using an alternate I2C port April 14/20 G. D. (Joe) Young <jyoung@islandnet.com>

#include <Wire.h>
#include "Keypad.h"
#include <Keypad_I2C.h>
#include "wiring_private.h"
TwoWire jwire(&sercom3, 0, 1);   // Create the new wire instance assigning it to sda 0 scl 1

const byte n_rows = 4;
const byte n_cols = 4;

char keys[n_rows][n_cols] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[n_rows] = {0, 1, 2, 3};
byte colPins[n_cols] = {4, 5, 6, 7};

Keypad_I2C myKeypad = Keypad_I2C(makeKeymap(keys), rowPins, colPins, n_rows, n_cols, 0x38, PCF8574, &jwire);

String swOnState(KeyState kpadState)
{
    switch (kpadState)
    {
    case IDLE:
        return "IDLE";
        break;
    case PRESSED:
        return "PRESSED";
        break;
    case HOLD:
        return "HOLD";
        break;
    case RELEASED:
        return "RELEASED";
        break;
    } // end switch-case
    return "";
} // end switch on state function

void setup()
{
    // This will be called by App.setup()
    Serial.begin(115200);
    while (!Serial)
    { /*wait*/
    }
    Serial.println("Press any key...");
  jwire.begin( );
  pinPeripheral(0, PIO_SERCOM);   //Assign SDA function to pin 0
  pinPeripheral(1, PIO_SERCOM);   //Assign SCL function to pin 1
    myKeypad.begin(makeKeymap(keys));
}

char myKeyp = NO_KEY;
KeyState myKSp = IDLE;
auto myHold = false;

void loop()
{

    char myKey = myKeypad.getKey();
    KeyState myKS = myKeypad.getState();

    if (myKSp != myKS && myKS != IDLE)
    {
        Serial.print("myKS: ");
        Serial.println(swOnState(myKS));
        myKSp = myKS;
        if (myKey != NULL)
            myKeyp = myKey;
        String r;
        r = myKeyp;
        Serial.println("myKey: " + String(r));
        if (myKS == HOLD)
            myHold = true;
        if (myKS == RELEASED)
        {
            if (myHold)
                r = r + "+";
            Serial.println(r.c_str());
            myHold = false;
        }
        Serial.println(swOnState(myKS));
        myKey == NULL;
        myKS = IDLE;
    }
}
