#include <ESP8266WiFi.h> 
#include <ThingSpeak.h> 
// Wi-Fi configuration 
const char* ssid = "Ask Krat"; 
const char* pass = "feature-hollow-truly"; 
WiFiClient client; 
// ThingSpeak configuration 
unsigned long channelID = 2808435;             
// Replace with your ThingSpeak channel ID 
const char* APIWriteKey = "NZMINPBNS2DQ54M7";  // Replace with your Write API Key 
const char* APIReadKey = "Y19UBOX6WN8V7LNW"; 
const int postDelay = 20 * 1000;  // 20 seconds delay 
int ledPin = D2;  // Pin connected to the LED (D4 for built-in LED on ESP8266) 
void setup() { 
Serial.begin(115200);       
// Initialize serial communication for debugging 
pinMode(ledPin, OUTPUT);    // Set LED pin as output 
digitalWrite(ledPin, LOW);  // Turn off the LED initially 
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
  float rssi = WiFi.RSSI(); 
 
  // Toggle LED state 
  static bool ledState = false;                 // Track LED state (on/off) 
  ledState = !ledState;                         // Toggle state 
  digitalWrite(ledPin, ledState ? HIGH : LOW);  // Update LED 
 
  // Print debug information 
  Serial.print("WiFi RSSI: "); 
  Serial.println(rssi); 
  Serial.print("LED State: "); 
  Serial.println(ledState ? "ON" : "OFF"); 
 
  // Send data to ThingSpeak 
  ThingSpeak.begin(client); 
  ThingSpeak.setField(1, rssi);              // Send RSSI to Field 1 
  ThingSpeak.setField(2, ledState ? 1 : 0);  // Send LED state to Field 2 (1 for ON, 0 for OFF) 
 
  if (ThingSpeak.writeFields(channelID, APIWriteKey)) { 
    Serial.println("Data sent to ThingSpeak successfully."); 
  } else { 
    Serial.println("Failed to send data to ThingSpeak."); 
  } 
 
  // Read the value from Field 1 (RSSI) using the Read API key 
  float field1Value = ThingSpeak.readFloatField(channelID, 1, APIReadKey); 
  if (!isnan(field1Value)) { 
    Serial.print("Field 1 (RSSI) value from ThingSpeak: "); 
    Serial.println(field1Value); 
  } else { 
    Serial.println("Failed to read Field 1 value from ThingSpeak."); 
  } 
 
  client.stop();     // Close the connection 
  delay(postDelay);  // Wait before the next update 
} 
