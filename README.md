### arduino_keypads

This repository contains several separate arduino format libraries, each one adding
to the library Keypad external expansion port communication via an I2C (or `Wire( )`) 
port on the arduino microprocessor. 

The separate libraries each handle the I/O needed by Keypad for different expansion
port chips:

 * `Keypad_I2C` - PCF8574, PCF8574A and PCF8575
 * `Keypad_I2Ca` - PCA9554, PCA9555, PCA9534, PCA9535, PCA9539, PCA6408, PCA6416
 * `Keypad_MC16` - MCP23016
 * `Keypad_MCP` - MCP23008
 * `Keypad_MC17` - MCP23017
 * `Keypad_tw`  - PCF8574, PCF8575 using the TinyWire lib on atTiny processors
 
 The reason for separate libraries for each sort of expansion port IC is that their
 register sets, while implementing similar I/O functions, are generally not compatible
 among the devices, or/and because the I/O needed for Keypad is a small subset of
 the port chip capabilities, a system will likely only contain one type of port IC so
 a simple program library for that specific device will have a smaller memory 
 footprint than would a single larger program that could handle all devices.
 
 To use the libraries, download the .zip file, unzip to a temporary location, and
 then move the library desired into your libraries folder. The library manager 
 does not work on the entire repository.
 
 A recent update to the libraries permits the optional specifation of an I2C port 
 as an alternative or in addition to `Wire` on those arduino that have more than
 one I2C bus. See the example `Hello_I2Cboth` in `Keypad_I2C` for using two 
 expansion port chips on MKR ZERO. See also the arduino tutorial [Adding more Serial
 Interfaces to SAMD microcontrollers (SERCOM)](https://www.arduino.cc/en/Tutorial/SamdSercom)
 
 The [`Keypad`](https://www.github.com/Chris--A/Keypad) library is found on github.
 
 An interesting use of `Keypad's` features enables decoding of quadrature encoders--see
 the examples `KeypadEnc`, and `KeypadEnc_I2C` in the `Keypad_I2C` library.

The folder `scadKeypadLabel` has a simple scad script for creating keypad overlays.
 
 
