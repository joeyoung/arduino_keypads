/* Keypad_I2C no headers example
 * 
 *  This example demonstrates the need to include three different headers to compile properly.
 *
 * The Keypad_I2C library inherits and extends the Keypad library and the Wire library.
 * That means you MUST also #include "Keypad.h" and #include "Wire.h" to make it work.
 *
 *
 *  Start with Sketch->Import Library...->Keypad
 *
 *  Then Sketch->Import Library...->Keypad_I2C
 *
 *  And finally  Sketch->Import Library...->Wire
 *
 *  NOTE: The order doesn't matter.
 *
 * Now the sketch should compile properly.
 *
 */

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

int i2caddress = 0x22;

Keypad_I2C kpd = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, i2caddress );

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
