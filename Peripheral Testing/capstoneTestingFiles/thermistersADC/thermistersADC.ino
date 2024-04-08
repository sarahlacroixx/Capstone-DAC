
#include <Wire.h>
#include <ADS1X15.h> //library for ADC
ADS1115 ADS(0x48);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(6,7);

  ADS.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  ADS.setGain(0);
  float baseTemp = 273.15 + 25.00;
  float beta = 3984.00;
  const int16_t r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;

  float rt; //Thermistor resistance
  float reading; //Current ADC Reading value
   //read ADC

  int16_t val_0 = ADS.readADC(0);  
  //int16_t val_1 = ADS.readADC(1);  
  //int16_t val_2 = ADS.readADC(2);  
  //int16_t val_3 = ADS.readADC(3);  
 
  float f = ADS.toVoltage(2);  // voltage factor
  //Serial.println(f,5);

 //print voltage values
  Serial.print("\tAnalog0: ");
  Serial.print(val_0);
  Serial.print('\t');
  Serial.println(val_0 * f/2, 3);
  Serial.println();
  float two = 2.000;
  reading = val_0 * f/two;
  rt = (r1 * 3.3/reading)-r1;
    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(baseTemp))+((1/beta) * log(rt/r0))))-273.15;
 
  
  Serial.println(temp);
 
  delay(3000);

}
