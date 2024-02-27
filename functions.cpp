//place functions here
#include "functions.h"
//thermister things

float R1 = 10000;
float logR2, R2, T;
//these values need to be fixed
float c1 = 1.009249522e-03;
c2 = 2.378405444e-04;
c3 = 2.019202697e-07;

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

int checkCO2(){
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
}

int checkCO2inTank(){
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
    
}

int checkTempElecBox() {
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

int checkTherms() {
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads1015.readADC_SingleEnded(0);
  adc1 = ads1015.readADC_SingleEnded(1);
  adc2 = ads1015.readADC_SingleEnded(2);
  adc3 = ads1015.readADC_SingleEnded(3);

  //test this out to see if it works then copy paste for the rest
  R2 = R1 * (1023.0 / (float)adc0 - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15; //in farenheight
  T = (T * 9.0)/ 5.0 + 32.0; 

}

void openValve(){
  digitalWrite(valve, HIGH);
}

void closeValve(){
  digitalWrite(valve, LOW);
}

void startPump(){
  digitalWrite(pump, HIGH);
}

void stopPump(){
  digitalWrite(pump, LOW);
}

void startUp(){
    while CO2() != 0 {
      startFans();
      CO2 = checkCO2();
    }
}

void 


