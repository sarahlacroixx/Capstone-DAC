

float readCO2(ADS1115 ADS2, int channel){
    int16_t val;
    val = ADS2.readADC(channel);

    float f = ADS2.toVoltage(2);
    float reading = val * f/2;

    float CO2Value = reading*1000.00;
    return CO2Value;
}
