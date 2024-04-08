#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

unsigned long StartTime = millis();

void lcdDisplayAdsorption(int currentTemp, int currentCO2);
void clock(unsigned long StartTime);


//lcd display
int lcdColumns = 20;
int lcdRows =4;
int static_variable = 500;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

void setup() {
  Wire.begin(6,7);
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  lcdDisplayAdsorption(20, 20);
  clock(StartTime);

}


void lcdDisplayAdsorption(int currentTemp, int currentCO2){
  //lcd.clear();
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
  lcd.setCursor(0,3);
}

void clock(unsigned long StartTime){
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
  /*
  if (min >= 60) {
    hour++;
    min = min - (60)*hour;
  }
*/
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



