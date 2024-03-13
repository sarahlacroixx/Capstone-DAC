void displayClock(unsigned long StartTime, int spot){
  lcd.setCursor(0,spot);
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
  lcd.setCursor(12,spot);
  if (hour < 10){
    lcd.print("0");
    lcd.setCursor(13,spot);
    lcd.print(hour);
  } else {
    lcd.print(hour);
  }
  
  lcd.setCursor(14,spot);
  lcd.print(":");
  lcd.setCursor(15, spot);

  if (min < 10){
    lcd.print("0");
    lcd.setCursor(16,spot);
    lcd.print(min);
  } else {
    lcd.print(min);
  }
  lcd.setCursor(17,spot);
  lcd.print(":");
  lcd.setCursor(18, spot);

  if (sec < 10){
    lcd.print("0");
    lcd.setCursor(19,spot);
    lcd.print(sec);
  } else {
    lcd.print(sec);
  }
  
}
