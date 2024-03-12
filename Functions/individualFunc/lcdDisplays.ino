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

//THIS FUNCTION HAS BEEN MOVED TO newLcdDisplay 
void lcdDisplayWithPot(int pot, char display, int chambTemp, int elecTemp, int inCo2, int outCo2, int flowCo2){
  value = map(pot, 0, 4, 0, 1023);
  switch(value){
    case 0:
      displayChar(display, 0);
      displayChambTemp(chambTemp, 1);
      displayInCo2(inCo2, 2);
      displayOutCo2(outCo2, 3):
    case 1:
      displayChambTemp(chambTemp, 0);
      displayInCo2(inCo2, 1);
      displayOutCo2(outCo2, 2):
      displayClock(StartTime, 3);
    case 2:
      displayInCo2(inCo2, 0);
      displayOutCo2(outCo2, 1):
      displayClock(StartTime, 2);
      displayElecTemp(elecTemp, 3);
    case 3:
      displayOutCo2(outCo2, 0):
      displayClock(StartTime, 1);
      displayElecTemp(elecTemp, 2);
      displayFlowCo2(flowCo2, 3);
  }
  
}
void displayChambTemp(int chambTemp, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Temperature: ");
      lcd.setCursor(14, spot);
      lcd.print(chambTemp);
}

void displayChar(char display, int spot){
      lcd.setCursor(0, spot);
      lcd.print(display);
}

void displayInCo2(int inCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Inlet CO2: ");  
      lcd.setCursor(12, spot);
      lcd.print(inCo2);
}

void displayOutCo2(int outCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Outlet CO2: ");  
      lcd.setCursor(13, spot);
      lcd.print(outCo2);
}

void displayElecTemp(int elecTemp, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Elec. Temp: ");
      lcd.setCursor(13, spot);
      lcd.print(elecTemp);
}

void displayFlowCo2(int flowCo2, int spot){
      lcd.setCursor(0, spot);
      lcd.print("Exiting CO2: ");  
      lcd.setCursor(14, spot);
      lcd.print(flowCo2);
}


