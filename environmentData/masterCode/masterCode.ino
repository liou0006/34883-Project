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


#include <dht.h> ///< Include the library for DHT sensors

dht DHT; ///< Create a DHT object to interact with the DHT11 sensor

#define DHT11_PIN 12 ///< Define the pin connected to the DHT11 sensor signal line

int data;                    ///< Variable to store received data
#include <SoftwareSerial.h>  ///< Include the SoftwareSerial library to enable serial communication on other pins

SoftwareSerial s(5,6); ///<// Use Arduino pins 5 and 6 for SoftwareSerial communication with the ESP8266

/** @brief Initialize the serial monitor and the SoftwareSerial communication with ESP8266
**/
void setup() {
  Serial.begin(9600); ///<// Initialize the serial monitor for debugging
  s.begin(9600); ///< Initialize the SoftwareSerial communication with ESP8266

}

/** @brief Reads the data from the humidity sensor and calculates the temperature value from the analog value from the TMP36 temperature sensor
*and sends the humidity and temperature to the ESP8266 
**/
void loop() {
  int chk = DHT.read11(DHT11_PIN); ///< Read the data from the DHT11 sensor
   int sensorValue = analogRead(A0); ///< Read the analog value from TMP36 temperature sensor on pin A0
   float tempValue = (5.0 / 1023.0) * sensorValue * 100; ///< Convert the analog value to a temperature in Celsius

  sendingDataToESP(DHT.humidity,tempValue); ///< Send humidity and temperature data to the ESP8266
  
  delay(2000); ///< Wait for 2 seconds before taking the next reading
}

/** @brief A function that takes the humidity and temperature as floats and sends it to the ESP8266 module
* Prints to the serial monitor to double-check that the values are correct
*@param humidity is Humidity value from the DHT11 sensor
*@param temperature is the calculated temperature from the TMP36 sensor  
**/
void sendingDataToESP(float humidity, float temperature) {
  s.print(String(humidity)+ "," + String(temperature) + "\n"); ///< Send the data as a formatted string to the ESP8266 over SoftwareSerial
  Serial.println(humidity); ///< Print the data to the serial monitor
  Serial.println(String(temperature)); ///< Print the data to the serial monitor

}


