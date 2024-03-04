#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h>
#include "SparkFun_ENS160.h"

//set up motors
AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
AccelStepper motor2(motorInterfaceType, STEP2, DIR2);

//set up co2 sensor
SparkFun_ENS160 co2Sensor1;
SparkFun_ENS160 co2Sensor2;

//set up electric box sensor
TMP102 elecSensor;

//set up ADC
ADS1115 ADS(0x48);

//set up LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd
#include "headers.h"
#include "functions.h"

//set up timer for co2
hw_timer_t *Co2timer = NULL;
bool timerFlag = false;


//see setup file

void loop() {

  switch (startTrigger) {
    case true:
      adsorption(motor1, motor2, fan1, fan2, ADS, co2Sensor1);
      desorption(ADS, co2sensor2);
      coolDown(motor1, motor2, fan1, fan2, ADS, co2sensor2);
      startTrigger = false;
  }
  case false:
    lcdDisplayWaiting();
}
  


void IRAM_ATTR detectStart(){
  startTrigger = true;
 

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}

