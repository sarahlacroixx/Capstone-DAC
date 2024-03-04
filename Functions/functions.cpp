//place functions here
#include "functions.h"

//CO2 things
int data [4];
double CO2ppmValue;

void stepperMotorsClose(){
    //change pin number depending on limit switch pin
    while digitalRead(7) == LOW {
        motor1.run();
        motor2.run();
    }
    
}

void stepperMotorsOpen(){
    //add code here to open stepper motor
}

void lcdDisplayAdsorption(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adsorption Stage");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 1);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:);
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}
void lcdDisplayDesorption(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desorption Stage");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 1);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:);
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}

void lcdDisplayWaiting(){
    lcd.clear();
  lcd.setCurser(0, 0);
  lcd.print("Waiting...");
}

void lcdDisplayCooldown(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cooldown");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 1);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:);
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}
void lcdDisplayCapCO2(int finalCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process");
  lcd.setCursor(0, 1);
  lcd.print("Completed");
  lcd.setCursor(0, 1);
  lcd.print("CO2 Captured:);
  lcd.setCursor(14, 1);
  lcd.print(currentCO2);
}

void startFans(){
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  
}
void turnOffFans(){
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
}

double checkCO2(SparkFun_ENS160 sensor){
    double CO2ppmValue = 0;
    if(myENS.chechDataStatus()) {
        CO2ppmValue = sensor.getECO2();
    }
    return CO2ppmValue;
}

double checkTempElecBox(TMP102 sensor0) {
  float temperature;

  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read temperature data
  temperature = sensor0.readTempC();
  // Place the sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  return temperature;

}

void openValve(int valve){
  digitalWrite(valve, HIGH);
}

void closeValve(int valve){
  digitalWrite(valve, LOW);
}

void startPump(int pump){
  digitalWrite(pump, HIGH);
}

void stopPump(int pump){
  digitalWrite(pump, LOW);
}

void heatON(int relay){
    digitalWrite(relay, LOW);
}

void hearOFF(int relay){
    digitalWrite(relay, LOW);
}

void adsorbtion(SparkFun_ENS160 co2Sensor1){
    currentCO2 = checkCO2();
    startFans();
    //wait for 150 minutes to go by
    while timerFlag = false {
        lcdDisplay(currentTemp, currentCO2);
        prevCO2 = currentCO2;
        currentCO2 = checkCO2(co2Sensor1);
        currentTemp = checkTherms();
    }
    //set timer back to false for next cycle
    timerFlag = false;
}
//function to check all of the thermistors from each adc channel and return the average value
float checkTherms(){
    float sum = 0;
    for (int i = 0; i<=3; i++){
        sum = sum + thermistor(ADS, 1;);
    }
    return sum/4;
}

void heating(SparkFun_ENS160 co2Sensor2){
    currentTemp = checkTherms();
    CO2 = checkCO2(co2Sensor2);
    while currentTemp < 125 {
        heatON();
        lcdDisplayDesorption(currentTemp, CO2);
        currentTemp = checkTherms();
        CO2 = checkCO2(co2Sensor1);
    }
}

void desorption(SparkFun_ENS160 co2sensor2){
    heating(co2Sensor2)'
    openValve();
    startPump():
    currentTemp = checkTherms();
    outputtedCO2 = checkCO2(co2sensor2);
    
    //hold until CO2 in the tank has stopped increasing
    while outputtedCO2 > prevCO2  {
        if currentTemp < 120{
            heatON();
        }
        if currentTemp > 130 {
            heatOFF();
        }
        prevCO2 = outputtedCO2;
        outputtedCO2 = checkCO2(co2sensor2);
        currentTemp = checkTherms();
        lcdDisplayDesorption(currentTemp, outputtedCO2);
    }
    stopPump();
    closeValve();
}

void coolDown(){
    openStepperMotors();
    currentTemp = checkTherms();
    finalCO2 = checkCO2(co2sensor2);
    while currentTemp > 30{
        startFans();
        lcdDisplayCooldown(finalCO2);
    }
    turnOffFans();
}




    


