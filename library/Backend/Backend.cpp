/**
*@file Backend.cpp
*
*
*@section description Description
*A simple library to connect various WiFi and ThingSpeak calls into one space. Utilizes a class to store network and ThingSpeak variables, to use in public function calls
*
*@section author Authors
*Created by Liu Xia, Mads Andersen & Lukas Tallbacka
*
*
*@date 24/1/2025
*
**/

#include <Backend.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

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
   @brief Writes two floats to specified fields in ThingSpeak.

   @param Data1  First data variable.
   @param Data1  Second data variable.
   @param field  First field-index of ThingSpeak field. 1-indexed.
   @param field  Second field-index of ThingSpeak field. 1-indexed.

   @return  ThingSpeak response-code
*/
int Backend::postTSFloatData(float Data1, float Data2, byte field1, byte field2) {
  int responseCode; 
  client.connect(_server, 80);
  ThingSpeak.setField(field1, Data1);
  ThingSpeak.setField(field2, Data2);
  responsCode = ThingSpeak.writeFields(_channelID, _APIWriteKey);
  client.stop();

  return responsCode;
}

/**
   @brief Writes an byte to a specified field in ThingSpeak.

   @param D   Data variable.
   @param field   Field-index of ThingSpeak field. 1-indexed.

   @return  ThingSpeak response-code
*/
int Backend::postTSByteData(byte D, byte field) { 
  int responseCode;
  client.connect(_server, 80);
  ThingSpeak.setField(field, D);
  responsCode = ThingSpeak.writeFields(_channelID, _APIWriteKey);
  client.stop();

  return responsCode;
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

/**
   @brief Reads byte from a specified field in ThingSpeak.

   @param *D  Pointer to data variable.
   @param field  Field-index of ThingSpeak field. 1-indexed.
*/
void Backend::getTSByteData(int *D, byte field) {
  client.connect(_server, 80);
  *D = ThingSpeak.readIntField(_channelID, field, _APIReadKey);
  client.stop();
}
