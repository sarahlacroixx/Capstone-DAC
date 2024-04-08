#include <Wire.h>
#include <LiquidCrystal_I2C.h> //library for LCD
#include <ADS1X15.h> //library for ADC
#include <AccelStepper.h> //library for stepper motors
#include <SparkFunTMP102.h>
#include <esp_now.h> //ESPnow to connect to slave
#include <WiFi.h> //wifi to connect to slave
#include <ESP_Google_Sheet_Client.h> // logging data
#include <GS_SDHelper.h>
#include <Math.h>

int current12V;

//prototype functions
void coolDown(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor);
void displayWaiting();
void lcdDisplayWithPot(char display[], int chambTemp, int elecTemp, int inCo2, int outCo2, int flowCo2);
void displayChambTemp(int chambTemp, int spot);
void displayChar(char display[], int spot);
void displayInCo2(int inCo2, int spot);
void displayOutCo2(int outCo2, int spot);
void displayElecTemp(int elecTemp, int spot);
void displayFlowCo2(int flowCo2, int spot);

//start/stop buttons for chamber
const int start1 = 23;
const int start2 = 17;
const int stop = 22;

//MASTER PINS
const int miso = 20;
const int sclk = 21;
const int mosi = 19;
const int cs = 18;
const int red = 11;
const int green = 3;
const int yellow = 2;
const int curSen = 10;
const int pot = 0;
const int lcdColumns = 20;
const int lcdRows = 4;

bool startTrigger = false;

void IRAM_ATTR detectStart1();
void IRAM_ATTR detectStop();
void(* resetFunc) (void) = 0;

unsigned long StartTime = millis();

//set up electric box sensor
TMP102 elecSensor;

//set up ADC
ADS1115 ADS(0x48);
ADS1115 ADS2(0x49);

//set up LCD
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); //0x27 is address for lcd

//set up timer for co2
hw_timer_t *Co2timer = NULL;
bool timerFlag = false;

hw_timer_t *newtimer = NULL;
bool timerFlag2 = false;

uint8_t slaveAddress[] = {0x40, 0x4C, 0xCA, 0x44, 0xB3, 0x04};

typedef struct parameters {
    int pinAssign;
    bool heat;
    bool fans;
    bool pump;
    bool valve;
    int motors;
} parameters;

parameters highVolt;

typedef struct currSensor {
  int currSen;
} currSensor;

currSensor sendCurrent;

esp_now_peer_info_t peerInfo;

void IRAM_ATTR checkTimer();

// The ID of the spreadsheet
const char spreadsheetId[] = "";


int count = 0;

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

  //THESE ARE FROM LIBRARY
  // Register peer
  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  //start with all pins
  //buttons and switches
  pinMode(start1, INPUT);
  pinMode(start2, INPUT);
  pinMode(stop, INPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  

  //set lcd screen
  lcd.init();
  lcd.backlight();

  // set interrupts
  attachInterrupt(digitalPinToInterrupt(start1), detectStart1, RISING);
  attachInterrupt(digitalPinToInterrupt(start2), detectStart2, RISING);
  attachInterrupt(digitalPinToInterrupt(stop), detectStop, RISING);


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
  //2 = 4Hz
  elecSensor.setConversionRate(2);
  //Temperature(-55C to +150C)
  elecSensor.setExtendedMode(1);

  //adc setup
  ADS.begin();
  ADS2.begin();
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);

  //setting up the data logging with google sheets
  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

    // Connect to Wi-Fi
    WiFi.setAutoReconnect(true);
  //replace text below with wifi username and password
    WiFi.begin("Enter ID", "Enter Password here");
  
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(red, HIGH);
      lcd.setCursor(0,0);
      lcd.print("Connecting to Wifi");
      Serial.print(".");
      delay(1000);
    }
    lcd.clear();
    digitalWrite(red, LOW);
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // replace the information below with the appropriate info
    GSheet.begin("replace with email of google service account", "replace with project ID", "replace with the private ID of the google service account");

  //setting up SD card
  SPIClass SPI2(SPI);
  SPI2.begin(SCLK, MISO, MOSI, CS);

  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS, SPI2)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  WriteFile(SD, "/co2Data.txt", "Time, Inlet CO2, Outlet CO2 \r\n");
  WriteFile(SD, "/tempData.txt", "Time, Inlet CO2, Outlet CO2 \r\n");

}


typedef struct currParameters{
    float inCo2;
    float outCo2;
    float centCurrentTemp;
    float sideCurrentTemp;
    float elecTemp;
    int flowCo2;
} currParameters;

currParameters curr;

void checkParameters(ADS1115 ADS, ADS1115 ADS2, TMP102 elecSensor){
    curr.inCo2 = readCO2(ADS2, 0); // NEED TO FIX CHANNEL HERE
    curr.outCo2 = readCO2(ADS2, 1); // NEED TO FIX CHANNEL HERE
    curr.centCurrentTemp = thermister(ADS, 0);
    curr.sideCurrentTemp = thermister(ADS, 1);
    curr.elecTemp = checkTempElecBox(elecSensor);
    curr.flowCo2 = checkFlowCo2();
}


void loop() {
  switch (startTrigger) {
    case true:
      lcd.clear();
      digitalWrite(yellow, LOW);
      digitalWrite(green, HIGH);
      adsorption(ADS,ADS2, elecSensor);
      desorption(ADS,ADS2, elecSensor);
      coolDown(ADS, ADS2, elecSensor);
      startTrigger = false;
      lcd.clear();
      break;
    case false:
      digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);
      displayWaiting();
      sendData(3, false, false, false, false, 3);
      break;
  }  
}
  
void IRAM_ATTR detectStart1(){
  sendData(1, false, false, false, false, 0);
  StartTime = millis();
  startTrigger = true;
}

void IRAM_ATTR detectStart2(){
  sendData(2, false, false, false, false, 0);
  StartTime = millis();
  startTrigger = true;
}

void IRAM_ATTR detectStop(){
  sendData(3, false, false, false, false, 0);
  resetFunc();
  startTrigger = false;

}

void IRAM_ATTR checkTimer(){
  timerFlag = true;
}


