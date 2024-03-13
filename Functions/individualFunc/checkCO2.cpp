int samples = 0;

//check co2 takes the runing average, not needed anymore
long checkCO2(RunningAverage currAvg, ADS1115 ADS, int channel){
    //load the first 10 samples

    currAvg.addValue(readCO2(ADS, channel));
    samples++;

    if (samples == 10){
        samples = 0;
        myRA.fillValue(400, 10);
    }
 }

float readCO2(ADS1115 ADS, int channel){
    int16_t val;
    switch(channel){
        case 0:
            val = ADS.readADC(0);
            break;
        case 1:
            val = ADS.readADC(1);
            break;   
    }
    float f = ADS.toVoltage(2);
    float reading = val * f/2;

    float CO2Value = reading*500.00;
    return CO2Value;
}
