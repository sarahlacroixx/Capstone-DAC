/*
 Example 41.2 - Microchip MCP23017 with Arduino
 */
// pins 15~17 to GND, I2C bus address is 0x20
#include "Wire.h"
byte inputs=0;
void setup()
{
 Serial.begin(9600);
 Wire.begin(6,7); // wake up I2C bus
  Wire.beginTransmission(0x20);
  Wire.write(0x01); // IODIRA register
  Wire.write(0xFF); // set all of bank A to outputs
  Wire.endTransmission();
}
void loop()
{
 Wire.beginTransmission(0x20);
 Wire.write(0x12); // set MCP23017 memory pointer to GPIOB address
 Wire.endTransmission();
 Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
 inputs=Wire.read(); // store the incoming byte into "inputs"
 Serial.println(inputs, BIN);
 delay(200); // for debounce
 
}
