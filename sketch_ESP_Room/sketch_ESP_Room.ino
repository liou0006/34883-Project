/**
*@file masterCode.ino
*
*
*@section description Description
*Sending data between arduino Uno R3 and ESP8266 NodeMCU while reading values from a temperature sensor (analog) and a humidity sensor (digital)
*and uploading the data to thingSpeak to show a graph of the humidity and temperature 
*
*
*@section setup Setup
*Setup:
*-baud rate = 9600 on both arduino and ESP
*
*
*@section circuit Circuit
*How to connect the DHT11:
*-Signal (DHT) to D12 (arduino)
*-Vcc (DHT) to power (5V - breadboard)
*-GND (DHT) to GND (breadboard)
*
*How to connect TMP36GZ:
*-Vs (TMP) to GND (breadboard)
*-Vout (TMP) to A0 (arduino)
*-Gnd (TMP) to GND (arduino)
*
*Setup the breadboard with GND and 5V:
*-5V (arduino) to + (breadboard)
*-GND (arduino) to - (breadboard) 
*
*How to connect arduino to ESP8266:
*-D6 (arduino) to Rx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-D5 (arduino) to Tx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-GND (ESP) to GND (breadboard)
*
*Connect LED to ESP8266:
*-cathode (LED) to D2 (ESP)
*-anode (LED) to GND (breadboard) 
*
*
*@section libraries Libraries
*-dht.h (Download a zip file of this library from this link: https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/)
*-SoftwareSerial.h
*
*
*@section author Authors
*Created by Ask Krat, Christian Houmann
*
*
*@date Today
*
**/



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
      backend.postTSFloatData(H, T, fieldH, fieldT);
    } else {
      //Serial.println("Readings ignored. Probably faulty.");
    }
  }
}
