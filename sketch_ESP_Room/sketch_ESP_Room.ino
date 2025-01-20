/**
*@file sketch_ESP_Room.ino
*
*
*@section description Description
*Receives data from arduino and transmits it through WiFi to ThingSpeak
*
*
*@section circuit Circuit
*How to connect the DHT11:
*-Signal (DHT) to D4 (arduino)
*
*How to connect TMP36GZ:
*-Vout (TMP) to A0 (arduino)
*
*
*How to connect arduino to ESP8266:
*-D18 (arduino) to Rx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-D19 (arduino) to Tx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*
*How to connect servo:
*-D37 (arduino Mega) to servo signal 
*
*How to connect lcd:
*-D20 (arduino) to SDA (LCD)
*-D21 (arduino) to SCL (LCD)
*
*
*
*@section libraries Libraries
*-Backend.h (personal library that contains functions for WiFi and ThingSpeak)
*
*
*@section author Authors
*Created by Ask Krat, Christian Houmann, Mads Andersen & Lukas Tallbacka
*
*
*@date 20/1/2025
*
**/

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
      backend.postTSFloatData(H, T, fieldH, fieldT);
    } else {
      //Serial.println("Readings ignored. Probably faulty.");
    }
  }
}
