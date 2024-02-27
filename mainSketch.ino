#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ADDR_6713  0x15

// create timer
long now = millis();
long lastTrigger = 0;
boolean startTimer = false;

//need to make this buttons on LED screen
const int startButton = 0;
const int stopButton = 0;
const int limitSwitch = 0;

bool startTrigger = false;

//stepper motor pins
const int DIR1 = 0;
const int STEP1 = 0;
const int DIR2 = 0;
const int STEP2 = 0;

#define motorInterfaceType 1
AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
AccelStepper motor1(motorInterfaceType, STEP2, DIR2);

//LCD display
int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

#include "functions.h"


void setup() {
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(limitSwitch, OUTPUT);

  //set stepper motors
  motor1.setMaxSpeed(1000);
  motor1.setSpeed(200);
  motor2.setMaxSpeed(1000);
  motor2.setSpeed(200);

  //set up I2C devices (needs to set up lcd, CO2, and temp sensor for inside elec box
  Wire.begin(6,7); //change to pins of lcd screen

  //set lcd screen
  lcd.init()
  lcd.backlight();

  // set interrupts
  attachInterrupt(digitalPinToInterrupt(startButton), detectStart, RISING);
  attachInterrupt(digitalPinToInterrupt(stopButton), detectStop, RISING);

  // temperature sensor things
  Serial.begin(115200);
  Wire.begin(6,7); //Join I2C Bus (NEED TO CHANGE THIS TO NOT INTERFERE WITH LCD)

  if(!sensor0.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    while(1);
  }

  Serial.println("Connected to TMP102!");
  delay(100);

  sensor0.setAlertMode(0); // Comparator Mode.

  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensor0.setConversionRate(2);

  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensor0.setExtendedMode(1);

}

void loop() {
  // put your main code here, to run repeatedly:

  switch (startTrigger) {
    case true:
      lcdDisplayStarting();
      startup();
      heating();
      removeCO2();
      coolDown();
      //need to add a function to display the removed CO2
      startTrigger = false;
  }
    
    startTimer = true;
    lastTrigger = millis();
}
  


void IRAM_ATTR detectStart(){
  startTrigger = true;
 

}

void IRAM_ATTR detectStop(){
}

