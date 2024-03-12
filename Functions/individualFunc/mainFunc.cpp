void adsorption(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2Sensor1){
    double currentCO2 = checkCO2(co2Sensor1);
    float currentTemp = checkTherms(ADS);
    double prevCO2 = 0;
    //start fans
    sendData(false, true, false, false);
    //wait for 150 minutes to go by
    while (timerFlag = false) {
        lcdDisplayAdsorption(currentTemp, currentCO2);
        prevCO2 = currentCO2;
        currentCO2 = checkCO2(co2Sensor1);
        currentTemp = checkTherms(ADS);
    }
    //set timer back to false for next cycle
    timerFlag = false;
    //turn off fans
    sendData(false, true, false, false);
    //FIX CODE HERE TO CLOSE STEPPER MOTORS
    stepperMotorsClose(motor1, motor2);
    
}

void heating(ADS1115 ADS, SparkFun_ENS160 co2Sensor2){
    float currentTemp = checkTherms(ADS);
    double CO2 = checkCO2(co2Sensor2);
    
    //turn the heat on here
    sendData(true, false, false, false);
    while (currentTemp < 125) {
        lcdDisplayDesorption(currentTemp, CO2);
        currentTemp = checkTherms(ADS);
        CO2 = checkCO2(co2Sensor1);
    }
}

void desorption(int valve, int pump, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    heating(ADS, co2Sensor2);
    //starting the pump and valve
    sendData(true, false, true, true);
    float currentTemp = checkTherms(ADS);
    double outputtedCO2 = checkCO2(co2sensor2);
    double prevCO2 = 0;
    
    //hold until CO2 in the tank has stopped increasing
    while (outputtedCO2 > prevCO2)  {
        if (currentTemp < 120) {
            sendData(true, false, true, true);
        }
        if (currentTemp > 130) {
            sendData(false, false, true, true);
        }
        prevCO2 = outputtedCO2;
        outputtedCO2 = checkCO2(co2sensor2);
        currentTemp = checkTherms(ADS);
        lcdDisplayDesorption(currentTemp, outputtedCO2);
    }
    //turn off heat, pump and valve
    sendData(false, false, false, false);
}

void coolDown(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    //openStepperMotors(motor1, motor2);
    float currentTemp = checkTherms(ADS);
    double finalCO2 = checkCO2(co2sensor2);
    //turn on the fans
    sendData(false, true, false, false);
    while (currentTemp > 30) {
        //startFans(fan1, fan2);
        lcdDisplayCooldown(currentTemp, finalCO2);
    }
    
    //turn off fans
    sendData(false, false, false, false);
    lcdDisplayCapCO2(finalCO2);
    delay(200);
}
