
uint16_t readFlowCO2(){
    uint8_t flowData[2];
    uint16_t rawFlowData;

    
    //need to get address of i2c for sensor (i think 0x03 for co2)
    Wire.beginTransmission(0x01);
    //write to 0xF1 to trigger the sensor
    Wire.write(0xF1);
    //close the request
    Wire.endTransmission();

    //read the value from oxo3 and request the number of bytes (example had 3, i think we only need to check for 2 bytes)
    Wire.requestFrom(0x01, 2);

    flowData[0] = Wire.read();
    flowData[1] = Wire.read();
    rawFlowData = (flowData[0] << 8) | flowData[1];
    Serial.println(rawFlowData);
    if ( rawFlowData <20001){
    return rawFlowData;
    } else{
        return 0; //false reading will return 0
    }

}
