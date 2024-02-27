#ifndef THERMISTOR_H
#define THERMISTOR_H

class Thermistor{

private:
int* pin;
double* beta;

public:
Thermistor(int pin[], double beta[]);
double thermistorRead();
};

#endif
