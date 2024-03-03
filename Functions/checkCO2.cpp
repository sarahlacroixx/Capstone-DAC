void checkCO2(){
  // start I2C

  Wire.beginTransmission(ADDR_6713);
  Wire.write(0x04);
  Wire.write(0x13);
  Wire.write(0x8B);
  Wire.write(0x00);
  Wire.write(0x01);
  // end transmission
  Wire.endTransmission();
  // read report of current gas measurement in ppm
  delay(2000);
  Wire.requestFrom(ADDR_6713, 4); //request 4 bytes from slave device

  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();

  Serial.print("Func code: ");
  Serial.print(data[0],HEX);
  Serial.print(" byte count: ");
  Serial.println(data[1],HEX);
  Serial.print("MSB: 0x"); 
  Serial.print(data[2],HEX);
  Serial.print("  ");
  Serial.print("LSB: 0x");
  Serial.print(data[3],HEX);
  Serial.print("  ");
  CO2ppmValue = ((data[2] * 0xFF ) + data[3]);
    return CO2ppmValue;
}
