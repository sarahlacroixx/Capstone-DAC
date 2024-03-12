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
