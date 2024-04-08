#include <Wire.h>
#include <ADS1X15.h>
#include "SparkFun_ENS160.h"

SparkFun_ENS160 myENS; 

ADS1115 ADS(0x48);

float readCO2(ADS1115 ADS, int channel){
    int16_t val;
    val = ADS.readADC(0);

    /*
    switch(channel){
        case 0:
            val = ADS.readADC(0);
            break;
        case 1:
            val = ADS.readADC(1);
            break;   
    }
    */
    float f = ADS.toVoltage(2);
    float reading = val * f/2;
    Serial.println(reading);

  //convert voltage to ppm
    float CO2Value = reading*500.00;
    //map(reading, 0.000, 4.000, 0.000, 2000.000);
    return CO2Value;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(6,7);
  ADS.begin();
 
}

void loop() {
  float curr = readCO2(ADS, 0);
  //int co = myENS.getECO2();
  Serial.print("New Co2: ");
  Serial.println(curr);
  //Serial.print("Old CO2: ");
  //Serial.println(co);
  delay(2000);

}
