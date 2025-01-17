
/**
   @file sketch_ESP_Room.ino

   @mainpage This is a mainpage
   Is this shown on the mainpage?

   @section description Test
   Does this even work?

   @author Who knows
*/

#include "Backend.h"

// WiFi and ThingSpeak variables.
char *ssid = "NameName";  ///< SSID of the connected network.
char *pass = "Etellerandet";  ///< Password of the connected network.

unsigned long channelID = 2808283;  ///< ChannelID of ThingSpeak channel.
char *APIReadKey = "PUSZ92SJXXMO8BDG";  ///< Read API key of ThingSpeak channel.
char *APIWriteKey = "QIDQOTKIH000FKXO"; ///< Write API key of ThingSpeak channel.
char *server = "api.thingspeak.com";  ///< ThingSpeak server.


// Define variables for the program
byte fieldT = 2; ///< The ThingSpeak field-index of the temperature field.
byte fieldH = 1; ///< The ThingSpeak field-index of the humidity field.
float T = 0;  ///< Temperature variable.
float H = 0;  ///< Humidity variable.
//variable!! yuppiii

Backend backend(ssid, pass, channelID, APIReadKey, APIWriteKey, server);

void setup() {
  //WiFi.begin(ssid, pass);
  backend.begin();
  Serial.begin(9600);
  //Serial1.begin(9600);
}

void loop() {
  // If all the sensor data has been sent, extract the values as float
  if (Serial.available() >= 12) { //idk if it's actually 12 bytes long
    String receivedData = Serial.readStringUntil('\n');  // Read until newline
    // Parse the received tuple
    int commaIndex = receivedData.indexOf(',');
    if (commaIndex > 0) {
      H = receivedData.substring(0, commaIndex).toFloat();
      T = receivedData.substring(commaIndex + 1).toFloat;
      // If the values are within the rated sensor values, post to ThingSpeak
      if (((H > 20) && (H < 90)) && ((T > -40) && (T < 125)))) {
        backend.postTSFloatData(H, fieldH);
        backend.postTSFloatData(T, fieldT);
      } else {
        //Serial.println("Readings ignored. Probably faulty.");
      }
    }
  }
}
