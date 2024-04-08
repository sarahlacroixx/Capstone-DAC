#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2);
void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2);
void lcdDisplayAdsorption(int currentTemp, int currentCO2);
void lcdDisplayDesorption(int currentTemp, int currentCO2);
void lcdDisplayWaiting();
void lcdDisplayCooldown(int currentTemp, int currentCO2);
void lcdDisplayCapCO2(int finalCO2);
//void startFans(fan1, fan2);
//void turnOffFans(fan1 fan2);
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


void IRAM_ATTR detectStart();
void IRAM_ATTR checkTimer();
//pin numbers
int startButton =  0;
int stopButton =  0;
int limitSwitch = 0;

//lcd display
int lcdColumns = 20;
int lcdRows =4;

bool startTrigger = true;

//stepper motor pins
int DIR1  =28;
int STEP1  =27;
int DIR2  =26;
int STEP2  =25;
int DIR3  =24;
int STEP3  =23;
int DIR4  =20;
int STEP4  =19;
#define motorInterfacetype 1

//relay heat pins

//fan pins
int fan1 =10;
int fan2 =11;


//set up motors
AccelStepper motor1(motorInterfacetype, STEP1, DIR1);
AccelStepper motor2(motorInterfacetype, STEP2, DIR2);

//set up co2 sensor
SparkFun_ENS160 co2Sensor1;
SparkFun_ENS160 co2Sensor2;

//set up electric box sensor
TMP102 elecSensor;

//set up ADC
ADS1115 ADS(0x48);

//set up LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd
//#include "headers.h"
#include "functions.h"

//set up timer for co2
hw_timer_t *co2timer = NULL;
bool timerFlag = false;


//see setup file
void setup() {
  Serial.begin(115200);

  //set up I2C devices
  Wire.begin(6,7);
  
  //start with all pins
  //buttons and switches
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(limitSwitch, OUTPUT);

  //relay for heaters
  //pinMode(relay1, OUTPUT);

  //fans
  //pinMode(fan1, OUTPUT);
  //pintMode(fan2, OUTPUT);

  //relay for valve
  //pinMode(valveRelay, INPUT);
  

  //set stepper motors
  motor1.setMaxSpeed(1000);
  motor1.setSpeed(200);
  motor2.setMaxSpeed(1000);
  motor2.setSpeed(200);


  //set lcd screen
  lcd.init();
  lcd.backlight();

  // set interrupts
  attachInterrupt(digitalPinToInterrupt(0), detectStart, RISING);
  //attachInterrupt(digitalPinToInterrupt(stopButton), detectStop, RISING);

  //set up electrical box sensor
  if(!elecSensor.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    while(1);
  }

  Serial.println("Connected to TMP102!");
  delay(100);

  elecSensor.setAlertMode(0); // Comparator Mode.

  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  elecSensor.setConversionRate(2);

  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  elecSensor.setExtendedMode(1);

  //adc setup
  ADS.begin();
  
  //set up timer
  co2timer = timerBegin(60);
  timerAttachInterrupt(co2timer, &checkTimer);
  //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
  timerAlarm(co2timer, 9000000000, true, 0);
  //timerAlarmEnable(co2timer);

}

void loop() {

  switch (startTrigger) {
    case true:
      adsorption(motor1, motor2, fan1, fan2, ADS, co2Sensor1);
      desorption(1, 1, ADS, co2Sensor2);
      coolDown(motor1, motor2, fan1, fan2, ADS, co2Sensor2);
      startTrigger = false;
      break;
    case false:
    //lcdDisplayWaiting();
       break;
  }    
}
  


void IRAM_ATTR detectStart(){
  startTrigger = true;
 

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}
void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2){
    //change pin number depending on limit switch pin
    while (digitalRead(7) == LOW) {
        motor1.run();
        motor2.run();
    }
    
}

void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2){
    //add code here to open stepper motor
}

void lcdDisplayAdsorption(int currentTemp, int currentCO2){
  lcd.clear();
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
void lcdDisplayDesorption(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desorption Stage");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 1);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:");
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}

void lcdDisplayWaiting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void lcdDisplayCooldown(int currentTemp, int currentCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cooldown");
  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.setCursor(14, 1);
  lcd.print(currentTemp);
  lcd.setCursor(0, 1);
  lcd.print("CO2 Level:");
  lcd.setCursor(12, 1);
  lcd.print(currentCO2);
}

