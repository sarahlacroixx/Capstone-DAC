#include "RunningAverage.h"


RunningAverage myRA(10);
int samples = 0;

uint32_t start, stop;


void setup(void) 
{
  Serial.begin(115200);
}

void loop(void) 
{
  long rn = random(90, 100);
  myRA.addValue(rn);
  samples++;
  Serial.print("Running Average: ");
  Serial.println(myRA.getAverage(), 4);
  
  if (samples == 300)
  {
    samples = 0;
    myRA.fillValue(100, 10);
  }
  delay(100);
}

