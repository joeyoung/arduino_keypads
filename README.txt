Description of arduino_keypads library add-ons

G. D. (Joe) Young, July 25, 2012
                   Aug 8, 2012 - removed Keypad lib folder
                   Feb 12, 2013 - add Keypad_MC16
                   Sep 2, 2013 - version 2.0 Keypad_I2C
                   Nov 30, 2013 - minor changes to 'using..' doc for 8575,
                                  add datasheet, pin share example
                   May 19, 2014 - add Keypad_MC17
                   May 24, 2014 - add Keypad_I2Ca
                   May 27, 2014 - additional devices with Keypad_I2Ca
                   May 24, 2015 - Keypad_tw - TinyWireM version for PCF857xx

keypadreadme - this file

folder Keypad_I2C - Joe Young. arduino library, extension of Keypad
                    using PCF8574 or PCF8575 I2C port chip

folder Keypad_I2Ca -Joe Young, arduino library, extension of Keypad
                    using PCA9554 or PCA9555 I2C port chips, and others
                    in this family of expanders.

folder Keypad_MCP - Joe Young. arduino library, extension of Keypad
                    using MCP23008 I2C port chip

folder Keypad_MC16- Joe Young. arduino library, extension of Keypad
                    using MCP23016 I2C, 16-bit port chip

folder Keypad_MC17- Joe Young. arduino library, extension of Keypad
                    using MCP23017 I2C, 16-bit port chip

folder Keypad_tw  - Joe Young, arduino library, extension of Keypad
                    using PCF857xx I2C, port chips

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

May 19, 2014 - Add Keypad_MC17. Originally discussed with Chris Atomix,
2014-01-17. The MCP23017 can be similar to MCP23016, but it has internal
pullups which must be enabled, and 16-bit mode is assumed for the library.

May 24, 2014 - Add Keypad_I2Ca. Use PCA9554, PCA9554A, (8-bit) and PCA9555
(16-bit) port chips for a drop-in, higher-drive upgrade of PCF8574,5 I2C
ports.

May 27, 2014 - confirm (with PCA9539 and TCA6403) that several other port
expanders are compatible with Keypad_I2Ca. See notes file in documents
folder included within Keypad_I2Ca library.

May 24, 2015 - Using TinyWireM library for port expansion on ATtiny 
processors. First one tested with PCF8574, PCF8574A chips on ATtiny85.

Apr 08, 2020 - With MKR ZERO, ESP32, possibly other newer boards, the
inheritance of TwoWire mechanism used resulted in compile error on the
Keypad_xxxx constructor. The Wire library now allows (but not currently
implemented) for more than one I2C port, so the Keypad_xxxx libraries
now have an added optional parameter in their constructors to specify 
the WireX to use for that Keypad_xxx object. The modified libraries may
require slight variations in the setup() - need kpd.begin(), wait for
serial for MKR. Some of the older examples have been left unchanged, see
HelloKeypad_I2C for current setup().

I will be maintaining the I2C bus enhancements, possibly adding other 
keypad interfaces, and the latest version(s) will be posted here on 
github.


