#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h>

TMP102 elecSensor;

float checkTempElecBox(TMP102 sensor0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //set up I2C devices
  Wire.begin(6,7);

  //set up electrical box sensor
  if(!elecSensor.begin(0x4A,Wire))
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

}

void loop() {
  float temp;
  temp = checkTempElecBox(elecSensor);
  Serial.println(temp);
  delay(100);

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
