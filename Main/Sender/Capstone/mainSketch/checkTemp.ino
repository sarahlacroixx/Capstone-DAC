float thermister(ADS1115 ADS, int channel){
 ADS.setGain(0);

  float baseTemp = 273.15 + 25.00;
  float beta = 3984.00;
  float r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;

  float rt; //Thermistor resistance
  float reading; //Current ADC Reading value
    int16_t val;
   //read ADC
   val = ADS.readADC(channel);

 
  float f = ADS.toVoltage(2);  // voltage factor
  float two = 2.000;
  reading = val * f/two;          
    
  //Voltage divider equation to solve for thermistor resistance
  rt = (r1 * 3.3/reading)-r1;
    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(baseTemp))+((1/beta) * log(rt/r0))))-273.15;
  return temp;

}
/*
//function to check all of the thermistors from each adc channel and return the average value
float checkTherms(ADS1115 ADS){
    float sum = 0;
    for (int i = 0; i<=1; i++){
        sum = sum + thermister(ADS, i);
    }
    return sum/2;
}
*/
