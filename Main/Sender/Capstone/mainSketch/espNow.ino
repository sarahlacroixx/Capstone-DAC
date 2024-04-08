

//FUNCTION FROM LIBRARY
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//function to send data to slave esp32
void sendData(int pinAssign, bool heat, bool fans, bool pump, bool valve, int motors){
    highVolt.pinAssign = pinAssign;
    highVolt.heat = heat;
    highVolt.fans = fans;
    highVolt.pump = pump;
    highVolt.valve = valve;
    highVolt.motors = motors;
    esp_err_t state = esp_now_send(slaveAddress, (uint8_t *) &highVolt, sizeof(highVolt));

    if (state == ESP_OK) {
        Serial.println("Sent success");
    }else {
        Serial.println("Sent fail");
    }
}
