void setup() {
  Serial.begin(115200);
  //set up I2C devices
  Wire.begin(6,7);

  //set up wifi
  WiFi.mode(WIFI_STA);
  //initialize espnow
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  //THESE ARE FROM LIBRARY (NOT MINE)
  // Register peer
  memcpy(otherInfo.peer_addr, address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


  
  //start with all pins
  //buttons and switches
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(limitSwitch, OUTPUT);

  //relay for heaters
  //pinMode(relay1, OUTPUT);

  //fans
  //pinMode(fan1, OUTPUT);
  //pintMode(fan2, OUTPUT);

  //relay for valve
  //pinMode(valveRelay, INPUT);
  

  //set stepper motors
  motor1.setMaxSpeed(1000);
  motor1.setSpeed(200);
  motor2.setMaxSpeed(1000);
  motor2.setSpeed(200);


  //set lcd screen
  lcd.init();
  lcd.backlight();

  // set interrupts
  attachInterrupt(digitalPinToInterrupt(0), detectStart, RISING);
  //attachInterrupt(digitalPinToInterrupt(stopButton), detectStop, RISING);

  //set up electrical box sensor
  if(!elecSensor.begin())
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

  //adc setup
  ADS.begin();
  
  //set up timer
  co2timer = timerBegin(60);
  timerAttachInterrupt(co2timer, &checkTimer);
  //pass through 9000000000 milliseconds to wait for 150 minutes to see an inturrupt
  timerAlarm(co2timer, 9000000000, true, 0);
  //timerAlarmEnable(co2timer);

}
