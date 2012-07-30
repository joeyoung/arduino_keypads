#include <Wire.h>

#include <Keypad_I2C.h>

#include <Keypad.h>

#define I2CADDR 0x22

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

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );

unsigned long loopCount = 0;
unsigned long timer_t = 0;

void setup(){
	Serial.begin(9600);
	kpd.begin( );
	kpd.pin_mode(2, INPUT);
	kpd.key_update('a', 0x01, true);
	
	// Try playing with different debounceTime settings to see how it affects
	// the number of times per second your loop will run. If you set it too
	// low the library will set it back to 1 mS.
	kpd.setDebounceTime(10);	// setDebounceTime(mS)
}

void loop(){
	char key = kpd.getKey();
	
	// Report the number of times through the loop in 1 second. This will give
	// you a relative idea of just how much the debounceTime has changed the
	// speed of your code. If you set a high debounceTime your loopCount will
	// look good but your keypad will begin to get unresponsive.
	if ((millis() - timer_t) > 1000) {
		Serial.print("Your loop code ran ");
		Serial.print(loopCount);
		Serial.println(" times over the last second");
		loopCount = 0;
		timer_t = millis();
	}
	loopCount++;
	if(key)
		Serial.println(key);
}
