/*
This code is for the "slave" esp32. This code controls the
- heaters
- fans
- stepper motors
- valve
- pump
*/

//load libraries
#include <Wire.h>
#include <AccelStepper.h> //library for stepper motors
#include <esp_now.h>
#include <WiFi.h>

//stepper motor pins
#DEFINE DIR1  28
#DEFINE STEP1  27
#DEFINE DIR2  26
#DEFINE STEP2  25
#DEFINE DIR3  24
#DEFINE STEP3  23
#DEFINE DIR4  20
#DEFINE STEP4  19
#DEFINE motorInterfacetype 1

//relay heat pins
int heat 0;

//fan pins
int fan1 10;
int fan2 11;

//pump
int pump 0;

//relay solenoid pins
int valve 0;

//set up stepper motors
AccelStepper motor1(motorInterfaceType, STEP1, DIR1);
AccelStepper motor2(motorInterfaceType, STEP2, DIR2);


//include functions from espNOW
typedef struct parameters {
    bool heat;
    bool fans;
    bool pump;
    bool valve;

} parameters;

parameters highVolt;

//FUNCTION FROM LIBRARY
//everything should happen in this function
void OnDataRecv(const esp_now_recv_info_t * mac, const uint8_t *recData, int len) {
  memcpy(&highVolt, recData, sizeof(highVolt));
  checkFans();
  checkHeat();
  checkPump();
  checkValve();
}

void setup(){
  Serial.begin(115200);
  
  // Set wifi up
  WiFi.mode(WIFI_STA);

   // init espnow
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error with init");
    return;
  }
  // callback function
  esp_now_register_recv_cb(OnDataRecv);

  //relay for heaters
  pinMode(relay1, OUTPUT);

  //fans
  pinMode(fan1, OUTPUT);
  pintMode(fan2, OUTPUT);

  //relay for valve
  pinMode(valveRelay, OUTPUT);

  //set up pump
  pinMode(pump, OUTPUT);

  
}

void loop(){
  //chill and wait for data to be received
}
void checkFans(){
  if (highVolt.fans == true){
    startFans();
  }
  else{
    stopFans();
  }
}

void checkHeat(){
  if (highVolt.heat == true){
    heatON();
  }
  else{
    heatOFF();
  }
}

void checkPump(){
  if (highVolt.pump == true){
    startPump();
  }
  else{
    stopPump();
  }
}

void checkValve(){
  if (highVolt.valve == true){
    openValve();
  }
  else{
    closeValve();
  }
}

//function to start fans
void startFans(){
  digitalWrite(fan1, HIGH);
  digitalWrite(fan2, HIGH);
}

void stopFans(){
  digitalWrite(fan1, LOW);
  digitalWrite(fan2, LOW);
}

void openValve(int valve){
  digitalWrite(valve, HIGH);
}

void closeValve(int valve){
  digitalWrite(valve, LOW);
}

void startPump(int pump){
  digitalWrite(pump, HIGH);
}

void stopPump(int pump){
  digitalWrite(pump, LOW);
}

void heatON(int relay){
    digitalWrite(relay, LOW);
}

void hearOFF(int relay){
    digitalWrite(relay, LOW);
}

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2){
    //change pin number depending on limit switch pin
    while (digitalRead(7) == LOW) {
        motor1.run();
        motor2.run();
    }
    
}

void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2){
    //add code here to open stepper motor
}
