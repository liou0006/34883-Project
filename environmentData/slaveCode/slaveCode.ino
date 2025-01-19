/**
*@file slaveCode.ino
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
*-ESP8266WiFi.h (use this link in Settings>Additional...: http://arduino.esp8266.com/stable/package_esp8266com_index.json)
*-ThingSpeak.h (Download in the Library manager - search for ThingSpeak by MathWorks)
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



// Include libraries for ESP8266, ThingSpeak, and SoftwareSerial
#include <ESP8266WiFi.h> ///<Library for connecting the ESP8266 to WiFi
#include <ThingSpeak.h> ///<Library for communicating with ThingSpeak
#include <SoftwareSerial.h> ///<library for communicating with the arduino uno

// Wi-Fi configuration
const char* ssid = "Ask Krat"; ///< Wi-Fi network name
const char* pass = "feature-hollow-truly"; ///< Wi-Fi password
WiFiClient client;  ///< Create a Wi-Fi client object

// ThingSpeak configuration
unsigned long channelID = 2808283; ///< ThingSpeak channel ID
const char* APIWriteKey = "G4QFBJM48LQQLI4T";  ///< Write API key for ThingSpeak
const char* APIReadKey = "PUSZ92SJXXMO8BDG"; ///< Read API key for ThingSpeak
const int postDelay = 20 * 1000;  ///< Delay between data posts (20 seconds)

//the pins on the ESP8266
SoftwareSerial mySerial(1, 3);  ///< SoftwareSerial for communication with ESP8266 (pins RX=1, TX=3)

/** @brief Tries to connect to the WiFi and if it is a success, it begins the communication with ThingSpeak
**/
void setup() {
  Serial.begin(9600); ///< Initialize Serial Monitor for debugging
  
  WiFi.begin(ssid, pass); ///< Start Wi-Fi connection with the given SSID and password

  // Wait until Wi-Fi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi"); ///< Confirm Wi-Fi connection
  ThingSpeak.begin(client); ///< Initialize ThingSpeak client
}

/** @brief Converts the received string from the Serial to two floats and sends the floats to ThingSpeak into two seperate graphs
*-One graph for humidity (the 1st value - Channel 1)
*-One graph for temperature (the 2nd value - Channel 2)
**/
void loop() {
  //handles the recieved data string and converts it into two floats
  String receivedData = Serial.readStringUntil('\n');  ///< Read data from Serial until newline character
  
  // Parse the received tuple
  int commaIndex = receivedData.indexOf(','); ///< Find the comma in the received string
  if (commaIndex > 0) { ///< Ensure a valid comma is found
    String value1 = receivedData.substring(0, commaIndex); ///< Extract first value (before the comma)
    String value2 = receivedData.substring(commaIndex + 1); ///< Extract second value (after the comma)
    
    // Validate and send values to ThingSpeak if they are within valid ranges
    if ((value1.toFloat() > -50.0 && (value1.toFloat() < 50.0)) && (value2.toFloat() > 0.0 && (value2.toFloat() < 100.0))) {
      sendDataToThingSpeak(value1,value2); ///< Send validated data to ThingSpeak
  }
  }
  
  //checks if the upload to ThingSpeak was succesful
  int responseCode = ThingSpeak.writeFields(channelID, APIWriteKey); ///< Write fields to ThingSpeak

  if (responseCode == 200) { ///< HTTP response code 200 indicates success
    Serial.println("Data sent success");
  } else { ///< Other codes indicate failure
    Serial.println("Failed to send");
  }


  delay(1000);  // Wait for 1 second before the next loop iteration
}

/** @brief Sends the humidity and temperature to ThingSpeak
*-the first value (humidity) is send to field 1
*-the 2nd value (temperature) is send to field 2
*@param value1 is the humidity as a percentage
*@param value2 is the temperature (in celcius)
**/
void sendDataToThingSpeak(String value1, String value2) {
   // Converts the first value from a string to a float and writes it to ThingSpeak field 1 
    if (ThingSpeak.setField(1, value1.toFloat())) {
      Serial.println("Field 1 updated");
      Serial.println(value1); ///< Print the updated value for debugging
    } else {
      Serial.println("Failed to update Field 1"); ///< Log failure
    }

    // Converts the 2nd value from a string to a float and writes it to ThingSpeak field 1 
    if (ThingSpeak.setField(2, value2.toFloat())) {
      Serial.println(value2); ///< Print the updated value for debugging
      Serial.println("Field 2 updated"); 
    } else {
      Serial.println("Failed to update Field 1"); ///< Log failure
    }
}





