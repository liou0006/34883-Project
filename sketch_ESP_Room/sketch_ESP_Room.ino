/**
   @file sketch_ESP_Room.ino

   @mainpage This is a mainpage
   Is this shown on the mainpage?

   @section description Test
   Does this even work?

   @author Who knows
*/

#include <Backend.h>

// WiFi and ThingSpeak variables.
char *ssid = "NameName";  ///< SSID of the connected network.
char *pass = "Etellerandet";  ///< Password of the connected network.

unsigned long channelID = 2808283;  ///< ChannelID of ThingSpeak channel.
char *APIReadKey = "PUSZ92SJXXMO8BDG";  ///< Read API key of ThingSpeak channel.
char *APIWriteKey = "G4QFBJM48LQQLI4T"; ///< Write API key of ThingSpeak channel.
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  //Serial1.begin(9600);
}

void loop() {
  // If all the sensor data has been sent, extract the values as float

  if (Serial.available() >= 9) { //idk if it's actually 12 bytes long
    T = Serial.parseFloat();
    H = Serial.parseFloat();
    Serial.print(T);
    Serial.println(H);

    // If the values are within the rated sensor values, post to ThingSpeak
    if (((H > 20) && (H < 90)) && ((T > -40) && (T < 125))) {
      // THIS HAPPENS TOO FAST. WHY DID I MAKE IT THIS WAY. ARRRRRRGH
      backend.postTSFloatData(H, fieldH);
      backend.postTSFloatData(T, fieldT);
    } else {
      //Serial.println("Readings ignored. Probably faulty.");
    }
  }
}
