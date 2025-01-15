#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(12,13);

//int data; //Initialized variable to store recieved data

// Wi-Fi configuration
const char* ssid = "Ask Krat";
const char* pass = "feature-hollow-truly";
WiFiClient client;
// ThingSpeak configuration
unsigned long channelID = 2808283;
// Replace with your ThingSpeak channel ID
const char* APIWriteKey = "G4QFBJM48LQQLI4T";  // Replace with your Write API Key
const char* APIReadKey = "PUSZ92SJXXMO8BDG";
const int postDelay = 20 * 1000;  // 20 seconds delay
int ledPin = D2;                  // Pin connected to the LED (D4 for built-in LED on ESP8266)



void setup() {
  Serial.begin(9600);
  s.begin(9600);
  // Initialize serial communication for debugging
  //pinMode(ledPin, OUTPUT);    // Set LED pin as output
  //digitalWrite(ledPin, LOW);  // Turn off the LED initially
  Serial.println("Connecting to WiFi...");
   WiFi.begin(ssid, pass);  // Connect to WiFi

  // Wait for WiFi connection
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println("\nConnected to WiFi");
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());  // Print the assigned IP address

}

void loop() {
  // Get RSSI value
   //float rssi = WiFi.RSSI();

  // // Toggle LED state
  // //static bool ledState = false;                 // Track LED state (on/off)
  // //ledState = !ledState;                         // Toggle state
  // //digitalWrite(ledPin, ledState ? HIGH : LOW);  // Update LED

  // // Print debug information
  //Serial.print("WiFi RSSI: ");
  //Serial.println(rssi);
  //Serial.print("LED State: ");
  //Serial.println(ledState ? "ON" : "OFF");
    
  float temperature, humidity;
  // if (s.available() >= 2 * sizeof(float)) {

  //   // Read the raw binary data into the float variables
  //   s.readBytes((char*)&temperature, sizeof(temperature));
  //   s.readBytes((char*)&humidity, sizeof(humidity));

  //   // Print the received values
  //   Serial.print("Received Temperature: ");
  //   Serial.println(temperature);
  //   Serial.print("Received Humidity: ");
  //   Serial.println(humidity);
  // }


  if(s.available()) {
    String temperature = s.readString(); //Read the serial data and store it
    String humidity = s.readString();
    Serial.println("Temperature: " + temperature);
    Serial.println("Humidity: " + humidity);
  }


  // // Send data to ThingSpeak
   ThingSpeak.begin(client);
   ThingSpeak.setField(1, temperature);
   ThingSpeak.setField(2, humidity);

   if (ThingSpeak.writeFields(channelID, APIWriteKey)) {
     Serial.println("Data sent to ThingSpeak successfully.");
   } else {
     Serial.println("Failed to send data to ThingSpeak.");
   }

  // // Read the value from Field 1 (RSSI) using the Read API key
   float field1Value = ThingSpeak.readFloatField(channelID, 1, APIReadKey);
   if (!isnan(field1Value)) {
     Serial.print("Field 1 (temperature) value from ThingSpeak: ");
     Serial.println(field1Value);
   } else {
     Serial.println("Failed to read Field 1 value from ThingSpeak.");
   }

   client.stop();     // Close the connection
   //delay(postDelay);  // Wait before the next update




  delay(2000);
  

}
