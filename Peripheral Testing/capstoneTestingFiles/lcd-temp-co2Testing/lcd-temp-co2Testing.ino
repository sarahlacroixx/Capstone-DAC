#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

unsigned long StartTime = millis();


//lcd display
int lcdColumns = 20;
int lcdRows =4;
int static_variable = 500;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

ADS1115 ADS(0x49);


SparkFun_ENS160 myENS; 

int ensStatus; 

void lcdDisplayAdsorption(int currentTemp, int currentCO2);
int checkCO2(SparkFun_ENS160 myENS);
float checkTemp(ADS1115 ADS);
//int timer(unsigned long StartTime);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(6,7);

	Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	// Device needs to be set to idle to apply any settings.
	// myENS.setOperatingMode(SFE_ENS160_IDLE);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

  ADS.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  //int sec = timer(StartTime);
  int co;
  co = checkCO2(myENS);
  Serial.print("Variable_1:");
  Serial.print(co);
  Serial.print(",");
  Serial.print("Variable_2:");
  Serial.println(static_variable);
  float temp = checkTemp(ADS);
  lcdDisplayAdsorption(temp, co, ElapsedTime/1000);
  delay(200);

}

void lcdDisplayAdsorption(int currentTemp, int currentCO2, int time){
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
  lcd.setCursor(0,3);
  lcd.print("Elapsed Time: ");
  lcd.setCursor(15,3);
  lcd.print(time);
}

int checkCO2(SparkFun_ENS160 myENS){
  int co2 = myENS.getECO2();
  return co2;
}

float checkTemp(ADS1115 ADS){
    // put your main code here, to run repeatedly:
  ADS.setGain(0);

  float baseTemp = 273.15 + 25.00;
  float beta = 3984.00;
  const int8_t numReadings = 4; //The number of readings to average
  const int16_t r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;

  float rt; //Thermistor resistance
  float reading; //Current ADC Reading value
   //read ADC

  int16_t val_0 = ADS.readADC(0);  
  int16_t val_1 = ADS.readADC(1);  
  int16_t val_2 = ADS.readADC(2);  
  int16_t val_3 = ADS.readADC(3);  
 
  float f = ADS.toVoltage(2);  // voltage factor
  //Serial.println(f,5);
  Serial.print("val_0: ");
  Serial.println(val_0);

  float two = 2.000;
  reading = val_0 * f/two;
  Serial.print("voltage: ");

  //reading = reading + 0.271;
  Serial.println(reading);
    
  //Voltage divider equation to solve for thermistor resistance
  rt = (r1 * 3.3/reading)-r1;
  Serial.print("rt: ");
  Serial.println(rt);

    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(baseTemp))+((1/beta) * log(rt/r0))))-273.15;
  return temp;
}
/*
int timerSec(unsigned long StartTime){
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;

  int sec = ElapsedTime/1000;
  int min = sec/60;

  return sec;
}
*/