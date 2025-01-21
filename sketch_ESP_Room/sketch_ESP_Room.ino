/**
*@file sketch_ESP_Room.ino
*
*
*@section description Description
*Receives data from ATMEGA2560 microcontroller, and transmits it through WiFi to ThingSpeak.
*
*
*@section circuit Circuit
*
* Just need to connect the ESP8266's UART pins to the ATMEGA2560:\n 
*-D18/TX1 (ATMEGA2560) to RX (ESP)\n 
*-D19/RX1 (ATMEGA2560) to TX (ESP)
*
*
*@section libraries Libraries
*-Backend.h (custom library for connecting to WiFi and ThingSpeak)
*
*
*@section author Authors
*Created by Ask Krat, Christian Houmann, Mads Andersen & Lukas Tallbacka
*
*
*@date 21/1/2025
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
byte fieldIsHome = 3; ///< The ThingSpeak field-index of the home status field.
float T = 0;  ///< Temperature variable.
float H = 0;  ///< Humidity variable.
int IsHome = 0; ///< "Boolean" status indicator of someone being home.
//variables!! yuppiii

/// Creates backend object with WiFi and ThingSpeak parameters
Backend backend(ssid, pass, channelID, APIReadKey, APIWriteKey, server);

void setup() {
  //WiFi.begin(ssid, pass);
  backend.begin();
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  //Serial1.begin(9600);
}

void loop() {
  // If all the sensor data has been sent (2x 4-byte floats and a space), extract the values as float
  if (Serial.available() >= 9) {
    T = Serial.parseFloat();
    H = Serial.parseFloat();

    // If the values are within the rated sensor values, post to ThingSpeak
    if (((H > 20) && (H < 90)) && ((T > -40) && (T < 125))) {
      backend.postTSFloatData(H, T, fieldH, fieldT);
    } else {
      //Serial.println("Readings ignored. Probably faulty.");
    }

    // Wait for another possible communication with ThingSpeak, then get IsHome status
    delay(20000);
    backend.getTSintData(&IsHome, fieldIsHome);
    Serial.print(IsHome);
  }
  

}
