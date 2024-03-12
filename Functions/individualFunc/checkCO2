int samples = 0;
long checkCO2(RunningAverage currAvg, ADS1115 ADS, int channel){
    //load the first 10 samples

    currAvg.addValue(readCO2(ADS, channel));
    samples++;

    if (samples == 10){
        samples = 0;
        myRA.fillValue(400, 10);
    }
 }

long readCO2(ADS1115 ADS, int channel){
    int16_t val;
    switch(channel){
        case 0:
            val = ADS.readADC(0);
            break;
        case 1:
            val = ADS.readADC(1);
            break;   
    }

    CO2Value = map(val, 0, 5, 0, 5000);
    return CO2Value;
}
