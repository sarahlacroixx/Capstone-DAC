void displayWaiting(){
  lcd.setCursor(0,0);
  lcd.print("Waiting...");
  lcd.setCursor(0,1);
  lcd.print("Press Start on");
  lcd.setCursor(0,2);
  lcd.print("Chamber 1 or 2");
  lcd.setCursor(0,3);
  lcd.print("To Begin");
}
