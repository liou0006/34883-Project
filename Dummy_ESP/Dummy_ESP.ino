/**
 * @file Dummy_ESP.ino
 *
 * @mainpage arrrrgh
 *
 * @section description Test
 * Does this even work?
 *
 * @author Mads Andersen & Lukas Tallbacka
 */

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// WiFi and ThingSpeak variables
const char* ssid = "NameName";  ///< SSID of the connected network.
const char* pass = "Etellerandet";  ///< Password of the connected network.
WiFiClient client;

const unsigned long channelID = 2808283;  ///< ChannelID of ThingSpeak channel. @see 
const char *APIReadKey = "PUSZ92SJXXMO8BDG";  ///< Read API key of ThingSpeak channel.
//const char *APIWriteKey = "QIDQOTKIH000FKXO";
const char *server = "api.thingspeak.com";


// Define variables for the program
const uint8_t fieldT = 1; ///< The ThingSpeak field-index of the temperature field.
const uint8_t fieldH = 2; ///< The ThingSpeak field-index of the humidity field.
float T = 0;  ///< Temperature variable.
float H = 0;  ///< Humidity variable.
//variable!! yuppiii

void setup() {
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  delay(10000);
  getData(&T, &H);
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

/**
 * @brief Gets the temperature and humidity data from ThingSpeak.
 *
 * @param *T  Pointer to temperature variable.
 * @param *H  Pointer to the humidity variable.
 */
void getData(float *T, float *H){
  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)
  *T = ThingSpeak.readFloatField(channelID, fieldT, APIReadKey);
  *H = ThingSpeak.readFloatField(channelID, fieldH, APIReadKey);
  client.stop();
}
