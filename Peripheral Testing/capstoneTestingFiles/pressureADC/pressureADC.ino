
#include <Wire.h>
#include <ADS1X15.h> //library for ADC

#include <WiFi.h> //wifi to connect to slave
#include <ESP_Google_Sheet_Client.h> // logging data
#include <GS_SDHelper.h>

#define WIFI_SSID "The View (2)"
#define WIFI_PASSWORD "sarah2001"
int count = 0;

// Google Project ID
#define PROJECT_ID "capstone-dac"

// Service Account's client email
#define CLIENT_EMAIL "capstone-dac@capstone-dac.iam.gserviceaccount.com"

const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCmY5oHrT8D+NsQ\nHcl90g9M9ddPJGJeRJ6PiurJiZcPHC8L0U8PYBHcSCEHAwKI8muCyF7K4E+w7Ht3\n2eiew+6NOZoXPC20XEDmyaDBCPe+vUo+BWIe7VTXo4vwuiEoU68qw3OtlissSfrK\nQvuh8p7V9Z6vLIZr6EA4TpjEDdpkAjNbE3+ZP+lYbceWib7JQfHYt/jc185G7XfJ\nudJUydfCWjqaZL6DHAvE5kQiSFhsqonGDK4BmZyiQk3jJdfutclORJKcM1KVuXs7\nQm6c5+lXTY93OiiINTBW3tkOdYs9PcOYgL9MqkWYOFd/LgpPbqo6lq7FfxtbJpkK\nPW1xR6F/AgMBAAECggEAIp/7f7l3sZXp0FLCiWJbOMK4CYoixkc1T92yujndq5FL\n4QOvlh2B2ftQlCr2LhV0dgTIZSOEseakUu6Pz+oqDE1+WXN2d1/zSZOP1lvNYm31\nsy5fmtv8mds41eGF4OR5QUbnHapwVYkXnVuPDcZ1c0TnvsBfQLlWE+GXPMU9Oy47\nb/6j413KZxpUXZevwMxVTRj6pd4Lgrgq997xnKiQa55PAppUxog+7nUwal/mDudG\n1X8KTXT1j3tdSyCImD4IPwu3BsiB5dgZ/1qnQJzuuEq3lk4nvLTpXlK9Z0UnTpLU\nzlhPZUov1bI8siUDZ/BKdfOKX18fpMAhRlgSqv70cQKBgQDq2b4C+46unHaFJoGE\nAMgIV2ltbzWAU0v1CUkXmFX09RMsWOWVhmBY/lPo+EMrzOP8OPaYyd1P10qU5tOP\nmgZZcCZIEnRtmyMLU2FWBL6pP0QuKVDCABt7D94AXqv81VyapZNdobgFBUKrobEg\nuc+xznOUQlBoHEpSZFApffNEIwKBgQC1X4tmRz0dqOsRe+6ZwXRHaKLoGp2CwlgV\ninTUzEEvC+v3c111e2GbcuNRcuvTQjum4KhQOYON/svIoakMNhOgujDywVEbvwu4\nyQuB4Fnq40NPVwoPt83ouXeijPmsq32I4nZ9iBPU1Ith0ytXfXMTfFr10Q3oJRK/\nya3gEXuk9QKBgQCshP3HVSIwdPG/yFSWMri/H+9rm6kytn3y6DpQYfCq/Kl0B23H\n1KnMVjAv0MgPUfkcnH/SJ2AzOZUpIxPAxsXpR7Bne18Z8T/iHwM+3EdvLvGX83PG\nv6twUGyl2uX+2oGnfQBvZNA4p++dAy+KOdUv6dgPzlMg179lHRmiEUr1/wKBgHS/\nKJLQXkWeOpmJhZ9RCBEDCIGQyDyn7l0HP4bOZ1LqQJyKlOkPX1fZTvtZlvToWE9l\notHpA4Y+MVxWuuLSWDYIQ4Ou7g+HHPt35cFW8WohmW0ToLVXYeJgp+JpDjmYNXkM\nFKtfESzZF1MDw41QgedUD5TVWTdJzdTH8/UcK9OVAoGAfHfctnvMLf9BSUR4R4fO\n/w+GrF+Jf+MeJ+FM4GVJD2EweFciiio2RLwbl+OWTpHxwc5NkQ8NDTzTS0N4bYS+\nAe3J3SdnYGjQkZneFgkZ6ZTCTMGGePuQwjUjjiae7P4EZQy//V6bvFdX+bQilQOB\nJn18iJiLFrRzKg11U812+RI=\n-----END PRIVATE KEY-----\n";

// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "11HEty0bs7-QprJfBVuelEG3vBm6lEPrlrAMEcQsCsqs";


ADS1115 ADS(0x48);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(6,7);

  ADS.begin();

   GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

    // Connect to Wi-Fi
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

}

void loop() {
  // put your main code here, to run repeatedly:
  ADS.setGain(0);

  float rt; //Thermistor resistance
  float reading; //Current ADC Reading value
   //read ADC

  int16_t val_0 = ADS.readADC(2);  
 
  float f = ADS.toVoltage(2);  // voltage factor
  //Serial.println(f,5);



  float two = 2.000000;
  reading = (float)val_0 * f/two;
  //print voltage values
  //Serial.println(reading, 4);

  float press = ((reading/5.00000)+0.00842)/0.002421;

  press = press*1000;


  
  Serial.println(press, 4);
  Serial.print(",");
  Serial.println(105.2003);
  dataLogging((int)press);
 
  delay(1000);

}

void dataLogging(int pressure){
  bool ready = GSheet.ready();
  FirebaseJson response;

  Serial.println("\nAppend spreadsheet values...");
  Serial.println("----------------------------");

  FirebaseJson valueRange;
  count++;

  //int temp = curr.currentTemp;
  //int inCo2 = curr.inCo2;
  //int outCo2 = curr.outCo2;

  valueRange.add("majorDimension", "COLUMNS");
  valueRange.set("values/[0]/[0]", count);
  valueRange.set("values/[1]/[0]", pressure);
  //valueRange.set("values/[2]/[0]", inCo2);
  //valueRange.set("values/[3]/[0]", outCo2);

  bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A2" /* range to append */, &valueRange /* data range to append */);
        if (success){
            response.toString(Serial, true);
            valueRange.clear();
        }
        else{
            Serial.println(GSheet.errorReason());
        }
        Serial.println();
        Serial.println(ESP.getFreeHeap());

}
