#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors

AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
AccelStepper motor1(motorInterfaceType, STEP2, DIR2);

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd
#include "headers.h"
#include "functions.h"

hw_timer_t *Co2timer = NULL;

bool timerFlag = false;
//see setup file

void loop() {
  // put your main code here, to run repeatedly:

  switch (startTrigger) {
    case true:
      startup();
      heating();
      removeCO2();
      coolDown();
      //need to add a function to display the removed CO2
      startTrigger = false;
  }
  case false:
    lcdDisplayWaiting();
    
    startTimer = true;
    lastTrigger = millis();
}
  


void IRAM_ATTR detectStart(){
  startTrigger = true;
 

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}

