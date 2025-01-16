
/**
 * @file sketch_ESP_Room.ino
 *
 * @mainpage arrrrgh
 *
 * @section description Test
 * Does this even work?
 *
 * @author Mads Andersen & Lukas Tallbacka
 */

//#include <ESP8266WiFi.h>
//#include <ThingSpeak.h>
#include "Backend.h"

// WiFi and ThingSpeak variables
char *ssid = "NameName";  ///< SSID of the connected network.
char *pass = "Etellerandet";  ///< Password of the connected network.
//WiFiClient client;

unsigned long channelID = 2808283;  ///< ChannelID of ThingSpeak channel. @see 
char *APIReadKey = "PUSZ92SJXXMO8BDG";  ///< Read API key of ThingSpeak channel.
char *APIWriteKey = "QIDQOTKIH000FKXO";
char *server = "api.thingspeak.com";


// Define variables for the program
uint8_t fieldT = 2; ///< The ThingSpeak field-index of the temperature field.
uint8_t fieldH = 1; ///< The ThingSpeak field-index of the humidity field.
float T = 0;  ///< Temperature variable.
float H = 0;  ///< Humidity variable.
//variable!! yuppiii

Backend backend(ssid, pass, channelID, APIReadKey, APIWriteKey, server, fieldT, fieldH);

void setup() {
  //WiFi.begin(ssid, pass);
  backend.begin();
  Serial.begin(9600);
  //Serial1.begin(9600);
}

void loop() {
  delay(10000);
  backend.getData(&T, &H);
  UARTSendData(T, H);
}

/**
 * @brief Sends the temperature and humidity data to Arduino connected to UART1.
 *
 * @param T  Temperature variable.
 * @param H  Humidity variable.
 */
void UARTSendData(float T, float H) {
  Serial1.print(T);
  Serial1.write(32);
  Serial1.print(H);
}
