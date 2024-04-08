#include <esp_now.h>
#include <WiFi.h>
//need to create a loop where it just checks the data received
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


//function to start fans
void startFans(){
  Serial.println("Fans On");
}

void stopFans(){
  Serial.println("Fans Off");
}