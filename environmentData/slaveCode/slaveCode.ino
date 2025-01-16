#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>
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
//int ledPin = D2;  // Pin connected to the LED (D4 for built-in LED on ESP8266)
int data;  //Initialized variable to store recieved data

SoftwareSerial mySerial(1, 3);  //RX, TX


char buffer[50];      // Buffer to store incoming data
int bufferIndex = 0;  // Index to track the buffer

void setup() {
  Serial.begin(9600);
  // // Initialize serial communication for debugging
  // //pinMode(ledPin, OUTPUT);    // Set LED pin as output
  // //digitalWrite(ledPin, LOW);  // Turn off the LED initially
  // Serial.println("Connecting to WiFi...");
  // WiFi.begin(ssid, pass);  // Connect to WiFi
  WiFi.begin(ssid, pass);

  // //   // Wait for WiFi connection
  //    while (WiFi.status() != WL_CONNECTED) {
  //      delay(500);
  //      Serial.print(".");
  //    }
  //    Serial.println("\nConnected to WiFi");
  //    Serial.print("IP Address: ");
  //    Serial.println(WiFi.localIP());  // Print the assigned IP address
  // }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  // // Get RSSI value
  //float rssi = WiFi.RSSI();

  // // // Toggle LED state
  // // // static bool ledState = false;                 // Track LED state (on/off)
  // // // ledState = !ledState;                         // Toggle state
  // // // digitalWrite(ledPin, ledState ? HIGH : LOW);  // Update LED

  // // // Print debug information
  //Serial.print("WiFi RSSI: ");
  //Serial.println(rssi);
  // //Serial.print("LED State: ");
  // //Serial.println(ledState ? "ON" : "OFF");

  // // // Send data to ThingSpeak
  //ThingSpeak.begin(client);
  //ThingSpeak.setField(1, rssi);              // Send RSSI to Field 1
  //  //ThingSpeak.setField(2, ledState ? 1 : 0);  // Send LED state to Field 2 (1 for ON, 0 for OFF)

  // if (ThingSpeak.writeFields(channelID, APIWriteKey)) {
  //   Serial.println("Data sent to ThingSpeak successfully.");
  // } else {
  //   Serial.println("Failed to send data to ThingSpeak.");
  // }

  // // // Read the value from Field 1 (RSSI) using the Read API key
  // float field1Value = ThingSpeak.readFloatField(channelID, 1, APIReadKey);
  // if (!isnan(field1Value)) {
  //   Serial.print("Field 1 (RSSI) value from ThingSpeak: ");
  //   Serial.println(field1Value);
  // } else {
  //   Serial.println("Failed to read Field 1 value from ThingSpeak.");
  // }

  // client.stop();     // Close the connection
  // delay(postDelay);  // Wait before the next update

  //data = Serial.read();  //Read the serial data and store it
  //Serial.println(data);
  //Serial.write(Serial.read());


  String receivedData = Serial.readStringUntil('\n');  // Read until newline
  // Parse the received tuple
  int commaIndex = receivedData.indexOf(',');
  if (commaIndex > 0) {
    String value1 = receivedData.substring(0, commaIndex);
    String value2 = receivedData.substring(commaIndex + 1);
    // Send values to ThingSpeak

    if ((value1.toFloat() > -50.0 && (value1.toFloat() < 50.0)) && (value2.toFloat() > 0.0 && (value2.toFloat() < 100.0))) {

    if (ThingSpeak.setField(1, value1.toFloat())) {
      Serial.println("Field 1 updated");
      Serial.println(value1);
    } else {
      Serial.println("Failed to update Field 1");
    }

    if (ThingSpeak.setField(2, value2.toFloat())) {
      Serial.println(value2);
      Serial.println("Field 2 updated");
    } else {
      Serial.println("Failed to update Field 1");
    }
    // if (ThingSpeak.writeField(myChannelNumber, 2, value2.toInt(), myWriteAPIKey) == 200) {
    //   Serial.println("Field 2 updated");
    // } else {
    //   Serial.println("Failed to update Field 2");
    // }
  }
  }
  
  int responseCode = ThingSpeak.writeFields(channelID, APIWriteKey);

  if (responseCode == 200) {
    Serial.println("Data sent success");
  } else {
    Serial.println("Failed to send");
  }


  delay(1000);  // Wait for 1 second before next loop
}

