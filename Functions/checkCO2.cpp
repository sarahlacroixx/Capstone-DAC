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

  co2[0] = Wire.read();
  co2[1] = Wire.read();
  co2[2] = Wire.read();
  co2[3] = Wire.read();

  CO2ppmValue = ((co2[2] * 0xFF ) + data[3]);
  return CO2ppmValue;
}


