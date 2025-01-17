#include <Backend.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

WiFiClient client;

Backend::Backend(char* ssid, char* pass, unsigned long channelID, char *APIReadKey, char *APIWriteKey, char *server) {
  _ssid = ssid;
  _pass = pass;
  _channelID = channelID;
  _APIReadKey = APIReadKey;
  _APIWriteKey = APIWriteKey;
  _server = server;
}

void Backend::begin() {
  WiFi.begin(_ssid, _pass);
  ThingSpeak.begin(client);
}

/**
   @brief Writes a float to a specified field in ThingSpeak.

   @param D  Data variable.
   @param field  Field-index of ThingSpeak field. 1-indexed.
*/
void Backend::postTSFloatData(float Data1, float Data2, byte field1, byte field2) {
  client.connect(_server, 80);
  ThingSpeak.setField(field1, Data1);
  ThingSpeak.setField(field2, Data2);
  ThingSpeak.writeFields(_channelID, _APIWriteKey);
  client.stop();
}

/**
   @brief Reads a float datatype from a specified field in ThingSpeak.

   @param *D  Pointer to data variable.
   @param field  Field-index of ThingSpeak field. 1-indexed.
*/
void Backend::getTSFloatData(float *D, byte field) {
  client.connect(_server, 80);
  *D = ThingSpeak.readFloatField(_channelID, field, _APIReadKey);
  client.stop();
}




