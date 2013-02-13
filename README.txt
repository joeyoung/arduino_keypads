Description of arduino_keypads library add-ons

G. D. (Joe) Young, July 25, 2012
                   Aug 8, 2012 - removed Keypad lib folder
                   Feb 12, 2013 - add Keypad_MC16

keypadreadme - this file

folder Keypad_I2C - Joe Young. arduino library, extension of Keypad
                    using PCF8574 I2C port chip

folder Keypad_MCP - Joe Young. arduino library, extension of Keypad
                    using MCP23008 I2C port chip

folder Keypad_MC16- Joe Young. arduino library, extension of Keypad
                    using MCP23016 I2C, 16-bit port chip

To use the Keypad_xxx libraries requires that the Keypad library be 
present and included within your sketch. The Keypad library is maintained
by Mark Stanley, Alexander Brevig on the Arduino web site--search for 
keypad.zip. The most recent version that's been checked out with the
I2C additions here is Keypad version 3.1.

Within each folder are the .cpp, .h, and keywords.txt files comprising the 
arduino library code, an additional folder called examples, and possibly
another folder called docs. The directory structure is such that adding
the library to the arduino environment is accomplished by placing the top
level folders (Keypad, Keypad_I2C, and/or Keypad_MCP) into the libraries
folder within the arduino sketchbook folder.

I will be maintaining the I2C bus enhancements, possibly adding other 
keypad interfaces, and the latest version(s) will be posted here on 
github.


