//place functions here
#include "functions.h"

//CO2 things
int data [4];
double CO2ppmValue;

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2){
    //change pin number depending on limit switch pin
    while (digitalRead(7) == LOW) {
        motor1.run();
        motor2.run();
    }
    
}

void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2){
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
  lcd.setCursor(0, 2);
  lcd.print("CO2 Level:");  
  lcd.setCursor(12, 2);
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
  lcd.print("CO2 Level:");
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}

void lcdDisplayWaiting(){
  lcd.clear();
  lcd.setCursor(0, 0);
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
  lcd.print("CO2 Level:");
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
  lcd.print("CO2 Captured:");
  lcd.setCursor(14, 1);
  lcd.print(finalCO2);
}


void startFans(fan1, fan2){
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  
}
void turnOffFans(fan1 fan2){
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
}

double checkCO2(SparkFun_ENS160 sensor){
    double CO2ppmValue = 0;
    if(sensor.checkDataStatus()) {
        CO2ppmValue = sensor.getECO2();
    }
    return CO2ppmValue;
}

float checkTempElecBox(TMP102 sensor0) {
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

void adsorption(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2Sensor1){
    double currentCO2 = checkCO2(co2Sensor1);
    float currentTemp = checkTherms(ADS);
    double prevCO2 = 0;
    //startFans(fan1, fan2);
    //wait for 150 minutes to go by
    while (timerFlag = false) {
        lcdDisplayAdsorption(currentTemp, currentCO2);
        prevCO2 = currentCO2;
        currentCO2 = checkCO2(co2Sensor1);
        currentTemp = checkTherms(ADS);
    }
    //set timer back to false for next cycle
    timerFlag = false;
    //turnOffFans(fan1, fan2);
    stepperMotorsClose(motor1, motor2);
    
}

float thermister(ADS1115 ADS, int channel){
 ADS.setGain(0);

  float baseTemp = 273.15 + 25.00;
  float beta = 3984.00;
  const int16_t r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;

  float rt; //Thermistor resistance
  float reading; //Current ADC Reading value
    int16_t val;
   //read ADC
    switch(channel){
        case 0:
            val = ADS.readADC(0);
            break;
        case 1:
            val = ADS.readADC(1);
            break;   
    }
            
 
  float f = ADS.toVoltage(2);  // voltage factor
  float two = 2.000;
  reading = val * f/two;
    
  //Voltage divider equation to solve for thermistor resistance
  rt = (r1 * 3.3/reading)-r1;
    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(baseTemp))+((1/beta) * log(rt/r0))))-273.15;
  return temp;

}

//function to check all of the thermistors from each adc channel and return the average value
float checkTherms(ADS1115 ADS){
    float sum = 0;
    for (int i = 0; i<=3; i++){
        //sum = sum + thermistor(ADS, 1;);
    }
    return sum/4;
}

void heating(ADS1115 ADS, SparkFun_ENS160 co2Sensor2){
    float currentTemp = checkTherms(ADS);
    double CO2 = checkCO2(co2Sensor2);
    while (currentTemp < 125) {
        //heatON();
        lcdDisplayDesorption(currentTemp, CO2);
        currentTemp = checkTherms(ADS);
        CO2 = checkCO2(co2Sensor1);
    }
}

void desorption(int valve, int pump, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    heating(ADS, co2Sensor2);
    //openValve(valve);
    //startPump(pump):
    float currentTemp = checkTherms(ADS);
    double outputtedCO2 = checkCO2(co2sensor2);
    double prevCO2 = 0;
    
    //hold until CO2 in the tank has stopped increasing
    while (outputtedCO2 > prevCO2)  {
        if (currentTemp < 120) {
            //heatON();
        }
        if (currentTemp > 130) {
            //heatOFF();
        }
        prevCO2 = outputtedCO2;
        outputtedCO2 = checkCO2(co2sensor2);
        currentTemp = checkTherms(ADS);
        lcdDisplayDesorption(currentTemp, outputtedCO2);
    }
    stopPump(pump);
    closeValve(valve);
}

void coolDown(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    //openStepperMotors(motor1, motor2);
    float currentTemp = checkTherms(ADS);
    double finalCO2 = checkCO2(co2sensor2);
    while (currentTemp > 30) {
        //startFans(fan1, fan2);
        lcdDisplayCooldown(currentTemp, finalCO2);
    }
    //turnOffFans(fan1, fan2);
    lcdDisplayCapCO2(finalCO2);
    delay(200);
}






    


