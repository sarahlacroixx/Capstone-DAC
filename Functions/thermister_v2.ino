float thermistor() {
  const int8_t baseTemp = 25;
  const int8_t numReadings = 4; //The number of readings to average
  const int16_t r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;
  float sum;
  float rt; //Thermistor resistance
  int16_t reading; //Current ADC Reading value

  ADS.setGain(0);

  //read ADC
  int16_t val[0] = ADS.readADC(0);  
  int16_t val[1] = ADS.readADC(1);  
  int16_t val[2] = ADS.readADC(2);  
  int16_t val[3] = ADS.readADC(3);  
 
  float f = ADS.toVoltage(2);

  //Loop to Read the pin a certain amount of times and average the value
  for(int i = 0; i < numReadings; i++){
    
  //A reading of zero will cause a divide by 0 error
  if(val[i] == 0){
    val[i] = 1;
  }
    
  //Voltage divider equation to solve for thermistor resistance
  rt = r1 * (4095.00/val[i]-1.00);
    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(273.15+baseTemp))+(1/_beta) * log(rt/r0)))-273.15;
  sum += temp;
  }
  return sum/numReadings;
}
