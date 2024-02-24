//place functions here
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
}

int checkCO2(){
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
