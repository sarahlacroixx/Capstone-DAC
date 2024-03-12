uint8_t slaveAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0xAE, 0x7C}; //NEED TO UPDATE

typedef struct parameters {
    bool heat;
    bool fans;
    bool pump;
    bool valve;

} parameters;

parameters highVolt;

esp_now_peer_info_t otherInfo;

//FUNCTION FROM LIBRARY
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//function to send data to slave esp32
void sendData(bool heat, bool fans, bool pump, bool valve){
    highVolt.heat = heat;
    highVolt.fans = fan;
    highVolt.pump = pump;
    highVolt.valve = valve;
    esp_err_t state = esp_now_send(slaveAddress, (uint8_t *) &highVolt, sizeof(highVolt));

    if (state == ESP_OK) {
        Serial.println("Sent success");
    }else {
        Serial.println("Sent fail");
    }
}
