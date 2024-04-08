

void adsorption(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    //set up timer
    sendData(0, false, false, false, false, 0);
    Co2timer = timerBegin(1000000);
    timerAttachInterrupt(Co2timer, &checkTimer);
  //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
    timerAlarm(Co2timer, 60000*1000*30, true, 0);
    checkParameters(ADS, ADS2, elecSensor);

    //start fans

    //wait for 150 minutes to go by
    
    while (timerFlag == false) { //usually is timerFlag == false but test that another day
        //flow co2 is 0 as it is not applicable in adsorption phase
        lcdDisplayWithPot("Adsorption Stage", thermister(ADS, 0), curr.elecTemp, curr.inCo2, curr.outCo2, 0);
        checkParameters(ADS, ADS2, elecSensor);
        //Serial.println(readFlowCO2());
        dataLogging();
        sendData(0, false, true, false, false, 0);
        //delay(200);

    }
    
    //set timer back to false for next cycle
    timerFlag = false;
    //turn off fans and close motors

}      

void heating(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    checkParameters(ADS, ADS2, elecSensor);
    Serial.println("motors Start");
    sendData(1, false, false, false, false, 1);

    delay(400);
    
    //turn the heat on here
    sendData(0, true, false, false, false, 0);
    /*
    while (curr.centCurrentTemp < 125) {
        sendData(0, true, false, false, false, 0, false);
        dataLogging();
        lcdDisplayWithPot("Heating...", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, 0);
        checkParameters(ADS, ADS2, elecSensor);
    }
    */
    bool pumpCase = false;
    bool valveOn = false;
    //temp of center
    while (thermister(ADS, 1) < 120) { 
    //while (timerFlag == false){
      //temp of perimeter
      if (thermister(ADS, 0) > 120) {
        pumpCase = true;
        valveOn = true;
      }
      lcdDisplayWithPot("Heating...", thermister(ADS, 0), curr.elecTemp, curr.inCo2, curr.outCo2, 0);
      
      //temp of perimeter
      
        if (thermister(ADS, 0) > 190) {
          
          //heat off
            sendData(0, false, false, pumpCase, valveOn, 0);
            
            
        }
        else{
          sendData(0, true, false, pumpCase, valveOn, 0);

        }

        dataLogging();
        checkParameters(ADS, ADS2, elecSensor);
    }
    timerFlag = false;

/*
    while(timerFlag2 == false) {
      newtimer = timerBegin(1000000);
      timerAttachInterrupt(newtimer, &checkTimer2);
      //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
      timerAlarm(newtimer, 60000*1000*0.5, true, 0);
      sendData(0, true, false, true, true, 0);
      lcdDisplayWithPot("Desorption Stage", thermister(ADS, 0), curr.elecTemp, curr.inCo2, curr.outCo2, 0);
      dataLogging();
      checkParameters(ADS, ADS2, elecSensor);
    }

    timerFlag2 = false;
*/

}

void desorption(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    heating(ADS, ADS2, elecSensor);
    //starting the pump and valve
    
    checkParameters(ADS, ADS2, elecSensor);
    
    //hold until CO2 in the tank has stopped increasing
    //NEED TO CHANGE THIS WHILE STATEMENT 
    
    while (timerFlag == false)  {
        
        if (thermister(ADS, 1) < 120) {
            sendData(0, true, false, true, true, 0);
        }
        else{
            sendData(0, false, false, true, true, 0);
        }
        //prevCO2 = outputtedCO2;
        checkParameters(ADS, ADS2, elecSensor);
        //lcdDisplayWithPot("D", curr.currentTemp, curr.elecTemp, curr.inCo2, curr.outCo2, curr.flowCo2);
        dataLogging();
    }
    
    //turn off heat, pump and valve and open motors
    sendData(0, false, false, false, false, 0);
}

void coolDown(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    checkParameters(ADS, ADS2, elecSensor);
    //turn on the fans
    sendData(0, false, true, false, false, 2);
    
    while (thermister(ADS, 1) > 30) {
    //while (timerFlag2 == false){
        lcdDisplayWithPot("Cooldown", thermister(ADS, 0), curr.elecTemp, curr.inCo2, curr.outCo2, curr.flowCo2);
        checkParameters(ADS, ADS2, elecSensor);
        dataLogging();
        delay(200);
       
    }
    
    //turn off fans
    sendData(0, false, false, false, false, 0);
    //NEED TO CREATE AN END FUNCTION DISPLAY
    delay(200);
    timerFlag2 = false;
}
