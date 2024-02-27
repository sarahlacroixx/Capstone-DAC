void Thermistor::Thermistor(int[] pin, double[] beta){
  this->pin = pin;
  this->beta = beta;
}

float Thermistor::thermistorRead(){
  const int8_t baseTemp = 25;
  const int8_t numReadings = 5; //The number of readings to average
  const int16_t r0 = 10000; //Thermistor resistance value at 25 degrees Celsius
  const int16_t r1 = 10000; //Fixed Resistor value in the voltage divider circuit
  float temp;
  float sum;
  float rt; //Thermistor resistance
  int16_t reading; //Current ADC Reading value
  

  //Loop to Read the pin a certain amount of times and average the value
  for(int i = 0; i < numReadings; i++){
 reading = analogRead(_pin);
    
  //A reading of zero will cause a divide by 0 error
  if(reading == 0){
    reading = 1;
  }
    
  //Voltage divider equation to solve for thermistor resistance
  rt = r1 * (4095.00/reading-1.00);
    
  //Equation to convert thermistor resistance to temperature
  temp = (1/((1/(273.15+baseTemp))+(1/_beta) * log(rt/r0)))-273.15;
  sum += temp;
  }
  return sum/numReadings;
}
