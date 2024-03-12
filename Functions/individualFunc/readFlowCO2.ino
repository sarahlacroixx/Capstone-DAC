void readFlowCO2(){
    uint8_t data[2];
    int rawFlowData;

    
    //need to get address of i2c for sensor (i think 0x03 for co2)
    Wire.beginTransmission(0x03);
    //write to 0xF1 to trigger the sensor
    Wire.write(0xF1);
    //close the request
    Wire.endTransmission();

    //read the value from oxo3 and request the number of bytes (example had 3, i think we only need to check for 2 bytes)
    Wire.requestFrom(0x03, 2);

    flowData[0] = Wire.read();
    flowData[1] = Wire.read();
    rawFlowData = (flowData[0] << 8) | flowData[1];
    return rawFlowData;

}
