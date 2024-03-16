#include <SPI.h>
#include <SD.h>

#define CS 18
#define MOSI 19
#define MISO 20
#define SCLK 21
File myFile;


void WriteFile(const char * path, const char * message){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(path, FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.printf("Writing to %s ", path);
    myFile.println(message);
    myFile.close(); // close the file:
    Serial.println("completed.");
  } 
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening file ");
    Serial.println(path);
  }
}


void ReadFile(const char * path){
  // open the file for reading:
  myFile = SD.open(path);
  if (myFile) {
     Serial.printf("Reading file from %s\n", path);
     // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close(); // close the file:
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void setup() {
  Serial.begin(9600);    // Set serial baud rate to 9600
  delay(500);

  SPIClass SPI2(SPI);
SPI2.begin(SCLK, MISO, MOSI, CS);

  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS, SPI2)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  WriteFile("/test.txt", "Testing123");
  ReadFile("/test.txt");
}

void loop() {
  // nothing happens after setup
}
