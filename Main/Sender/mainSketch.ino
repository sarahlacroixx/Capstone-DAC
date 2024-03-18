#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h>
#include "SparkFun_ENS160.h"
#include <esp_now.h> //ESPnow to connect to slave
#include <WiFi.h> //wifi to connect to slave

//start/stop buttons for chamber
const int start1 = 23;
const int start2 = 1;
const int stop = 22;

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
const int ledColumns 20
const int ledRows 4

//set up electric box sensor
TMP102 elecSensor;

//set up ADC
ADS1115 ADS(0x48);

//set up LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

//set up timer for co2
hw_timer_t *Co2timer = NULL;
bool timerFlag = false;


//see setup file

void loop() {

  switch (startTrigger) {
    case true:
      adsorption(ADS, elecSensor);
      desorption(ADS, elecSensor);
      coolDown(ADS, elecSensor);
      startTrigger = false;
      break;
    case false:
      lcdDisplayWaiting();
      break;
  }    
}
  


void IRAM_ATTR detectStart1(){
  sendData(1, false, false, false, false, 0);
  unsigned long StartTime = millis();
  startTrigger = true;

}

void IRAM_ATTR detectStart2(){
  sendData(2, false, false, false, false, 0);
  unsigned long StartTime = millis();
  startTrigger = true;

}

void IRAM_ATTR detectStop(){
  sendData(0, false, false, false, false, 0);
  unsigned long StartTime = 0;
  startTrigger = false;

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}




