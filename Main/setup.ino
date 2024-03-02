void setup() {
  //start with all pins
  //buttons and switches
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(limitSwitch, OUTPUT);

  //relay for heaters
  pinMode(relay1, OUTPUT);

  //fans
  pinMode(fan1, OUTPUT);
  pintMode(fan2, OUTPUT);

  //relay for valve
  

  //set stepper motors
  motor1.setMaxSpeed(1000);
  motor1.setSpeed(200);
  motor2.setMaxSpeed(1000);
  motor2.setSpeed(200);

  //set up I2C devices (needs to set up lcd, CO2, and temp sensor for inside elec box
  Wire.begin(6,7); //change to pins of lcd screen

  //set lcd screen
  lcd.init()
  lcd.backlight();

  // set interrupts
  attachInterrupt(digitalPinToInterrupt(startButton), detectStart, RISING);
  attachInterrupt(digitalPinToInterrupt(stopButton), detectStop, RISING);

  // temperature sensor things
  Serial.begin(115200);
  Wire.begin(6,7); //Join I2C Bus (NEED TO CHANGE THIS TO NOT INTERFERE WITH LCD)

  if(!sensor0.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    while(1);
  }

  Serial.println("Connected to TMP102!");
  delay(100);

  sensor0.setAlertMode(0); // Comparator Mode.

  // set the Conversion Rate (how quickly the sensor gets a new reading)
  //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
  sensor0.setConversionRate(2);

  //set Extended Mode.
  //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
  sensor0.setExtendedMode(1);

  //adc setup
  ADS.begin();

}
