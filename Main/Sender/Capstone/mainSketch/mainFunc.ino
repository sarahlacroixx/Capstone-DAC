

void adsorption(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    //set up timer
    sendData(0, false, false, false, false, 0);
    Co2timer = timerBegin(1000000);
    timerAttachInterrupt(Co2timer, &checkTimer);
  //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
    timerAlarm(Co2timer, 60000*1000*150, true, 0);
    checkParameters(ADS, ADS2, elecSensor);

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

void desorption(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    checkParameters(ADS, ADS2, elecSensor);
    Serial.println("motors Start");
    sendData(1, false, false, false, false, 1);

    delay(400);
    
    //turn the heat on here
    sendData(0, true, false, false, false, 0);
    
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


}


void coolDown(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    checkParameters(ADS, ADS2, elecSensor);
    //turn on the fans and open motors
    sendData(0, false, true, false, false, 2);
    
    while (thermister(ADS, 1) > 30) {
        lcdDisplayWithPot("Cooldown", thermister(ADS, 0), curr.elecTemp, curr.inCo2, curr.outCo2, curr.flowCo2);
        checkParameters(ADS, ADS2, elecSensor);
        dataLogging();
        delay(200);
       
    }
    
    //turn off fans
    sendData(0, false, false, false, false, 0);
    delay(200);
}
