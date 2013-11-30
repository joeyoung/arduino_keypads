Description of arduino_keypads library add-ons

G. D. (Joe) Young, July 25, 2012
                   Aug 8, 2012 - removed Keypad lib folder
                   Feb 12, 2013 - add Keypad_MC16
                   Sep 2, 2013 - version 2.0 Keypad_I2C
				   Nov 30, 2013 - minor changes to 'using..' doc for 8575,
								  add datasheet, pin share example

keypadreadme - this file

folder Keypad_I2C - Joe Young. arduino library, extension of Keypad
                    using PCF8574 or PCF8575 I2C port chip

folder Keypad_MCP - Joe Young. arduino library, extension of Keypad
                    using MCP23008 I2C port chip

folder Keypad_MC16- Joe Young. arduino library, extension of Keypad
                    using MCP23016 I2C, 16-bit port chip

To use the Keypad_xxx libraries requires that the Keypad library be 
present and its header <Keypad.h> be included within your sketch. 
The Keypad library is maintained by Mark Stanley, Alexander Brevig on 
the Arduino web site--search in http://playground.arduino.cc/code/Keypad 
The most recent version that's been checked out with the I2C additions 
here is Keypad version 3.1.

Within each folder are the .cpp, .h, and keywords.txt files comprising the 
arduino library code, an additional folder called examples, and possibly
another folder called docs. The directory structure is such that adding
the library to the arduino environment is accomplished by placing the top
level folders (Keypad, Keypad_I2C, Keypad_MC16 and/or Keypad_MCP) into the
libraries folder within the arduino sketchbook folder.

September 2, 2013 - Paul Williamson has extended the Keypad_I2C library
so that it now supports the PCF8575 16-bit I2C port IC, while retaining 
backward compatibility with the PCF8574 8-bit I2C port IC. 

November 30, 2013 - Verified use with PCF8575, note difference between
PCF8575 (pullups) and PCF8575C (no pullups) in 'using...' document, add
HelloKeyShr example to illustrate port pin sharing feature.

I will be maintaining the I2C bus enhancements, possibly adding other 
keypad interfaces, and the latest version(s) will be posted here on 
github.


