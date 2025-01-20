#ifndef Backend_H_
#define Backend_H_

#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
//#include "Arduino.h"

class Backend {
  public:
    Backend(char* ssid, char* pass, unsigned long channelID,
            char *APIReadKey, char *APIWriteKey, char *server);
    void begin();
    void getTSFloatData(float *D, byte field);
    void postTSFloatData(float D, byte field);
    
  private:
    // WiFi and ThingSpeak variables
    char* _ssid;  ///< SSID of the connected network.
    char* _pass;  ///< Password of the connected network.

    unsigned long _channelID;  ///< ChannelID of ThingSpeak channel.
    char *_APIReadKey;  ///< Read API key of ThingSpeak channel.
    char *_APIWriteKey;  ///< Write API key of ThingSpeak channel.
    char *_server;  ///< ThingSpeak server.
};

#endif
