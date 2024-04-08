#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h> //library for elec box sensor
#include "SparkFun_ENS160.h"

SparkFun_ENS160 myENS; 

int checkCO2(SparkFun_ENS160 myENS);

int static_variable = 440;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(6,7);
  Serial.begin(115200);

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

}

void loop() {
  // put your main code here, to run repeatedly:
  myENS.setOperatingMode(SFE_ENS160_STANDARD);
  int co;
  co = checkCO2(myENS);
  Serial.print("Variable_1:");
  Serial.print(co);
  Serial.print(",");
  Serial.print("Variable_2:");
  Serial.println(static_variable);
  delay(100);
  

}

int checkCO2(SparkFun_ENS160 myENS){
  int sum = 0;
  int count = 10;

  for (int i = 1; i <= 10; i++){
    int currCO2 = myENS.getECO2();
    if (currCO2 != 0) {
      sum = sum + myENS.getECO2();
      delay(200);
    }
    else{
      count = count - 1;
    }
    ///Serial.print("Value: ");
    //Serial.print(myENS.getECO2());
  }
  int total = sum/count;

  return total;
}