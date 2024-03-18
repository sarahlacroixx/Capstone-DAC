#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h>
#include "SparkFun_ENS160.h"
#include <esp_now.h> //ESPnow to connect to slave
#include <WiFi.h> //wifi to connect to slave
#include "RunningAverage.h"

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
      desorption(1, 1, ADS, co2Sensor2);
      coolDown(motor1, motor2, fan1, fan2, ADS, co2Sensor2);
      startTrigger = false;
      break;
    case false:
      lcdDisplayWaiting();
      break;
  }    
}
  


void IRAM_ATTR detectStart(){
  startTrigger = true;
 

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}

void cham1PinAssign(){
  //MASTER PINS
  const int miso = 20;
  const int sclk = 21;
  const int mosi = 19;
  const int cs = 18;
  const int red = 11;
  const int green = 3;
  const int yellow = 2;
  const int curSen = 10;
  const int pot = 0;

  //SLAVE PINS
  const int curSen12v = 6;
  const int fan = 7;
  const int pump 0;
  const int DIR = 1;
  const int STEP1 = 10;
  const int STEP2 = 11;
  const int ls1 = 21;
  const int ls2 20;
}
