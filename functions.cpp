//place functions here
#include "functions.h"

void stepperMotorsClose(){
  if (motor1.distanceToGo() == 0 && motor2.distanceToGo() == 0) {
      motor1.moveTo(-motor1.currentPosition());
      motor2.moveTo(-motor2.currentPosition());
    }
    motor.run();
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
  

}

void openValve(){
}

void turnPumpOn(){
}



