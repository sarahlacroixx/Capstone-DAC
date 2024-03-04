//place headers here

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <Arduino.h>

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2);
void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2);
void lcdDisplayAdsorption(int currentTemp, int currentCO2);
void lcdDisplayDesorption(int currentTemp, int currentCO2);
void lcdDisplayWaiting();
void lcdDisplayCooldown(int currentTemp, int currentCO2);
void lcdDisplayCapCO2(int finalCO2);
void startFans(fan1, fan2);
void turnOffFans(fan1 fan2);
double checkCO2(SparkFun_ENS160 sensor);
double checkTempElecBox(TMP102 sensor0);
void openValve(int valve);
void closeValve(int valve);
void startPump(int pump);
void stopPump(int pump);
void heatON(int relay);
void hearOFF(int relay);
void adsorption(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2Sensor1);
float checkTherms(ADS1115 ADS);
void heating(ADS1115 ADS, SparkFun_ENS160 co2Sensor2);
void desorption(int valve, int pump, ADS1115 ADS, SparkFun_ENS160 co2sensor2);
void coolDown(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2sensor2);

#endif
