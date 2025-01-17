#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>

// Wi-Fi configuration
// const char* ssid = "Ask Krat";
// const char* pass = "feature-hollow-truly";
const char* ssid = "X_Liou";
const char* pass = "Teemo4life";


WiFiClient client;
// ThingSpeak configuration
unsigned long channelID = 2808283;
// Replace with your ThingSpeak channel ID
const char* APIWriteKey = "G4QFBJM48LQQLI4T";  //
const char* APIReadKey = "PUSZ92SJXXMO8BDG";
const int postDelay = 20 * 1000;  // 20 seconds delay
int data;                         //Initialized variable to store recieved data
//the above is set correctly

void setup() {
  // put your setup code here, to run once:

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

//byte storedKey[4] = { 0xC3, 0x44, 0x22, 0x4F };  //Oscars Key
byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0D };  // Lious Key
//byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0E }; //tset


void loop() {
  // put your main code here, to run repeatedly:
  // If we have received 2 floats (2x4 bytes) and some other split character, we read the data and post it to the LCD

  // Send values to ThingSpeak
  //if ((T.toFloat() > -50.0 && (T.toFloat() < 50.0)) && (H.toFloat() > 0.0 && (H.toFloat() < 100.0))) {

  //writes the integer to ThingSpeak field 1
  // for (byte i = 0; i < sizeof(storedKey); i++) {
  //   ThingSpeak.setField(3, storedKey[i]);
  // }


  if (Serial.available() > 0) {
    int statusFromArduino = Serial.read();
    ThingSpeak.setField(3, statusFromArduino);
  }

  // ThingSpeak.setField(3, 1);
  // ThingSpeak.setField(4, 0);

  // might need faster read time
  //ThingSpeak.readFloatField(_channelID, field, _APIReadKey);
  int statusFromTS = ThingSpeak.readFloatField(channelID, 3, APIReadKey);
  Serial.print(statusFromTS);


  int responseCode = ThingSpeak.writeFields(channelID, APIWriteKey);

  Serial.println(responseCode);

  if (responseCode == 200) {
    Serial.println("Data sent success");
  } else {
    Serial.println("Failed to send");
  }

  client.stop();
  delay(postDelay);
}

void UARTSendStatus(int status) {
  Serial.print(status);
  Serial.write(32);
}
