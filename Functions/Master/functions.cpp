//place functions here
#include "functions.h"

//CO2 things
int data [4];
double CO2ppmValue;

uint8_t slaveAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0xAE, 0x7C}; //NEED TO UPDATE

typedef struct parameters {
    int pinAssign;
    bool heat;
    bool fans;
    bool pump;
    bool valve;

} parameters;

parameters highVolt;

esp_now_peer_info_t otherInfo;

//FUNCTION FROM LIBRARY
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//function to send data to slave esp32
void sendData(int pinAssign, bool heat, bool fans, bool pump, bool valve){
    highVolt.pinAssign = pinAssign;
    highVolt.heat = heat;
    highVolt.fans = fan;
    highVolt.pump = pump;
    highVolt.valve = valve;
    esp_err_t state = esp_now_send(slaveAddress, (uint8_t *) &highVolt, sizeof(highVolt));

    if (state == ESP_OK) {
        Serial.println("Sent success");
    }else {
        Serial.println("Sent fail");
    }
}

void turnFansOn(){
    sendData(false, true, false, false);
}
void turnFansOff(){
    sendData(false, true, false, false);
}

void lcdDisplayAdsorption(int currentTemp, int currentCO2){
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
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void lcdDisplayCooldown(int currentTemp, int currentCO2){
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
  lcd.setCursor(0, 0);
  lcd.print("Process");
  lcd.setCursor(0, 1);
  lcd.print("Completed");
  lcd.setCursor(0, 1);
  lcd.print("CO2 Captured:");
  lcd.setCursor(14, 1);
  lcd.print(finalCO2);
}


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
    for (int i = 0; i<=1; i++){
        sum = sum + thermistor(ADS, i);
    }
    return sum/2;
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

//display the elaspsed time on the lcd screen
void displayClock(unsigned long StartTime){
  lcd.setCursor(0,3);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  int hour = 0;
  int min = 0;
  int sec = ElapsedTime/1000;
  min = sec/60;
  sec = sec%60;
  if (min >= 60) {
    hour = min/60;
    min = min%60;
  }
  lcd.print("Elap. Time: ");
  lcd.setCursor(12,3);
  if (hour < 10){
    lcd.print("0");
    lcd.setCursor(13,3);
    lcd.print(hour);
  } else {
    lcd.print(hour);
  }
  
  lcd.setCursor(14,3);
  lcd.print(":");
  lcd.setCursor(15, 3);

  if (min < 10){
    lcd.print("0");
    lcd.setCursor(16,3);
    lcd.print(min);
  } else {
    lcd.print(min);
  }
  lcd.setCursor(17,3);
  lcd.print(":");
  lcd.setCursor(18, 3);

  if (sec < 10){
    lcd.print("0");
    lcd.setCursor(19,3);
    lcd.print(sec);
  } else {
    lcd.print(sec);
  }
  
}

void readExpander(){
    Wire.beginTransmission(0x20);
     Wire.write(0x12); // set MCP23017 memory pointer to GPIOB address
     Wire.endTransmission();
     Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
     byte data=Wire.read(); //receive data in binary
}

void readLS(int num){
    byte data = readExpander();
    //put a switch case here to mask the data to read each limit switch
}


//function to read the flow rate co2 sensor (the outputted co2 into the tank)
void readFlowCO2(){
    uint8_t data[2];
    int rawFlowData;

    
    //need to get address of i2c for sensor (i think 0x03 for co2)
    Wire.beginTransmission(0x03);
    //write to 0xF1 to trigger the sensor
    Wire.write(0xF1);
    //close the request
    Wire.endTransmission();

    //read the value from oxo3 and request the number of bytes (example had 3, i think we only need to check for 2 bytes)
    Wire.requestFrom(0x03, 2);

    flowData[0] = Wire.read();
    flowData[1] = Wire.read();
    rawFlowData = (flowData[0] << 8) | flowData[1];
    return rawFlowData;

}




    


