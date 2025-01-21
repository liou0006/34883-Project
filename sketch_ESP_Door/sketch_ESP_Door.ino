#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Wi-Fi configuration
WiFiClient client;
// const char* ssid = "Ask Krat";
// const char* pass = "feature-hollow-truly";
const char* ssid = "X_Liou";
const char* pass = "Teemo4life";

// ThingSpeak configuration
unsigned long channelID = 2808283;
// Replace with your ThingSpeak channel ID
const char* APIWriteKey = "G4QFBJM48LQQLI4T";  //
const char* APIReadKey = "PUSZ92SJXXMO8BDG";
const int postDelay = 15 * 1000;  // 15 seconds delay

int responseCode = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {

  if (Serial.available() > 0) {
    byte oneBite = Serial.read();
    Serial.println(oneBite);
    // debuggingTS();

    while (responseCode != 200) {
      ThingSpeak.setField(3, oneBite);
      responseCode = ThingSpeak.writeFields(channelID, APIWriteKey);

      if (responseCode == 200) {
        Serial.println("Data sent success");
      } else {
        Serial.println("Failed to send");
      }
      delay(postDelay);
    }
    responseCode = 0;
  }
  client.stop();
}

void debuggingTS() {
  while (responseCode != 200) {
    responseCode = ThingSpeak.writeFields(channelID, APIWriteKey);

    if (responseCode == 200) {
      Serial.println("Data sent success");
    } else {
      Serial.println("Failed to send");
    }
    delay(postDelay);
  }
  responseCode = 0;
}
