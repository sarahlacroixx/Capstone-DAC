#include <esp_now.h>
#include <WiFi.h>


//uint8_t slaveAddress[] = {0x40, 0x4C, 0xCA, 0x45, 0x2C, 0x44}; //NEED TO UPDATE

uint8_t slaveAddress[] = {0x40, 0x4C, 0xCA, 0x44, 0xB3, 0x04};

typedef struct parameters {
    bool heat;
    bool fans;
    bool pump;
    bool valve;

} parameters;

parameters highVolt;

esp_now_peer_info_t peerInfo;

//FUNCTION FROM LIBRARY
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//function to send data to slave esp32
void sendData(bool heat, bool fans, bool pump, bool valve){
    highVolt.heat = heat;
    highVolt.fans = fans;
    highVolt.pump = pump;
    highVolt.valve = valve;
    esp_err_t state = esp_now_send(slaveAddress, (uint8_t *) &highVolt, sizeof(highVolt));

    if (state == ESP_OK) {
        Serial.println("Sent success");
    }else {
        Serial.println("Sent fail");
    }
}

void setup(){
  Serial.begin(115200);
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
  memcpy(peerInfo.peer_addr, slaveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
void loop(){
  sendData(false, true, false, false);
  delay(2000);
  sendData(false, false, false, false);


}