void lcdDisplayCapCO2(int finalCO2){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Process");
  lcd.setCursor(0, 1);
  lcd.print("Completed");
  lcd.setCursor(0, 1);
  lcd.print("CO2 Captured:");
  lcd.setCursor(14, 1);
  lcd.print(finalCO2);
}


void startFans(fan1, fan2){
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
  
}
void turnOffFans(fan1 fan2){
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
}

double checkCO2(SparkFun_ENS160 sensor){
    double CO2ppmValue = 0;
    if(sensor.checkDataStatus()) {
        CO2ppmValue = sensor.getECO2();
    }
    return CO2ppmValue;
}

float checkTempElecBox(TMP102 sensor0) {
  float temperature;

  // Turn sensor on to start temperature measurement.
  // Current consumtion typically ~10uA.
  sensor0.wakeup();

  // read temperature data
  temperature = sensor0.readTempC();
  // Place the sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  return temperature;

}
/*
void openValve(int valve){
  digitalWrite(valve, HIGH);
}

void closeValve(int valve){
  digitalWrite(valve, LOW);
}

void startPump(int pump){
  digitalWrite(pump, HIGH);
}

void stopPump(int pump){
  digitalWrite(pump, LOW);
}

void heatON(int relay){
    digitalWrite(relay, LOW);
}

void hearOFF(int relay){
    digitalWrite(relay, LOW);
}
*/
void adsorption(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2Sensor1){
    double currentCO2 = checkCO2(co2Sensor1);
    float currentTemp = checkTherms(ADS);
    double prevCO2 = 0;
    //startFans(fan1, fan2);
    //wait for 150 minutes to go by
    while (timerFlag = false) {
        lcdDisplayAdsorption(currentTemp, currentCO2);
        prevCO2 = currentCO2;
        currentCO2 = checkCO2(co2Sensor1);
        currentTemp = checkTherms(ADS);
    }
    //set timer back to false for next cycle
    timerFlag = false;
    //turnOffFans(fan1, fan2);
    stepperMotorsClose(motor1, motor2);
    
}
//function to check all of the thermistors from each adc channel and return the average value
float checkTherms(ADS1115 ADS){
    float sum = 0;
    for (int i = 0; i<=3; i++){
        //sum = sum + thermistor(ADS, 1;);
    }
    return sum/4;
}

void heating(ADS1115 ADS, SparkFun_ENS160 co2Sensor2){
    float currentTemp = checkTherms(ADS);
    double CO2 = checkCO2(co2Sensor2);
    while (currentTemp < 125) {
        //heatON();
        lcdDisplayDesorption(currentTemp, CO2);
        currentTemp = checkTherms(ADS);
        CO2 = checkCO2(co2Sensor1);
    }
}

void desorption(int valve, int pump, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    heating(ADS, co2Sensor2);
    //openValve(valve);
    //startPump(pump):
    float currentTemp = checkTherms(ADS);
    double outputtedCO2 = checkCO2(co2sensor2);
    double prevCO2 = 0;
    
    //hold until CO2 in the tank has stopped increasing
    while (outputtedCO2 > prevCO2)  {
        if (currentTemp < 120) {
            //heatON();
        }
        if (currentTemp > 130) {
            //heatOFF();
        }
        prevCO2 = outputtedCO2;
        outputtedCO2 = checkCO2(co2sensor2);
        currentTemp = checkTherms(ADS);
        lcdDisplayDesorption(currentTemp, outputtedCO2);
    }
    stopPump(pump);
    closeValve(valve);
}

void coolDown(AccelStepper motor1, AccelStepper motor2, int fan1, int fan2, ADS1115 ADS, SparkFun_ENS160 co2sensor2){
    //openStepperMotors(motor1, motor2);
    float currentTemp = checkTherms(ADS);
    double finalCO2 = checkCO2(co2sensor2);
    while (currentTemp > 30) {
        //startFans(fan1, fan2);
        lcdDisplayCooldown(currentTemp, finalCO2);
    }
    //turnOffFans(fan1, fan2);
    lcdDisplayCapCO2(finalCO2);
    delay(200);
}
