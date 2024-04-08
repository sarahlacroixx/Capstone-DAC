#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

//lcd display
int lcdColumns = 20;
int lcdRows =4;


LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

void lcdDisplayAdsorption(int currentTemp, int currentCO2);



void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

  //set up I2C devices
  Wire.begin(6,7);

  lcd.init();
  lcd.backlight();
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Display");
  lcdDisplayAdsorption(25, 400);

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
}

