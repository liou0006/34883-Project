#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <ESP8266mDNS.h>

// Wi-Fi configuration
WiFiClient client;
ESP8266WebServer server(80);
// const char* ssid = "Ask Krat";
// const char* pass = "feature-hollow-truly";
//const char* ssid = "X_Liou";
//const char* pass = "Teemo4life";

const char* ssid = "Oscar Iphone";
const char* pass = "gggggggg";
const int ledLocal = D3;  // Remote LED


//! ThingSpeak configuration
unsigned long channelID = 2808283;
const char* APIWriteKey = "G4QFBJM48LQQLI4T";
const char* APIReadKey = "PUSZ92SJXXMO8BDG";
const int postDelay = 15 * 1000;  // 15 seconds delay

int responseCode = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ledLocal, OUTPUT);
  digitalWrite(ledLocal, LOW);

  while (!Serial) { ; }
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
  serverInit();



}

void loop() {

  serverLoop();

  if (Serial.available() > 0) {
    byte oneBite = Serial.read();
    Serial.println(oneBite);

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

void serverLoop() {
  server.handleClient();
  // Periodically sync local LED state to ThingSpeak, remote LED from ThingSpeak, and handle RSSI data
  static unsigned long lastSyncTime = 0;
  if (millis() - lastSyncTime > 200) {
    lastSyncTime = millis();

    //handleRSSIData();

    //syncLocalLEDToThingSpeak();
    //syncRSSIToThingSpeak();

    //if (ThingSpeak.writeFields(channelID, APIWriteKey)) {
    //  Serial.println("Data successfully sent to ThingSpeak.");
    //} else {
    //  Serial.println("Failed to send data to ThingSpeak.");
    //}

    //syncRemoteLEDFromThingSpeak();
  }
}

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

void handleLED() {
    // Toggle the local LED state
    digitalWrite(ledLocal, HIGH);

    // Update Field 2 on ThingSpeak with the local LED state
    //syncLocalLEDToThingSpeak();
  

    delay(3000);

    digitalWrite(ledLocal, LOW); 
    server.sendHeader("Location", "/");
    server.send(303);
}

void handleLEDState() {
    String ledState = digitalRead(ledLocal) ? "Open" : "Closed";
    server.send(200, "text/plain", ledState);
}

void handleNotFound() {
    server.send(404, "text/plain", "404: Not found");
}

