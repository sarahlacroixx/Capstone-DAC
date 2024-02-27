//place functions here
#include "functions.h"

//CO2 things
int data [4];
int CO2ppmValue;

void stepperMotorsClose(){
    //change pin number depending on limit switch pin
    while digitalRead(7) == LOW {
        motor1.runSpeed();
        motor2.runSpeed();
    }
    
}

void stepperMotorsOpen(){
    //add code here to open stepper motor
}

void lcdDisplay(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 0);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:);
}

void lcdDisplayStarting(){
  lcd.setCurser(0, 0);
  lcd.print("Starting...");
}

void startFans(){
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  
}
void turnOffFans(){
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
}

double checkCO2(){
  // start I2C

  Wire.beginTransmission(ADDR_6713);
  Wire.write(0x04);
  Wire.write(0x13);
  Wire.write(0x8B);
  Wire.write(0x00);
  Wire.write(0x01);
  // end transmission
  Wire.endTransmission();
  // read report of current gas measurement in ppm
  delay(2000);
  Wire.requestFrom(ADDR_6713, 4); //request 4 bytes from slave device

  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();

  Serial.print("Func code: "); Serial.print(data[0],HEX);
  Serial.print(" byte count: "); Serial.println(data[1],HEX);
  Serial.print("MSB: 0x");  Serial.print(data[2],HEX);
  Serial.print("  ");
  Serial.print("LSB: 0x");  Serial.print(data[3],HEX);
  Serial.print("  ");
  CO2ppmValue = ((data[2] * 0xFF ) + data[3]);
    return CO2ppmValue;
}

double checkCO2inTank(){
    //this code needs to be changed to address the CO2 sensor in the tank rather than the one inside the chamber
    // start I2C

  Wire.beginTransmission(ADDR_6713);
  Wire.write(0x04);
  Wire.write(0x13);
  Wire.write(0x8B);
  Wire.write(0x00);
  Wire.write(0x01);
  // end transmission
  Wire.endTransmission();
  // read report of current gas measurement in ppm
  delay(2000);
  Wire.requestFrom(ADDR_6713, 4); //request 4 bytes from slave device

  data[0] = Wire.read();
  data[1] = Wire.read();
  data[2] = Wire.read();
  data[3] = Wire.read();

  Serial.print("Func code: "); Serial.print(data[0],HEX);
  Serial.print(" byte count: "); Serial.println(data[1],HEX);
  Serial.print("MSB: 0x");  Serial.print(data[2],HEX);
  Serial.print("  ");
  Serial.print("LSB: 0x");  Serial.print(data[3],HEX);
  Serial.print("  ");
  CO2ppmValue = ((data[2] * 0xFF ) + data[3]);
    return CO2ppmValue;
}

double checkTempElecBox() {
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

void startUp(){
    lcdDisplayStarting();
    CO2 = checkCO2();
    //checking for specific amount of CO2
    while CO2() != 0 {
        lcdDisplay(currentTemp, CO2);
        startFans();
        CO2 = checkCO2();
        currentTemp = checkTherms();
    }
}

void heating(){
    currentTemp = checkTherms();
    
    while XXX {
        if currentTemp < 125 {
            heatON();
        }
        else {
            heatOFF();
        }
      lcdDisplay(currentTemp, CO2);
      currentTemp = checkTherms();
    }
}

void removeCO2(){
    openValve();
    startPump():
    currentTemp = checkTherms();
    //holding for a specific time?
    while XXX {
        if currentTemp < 120{
            heatON();
        }
        if currentTemp > 130 {
            heatOFF();
        }
    }
    millis(); //delay
    stopPump();
    closeValve();
}

void coolDown(){
    openStepperMotors();
    currentTemp = checkTherms();
    while currentTemp > 30{
        startFans();
    }
    turnOffFans();
}


    


