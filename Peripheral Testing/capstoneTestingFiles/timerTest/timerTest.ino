#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

void IRAM_ATTR checkTimer();
//set up timer for co2
hw_timer_t *co2timer = NULL;
bool timerFlag = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
    //set up timer
  co2timer = timerBegin(1000000);
  timerAttachInterrupt(co2timer, &checkTimer);
  //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
  timerAlarm(co2timer, 60000*1000, true, 0);
  //timerAlarmEnable(co2timer);

}

void loop() {
  // put your main code here, to run repeatedly:
  

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
  Serial.println("Timer!");
}
