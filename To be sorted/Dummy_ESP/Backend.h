#ifndef Backend_H_
#define Backend_H_

#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
//#include "Arduino.h"

class Backend{
    public:
        Backend(char* ssid, char* pass, unsigned long channelID,
        char *APIReadKey, char *APIWriteKey, char *server,
        uint8_t fieldT, uint8_t fieldH);
        void begin();
        void getData(float *T, float *H);
    private:
        // WiFi and ThingSpeak variables
        char* _ssid;  ///< SSID of the connected network.
        char* _pass;  ///< Password of the connected network.

        unsigned long _channelID;  ///< ChannelID of ThingSpeak channel. @see 
        char *_APIReadKey;  ///< Read API key of ThingSpeak channel.
        char *_APIWriteKey;
        char *_server;

        // Define the fieldIDs
        uint8_t _fieldT; ///< The ThingSpeak field-index of the temperature field.
        uint8_t _fieldH; ///< The ThingSpeak field-index of the humidity field.
};

#endif
