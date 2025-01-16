
//#include "Arduino.h"
#include "Backend.h"
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;

Backend::Backend(char* ssid, char* pass, unsigned long channelID, char *APIReadKey, char *APIWriteKey, char *server, uint8_t fieldT, uint8_t fieldH){
  _ssid = ssid;
  _pass = pass;
  _channelID = channelID;
  _APIReadKey = APIReadKey;
  _APIWriteKey = APIWriteKey;
  _server = server;
  _fieldT = fieldT;
  _fieldH = fieldH;
}

void Backend::begin(){
  WiFi.begin(_ssid, _pass);
  Serial1.begin(9600);
}

/**
 * @brief Gets the temperature and humidity data from ThingSpeak.
 *
 * @param *T  Pointer to temperature variable.
 * @param *H  Pointer to the humidity variable.
 */
void Backend::getData(float *T, float *H){
  ThingSpeak.begin(client);
  client.connect(_server, 80); //connect(URL, Port)
  *T = ThingSpeak.readFloatField(_channelID, _fieldT, _APIReadKey);
  *H = ThingSpeak.readFloatField(_channelID, _fieldH, _APIReadKey);
  client.stop();
}
