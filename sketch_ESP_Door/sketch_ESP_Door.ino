/**
*@file sketch_ESP_Door.ino
*
*
*@section description Description
*Receives home status from UNO microcontroller, and transmits it through WiFi to ThingSpeak.
*
*
*@section circuit Circuit
*
* Just need to connect the ESP8266's UART pins to the UNOs:\n 
*-TX (UNO) to RX (ESP)\n 
*-RX (UNO) to TX (ESP)
*
* And connect a status LED to D3 for website communication confirmation.
*
*@section libraries Libraries
*-Backend.h (custom library for connecting to WiFi and ThingSpeak)\n 
*-Various libraries for locally hosting a webserver (ESP8266mDNS.h, ESP8266WiFiMulti.h, ESP8266WebServer.h)
*
*
*@section author Authors
*Created by Liou Xia & Oscar Sjelle
*
*
*@date 24/1/2025
*
**/

#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <Backend.h>

ESP8266WebServer server(80);

// Wi-Fi configuration
char* ssid = "Username";  ///< SSID of the connected network.
char* pass = "Password";  ///< Password of the connected network.
const int ledLocal = D3;  ///< LED for remote access from website

// ThingSpeak configuration
unsigned long channelID = 2808283;  ///< ChannelID of ThingSpeak channel.
char *APIReadKey = "PUSZ92SJXXMO8BDG";  ///< Read API key of ThingSpeak channel.
char *APIWriteKey = "G4QFBJM48LQQLI4T"; ///< Write API key of ThingSpeak channel.
char *server = "api.thingspeak.com";  ///< ThingSpeak server.
byte fieldHS = 3;                 ///< The ThingSpeak field-index of the home status field.
const int postDelay = 15 * 1000;  ///< 15 seconds delay communication between ThingSpeak and ESP8266.

int responseCode = 0; ///< Variable for response from ThingSpeak.

/// Creates backend object with WiFi and ThingSpeak parameters
Backend backend(ssid, pass, channelID, APIReadKey, APIWriteKey, server);

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  serverInit();
  pinMode(ledLocal, OUTPUT);
  digitalWrite(ledLocal, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
}

void loop() {

  serverLoop();

  // If we receive communication on UART (home status), attempt to write to ThingSpeak until it is succesful.
  if (Serial.available() > 0) {
    byte oneBite = Serial.read();
    Serial.println(oneBite);

    while (responseCode != 200) {
      responseCode = Backend.postTSByteData(oneBite, fieldHS);

      if (responseCode == 200) {
        Serial.println("Data sent success");
      } else {
        Serial.println("Failed to send");
      }
      delay(postDelay);
    }
    responseCode = 0;
  }
}

/*
*
* @brief Initializes the server
*/
void serverInit() {

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  // Setup server endpoints
  server.on("/", HTTP_GET, handleRoot);
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/LEDState", HTTP_GET, handleLEDState);
 // server.on("/rssi", HTTP_GET, handleRSSIData);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Server started");
}

/*
*
* @brief creates the webpage
*/
void handleRoot() {
    server.send(200, "text/html", R"rawliteral(
<html>
<head>
    <title>ThingSpeak Web Server</title>
    <meta charset="utf-8" />
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            color: #333;
            text-align: center;
            padding: 20px;
        }
        h1 {
            color: #4CAF50;
        }
        p {
            font-size: 18px;
        }
        .button {
            background-color: #4CAF50;
            color: white;
            padding: 15px 30px;
            text-align: center;
            font-size: 18px;
            border: none;
            border-radius: 8px;
            cursor: pointer;
            margin: 20px 0;
        }
        .button:hover {
            background-color: #45a049;
        }
        .card {
            background-color: white;
            padding: 20px;
            margin: 20px auto;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
            max-width: 600px;
        }
        .status {
            font-weight: bold;
            font-size: 20px;
        }
    </style>
     <script>
        // Fetch the LED state and update the webpage every second
        setInterval(() => {
            fetch('/LEDState')
                .then(response => response.text())
                .then(state => {
                    document.getElementById('ledState').textContent = state;
                });
        }, 500);
    </script>
</head>
<body>
    <h1>Welcome to the smartest smart-home system ever created</h1>
    <div class="card">
        <p>Smart home system to open the frontdoor remote, in case you have visitors</p>
        <form action="/LED" method="POST">
            <button class="button">Open door remote</button>
        </form>
        <p>Current door state: <span class="status" id="ledState">)rawliteral" + String(digitalRead(ledLocal) ? "Open" : "Closed") + R"rawliteral(</span></p>
    </div>
</body>
</html>
    )rawliteral");
}

/*
*
* @brief sents a high to the Arduino when handles the LED
*/
void handleLED() {
    digitalWrite(ledLocal, HIGH);

    delay(3000);

    digitalWrite(ledLocal, LOW); 
    server.sendHeader("Location", "/");
    server.send(303);
}

/*
*
* @brief Handles the LED state to be displayed on the webpage
*/

void handleLEDState() {
    String ledState = digitalRead(ledLocal) ? "Open" : "Closed";
    server.send(200, "text/plain", ledState);
}

/*
*
* @brief Error handling for 404: Not found
*/
void handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}
