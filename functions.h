//place headers here

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

void stepperMotorsClose();
void lcdDisplay(int currentTemp, int currentCO2);
void lcdDisplayStarting();
void startFans();
int checkCO2();
int checkTempElecBox();
int checkTherms();
int checkTherms();
void openValve();
void turnPumpOn();

#endif
