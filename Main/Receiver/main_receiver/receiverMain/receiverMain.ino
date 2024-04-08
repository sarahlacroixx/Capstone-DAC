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
#include <ESP_Google_Sheet_Client.h> // logging data
#include <GS_SDHelper.h>

esp_now_peer_info_t peerInfo;
uint8_t slaveAddress[] = {0x40, 0x4C, 0xCA, 0x44, 0xA7, 0x58};

unsigned long StartTime = millis();

  const int curSen12v = 6;
  const int fanPin = 7;
  const int pumpPin = 0;
  const int DIR = 1;
  const int STEP1 = 10;
  const int STEP2 = 11;
  const int ls1 = 18;
  const int ls2 = 19;
  const int valvePin = 22;
  const int heatPin = 23;
  const int heatPin2 = 16;

  AccelStepper motor1(1, STEP1, DIR);
  AccelStepper motor2(1, STEP2, DIR);


// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "";

//include functions from espNOW
typedef struct parameters {
    int pinAssign;
    bool heat;
    bool fans;
    bool pump;
    bool valve;
    int motors;

} parameters;

parameters highVolt;

//when data is received it will run through this function
void OnDataRecv(const esp_now_recv_info_t * mac, const uint8_t *recData, int len) {
  memcpy(&highVolt, recData, sizeof(highVolt));
  checkPins();
  checkFans();
  checkHeat();
  checkPump();
  checkValve();
  checkMotors();
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
  

  pinMode(fanPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(heatPin2, OUTPUT);
  pinMode(ls1, INPUT);
  pinMode(ls2, INPUT);

  motor1.setMaxSpeed(400);
  motor1.setSpeed(-150);
  
  motor2.setMaxSpeed(400);
  motor2.setSpeed(150);

  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

      WiFi.setAutoReconnect(true);
  //replace text below with wifi username and password
    WiFi.begin("Enter ID", "Enter Password here");

  //function from library
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    GSheet.setPrerefreshSeconds(10 * 60);

    // replace the information below with the appropriate info
    GSheet.begin("replace with email of google service account", "replace with project ID", "replace with the private ID of the google service account");



  //THESE ARE FROM LIBRARY (NOT MINE)
  // Register peer
  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);


      //relay for valve
  pinMode(valvePin, OUTPUT);

  //set up pump
  pinMode(pumpPin, OUTPUT);
}

void loop(){
  dataLogging();
  delay(1000);

}

void checkPins(){
  /*
    switch (highVolt.motors){
        case 1:
            cham1PinAssign();
            break;
        case 2:
            cham2PinAssign();
            break;
        case 3:
            break;
    }
    */
}

void checkFans(){
  if (highVolt.fans == true){
    Serial.println("Starting fans");
    startFans();
  }
  else{
    stopFans();
  }
}

void checkHeat(){
  if (highVolt.heat == true){
    Serial.println("Heat");
    heatON(heatPin);
  }
  else{
    heatOFF(heatPin);
  }
}

void checkPump(){
  if (highVolt.pump == true){
    Serial.println("pump on");
    startPump(pumpPin);
  }
  else{
    stopPump(pumpPin);
  }
}

void checkValve(){
  if (highVolt.valve == true){
    Serial.println("valve on");
    openValve(valvePin);
  }
  else{
    closeValve(valvePin);
  }
}

void checkMotors(){
    //0 is do nothing
    //1 is close stepper motors
    //2 is open stepper motors
    switch (highVolt.motors){
        case 1:
          Serial.println("running motors1");
            stepperMotorsClose(motor1, motor2);
            break;
        case 2:
            stepperMotorsOpen(motor1, motor2);
            break;
        case 3:
            break;
    }
}

//function to start fans
void startFans(){
  digitalWrite(fanPin, HIGH);
}

void stopFans(){
  digitalWrite(fanPin, LOW);
}

void openValve(int valvePin){
  digitalWrite(valvePin, HIGH);
}

void closeValve(int valvePin){
  digitalWrite(valvePin, LOW);
}

void startPump(int pumpPin){
  digitalWrite(pumpPin, HIGH);
}

void stopPump(int pumpPin){
  digitalWrite(pumpPin, LOW);
}

void heatON(int heatPin){
    Serial.println("starting heat");
    digitalWrite(heatPin, HIGH);
    digitalWrite(heatPin2, HIGH);
}

void heatOFF(int heatPin){
    digitalWrite(heatPin, LOW);
    digitalWrite(heatPin2, LOW);
}

void stepperMotorsClose(AccelStepper motor1, AccelStepper motor2){
    //change pin number depending on limit switch pin
    
    while (digitalRead(ls1) == LOW) {
        motor1.runSpeed();
    }
    while (digitalRead(ls2) == LOW) {
        motor2.runSpeed();
    }
    
    
}

void stepperMotorsOpen(AccelStepper motor1, AccelStepper motor2){
  
  motor1.moveTo(100);
  motor1.run();

  motor2.moveTo(-100);
  motor2.run();

  /*
  motor2.moveTo(100);

  
  motor2.run();
  */
    
}
//Pin assignments for chamber 1
void cham1PinAssign(){
  //SLAVE PINS for chamber 1
  
  const int curSen12v = 6;
  const int fanPin = 7;
  const int pumpPin = 0;
  const int DIR = 1;
  const int STEP1 = 10;
  const int STEP2 = 11;
  const int ls1 = 18;
  const int ls2 = 19;
  const int valvePin = 22;
  const int heatPin = 23;
    AccelStepper motor1(1, STEP1, DIR);
    AccelStepper motor2(1, STEP2, DIR);
 //relay for heaters
  pinMode(heatPin, OUTPUT);

  //fans
  pinMode(fanPin, OUTPUT);
  //relay for valve
  pinMode(valvePin, OUTPUT);

  //set up pump
  pinMode(pumpPin, OUTPUT);

  pinMode(ls1, INPUT);
  pinMode(ls2, INPUT);
  
  
}

//pin assignments for chamber 2
void cham2PinAssign(){
  //SLAVE PINS for chamber 2
  const int curSen12v = 6;
  const int fanPin = 13;
  const int pumpPin = 0;
  const int DIR = 1;
  const int STEP1 = 2;
  const int STEP2 = 3;
  const int ls1 = 19;
  const int ls2 = 18;
  const int valvePin = 12;
  //const int heatPin = TXD; // NEED TO FIX THIS
    AccelStepper motor1(1, STEP1, DIR);
    AccelStepper motor2(1, STEP2, DIR);
    
//relay for heaters
  pinMode(heatPin, OUTPUT);

  //fans
  pinMode(fanPin, OUTPUT);
  //relay for valve
  pinMode(valvePin, OUTPUT);

  //set up pump
  pinMode(pumpPin, OUTPUT);
  pinMode(ls1, INPUT);
  pinMode(ls2, INPUT);
  
}
