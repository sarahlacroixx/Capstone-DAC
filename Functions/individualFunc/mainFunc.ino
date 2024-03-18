
typedef struct currParameters{
    float inCo2;
    float outCo2;
    float currentTemp;
    float elecTemp;
    int flowCo2;
} currParameters;

currParameters curr;

void checkParameters(ADS1115 ADS, TMP102 elecSensor){
    curr.inCo2 = readCO2(ADS, 0); // NEED TO FIX CHANNEL HERE
    curr.outCo2 = readCO2(ADS, 1); // NEED TO FIX CHANNEL HERE
    curr.currentTemp = checkTherms(ADS);
    curr.elecTemp = checkTempelecBox(elecSensor);
    curr.flowCo2 = readFlowCO2();
  
}

void adsorption(ADS1115 ADS, TMP102 elecSensor){
    checkParameters(ADS, elecSensor);

    //start fans
    sendData(0, false, true, false, false, 0);
    //wait for 150 minutes to go by
    while (timerFlag = false) {
        //flow co2 is 0 as it is not applicable in adsorption phase
        lcdDisplayWithPot("Adsorption", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, 0);
        checkParameters(ADS, elecSensor);
    }
    //set timer back to false for next cycle
    timerFlag = false;
    //turn off fans and close motors
    sendData(0, false, false, false, false, 1);
    
}      

void heating(ADS1115 ADS, TMP102 elecSensor){
    checkParameters(ADS, elecSensor);
    
    //turn the heat on here
    sendData(0, true, false, false, false, 0);
    while (curr.currentTemp < 125) {
        lcdDisplayWithPot("Adsorption", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, 0);
        checkParameters(ADS, elecSensor);
    }
}

void desorption(ADS1115 ADS, TMP102 elecSensor)){
    heating(ADS, co2Sensor2);
    //starting the pump and valve
    sendData(0, true, false, true, true, 0);
    checkParameters(ADS, elecSensor);
    
    //hold until CO2 in the tank has stopped increasing
    //NEED TO CHANGE THIS WHILE STATEMENT 
    while (outputtedCO2 > prevCO2)  {
        if (currentTemp < 120) {
            sendData(0, true, false, true, true, 0);
        }
        if (currentTemp > 130) {
            sendData(0, false, false, true, true, 0);
        }
        prevCO2 = outputtedCO2;
        checkParameters(ADS, elecSensor);
        lcdDisplayWithPot("Desorption", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, curr.flowCo2);
    }
    //turn off heat, pump and valve and open motors
    sendData(0, false, false, false, false, 1);
}

void coolDown(ADS1115 ADS, TMP102 elecSensor){
    checkParameters(ADS, elecSensor);
    //turn on the fans
    sendData(0, false, true, false, false, 0);
    while (curr.currentTemp > 30) {
        lcdDisplayWithPot("Desorption", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, curr.flowCo2);
        checkParameters(ADS, elecSensor);
    }
    
    //turn off fans
    sendData(0, false, false, false, false, 0);
    //NEED TO CREATE AN END FUNCTION DISPLAY
    delay(200);
}
