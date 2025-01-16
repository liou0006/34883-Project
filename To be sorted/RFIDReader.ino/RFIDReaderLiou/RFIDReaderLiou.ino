/*
* a public library for RFID has been downloaded
 * Created by ArduinoGetStarted.com
 *
 * The code has been modified for our purpose
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
//#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>

//! defines
#define Select_PIN 10
#define RST_PIN 5
#define LED_RED 2
#define LED_YELLOW 7
#define LED_GREEN 4
#define SOUND_SENSOR A0
#define EEPROM_ADDRESS 0

enum State {
  IDLE,
  APPROVED,
  DENIED,
  PROCESSING
};

//! create struct for rfid library
MFRC522 rfid(Select_PIN, RST_PIN);

//! Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//! variables
byte uid[4];
byte storedKey[4] = { 0xC3, 0x44, 0x22, 0x4F }; //Oscars Key
//byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0D };  // Lious Key
//byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0E }; //tset


int uidLength = 0;
int flagKey = 0;  //! wait = 0, approve = 1, deny = 2
int soundValue = 0;
int digValue = 0;
int TSField = 1;
State currentState = IDLE;

// Wi-Fi configuration
const char* ssid = "Ask Krat";
const char* pass = "feature-hollow-truly";
// WiFiClient client;
// ThingSpeak configuration
unsigned long channelID = 2808283;
// Replace with your ThingSpeak channel ID
const char* APIWriteKey = "G4QFBJM48LQQLI4T";  //
const char* APIReadKey = "PUSZ92SJXXMO8BDG";
const int postDelay = 20 * 1000;  // 20 seconds delay
int data;                         //Initialized variable to store recieved data
//the above is set correctly

SoftwareSerial mySerial(1, 3);  //RX, TX


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  //WiFi.begin(ssid, pass);

  SPI.begin();
  rfid.PCD_Init();  //! init MFRC522 (model of rfid)
  Serial.println("Tap RFID/NFC Tag on reader");

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(SOUND_SENSOR, INPUT);

  currentState = IDLE;

  initLcd();
}

void loop() {
  //soundListen();

  switch (currentState) {
    case IDLE:
      Serial.println("In: Idle");
      writeToLCD(0, 0, "Scanning...");
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      //lcdAccesWait();
      if (rfid.PICC_IsNewCardPresent()) {
        currentState = PROCESSING;
      }

      delay(1000);
      break;

    case PROCESSING:
      Serial.println("In: Processing");
      writeToLCD(0, 0, "PROCESSING");
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);

      if (rfid.PICC_IsNewCardPresent()) {  // look to see if chip registers something nearby
        if (rfid.PICC_ReadCardSerial()) {  // read chip
          MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

          uidLength = rfid.uid.size;

          for (byte i = 0; i < uidLength; i++) {
            //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
            uid[i] = rfid.uid.uidByte[i];
          }

          Serial.print("Scanned UID: ");
          for (byte i = 0; i < uidLength; i++) {
            Serial.print(uid[i] < 0x10 ? " 0" : " ");
            Serial.print(uid[i], HEX);
          }
          Serial.println();

          Serial.print("Stored key is: ");
          for (byte i = 0; i < sizeof(storedKey); i++) {
            Serial.print(uid[i] < 0x10 ? " 0" : " ");
            Serial.print(uid[i], HEX);
          }

          Serial.println();

          // Serial.print("EEPROM key is: ");
          // for (byte i = 0; i < sizeof(4); i++) {
          //   Serial.print(keyFromEEPROM[i] < 0x10 ? " 0" : " ");
          //   Serial.print(keyFromEEPROM[i], HEX);
          // }

          Serial.println();

          // for (byte i = 0; i < uidLength; i++) {
          //   //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          //   ThingSpeak.setField(3, uid[i]);
          // }

          // int responseCode = ThingSpeak.writeFields(channelID, APIWriteKey);
          // if (responseCode == 200) {
          //   Serial.println("Data sent success");
          // } else {
          //   Serial.println("Failed to send");
          // }

          if (isMatchingKey(uid, uidLength)) {

            /*
            motor do something
            send user UID to thingspeak
            */

            Serial.println("Key approved");

            // for (byte i = 0; i < uidLength; i++) {
            //   //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
            //   EEPROM.write(EEPROM_ADDRESS + (i * 1), uid[i]);
            // }

            // for (byte i = 0; i < uidLength; i++) {
            //   //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
            //   Serial.println(EEPROM.read(EEPROM_ADDRESS + (i * 1)), HEX);
            // }

            //sizeof(float

            currentState = APPROVED;
          } else if (!isMatchingKey(uid, uidLength)) {
            Serial.println("Wrong key");
            writeToLCD(0, 0, "No Access");
            currentState = DENIED;
          }

          rfid.PICC_HaltA();       // halt PICC
          rfid.PCD_StopCrypto1();  // stop encryption on PCD
        }
      }
      delay(1000);
      break;

    case DENIED:
      Serial.println("In: DENIED");
      lcdAccesDenied();
      //do alarm
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      delay(1000);

      currentState = IDLE;

      break;

    case APPROVED:
      Serial.println("In: APPROVED");
      writeToLCD(0, 0, "Welcome: someone");
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);

      /*
      check for if the temperature and humidity is good. need to receive data from tingspeak
      if not
      open windows / motor
      show warning type of temperature
      else
      show temperature and humidity
      */


      currentState = IDLE;

      delay(2000);
      break;
  }
}


bool isMatchingKey(byte* uid, int length) {
  if (length != sizeof(storedKey)) {
    Serial.println("Mismatch in length");
    return false;  // UID lengths do not match
  }

  for (byte i = 0; i < length; i++) {
    if (uid[i] != storedKey[i]) {
      Serial.println("Mismatch in loop");
      return false;  // Mismatch found
    }
  }
  return true;  // All bytes match
}

void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan card...");
}

void lcdAccesWait() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan card...");
}

void lcdAccesGain() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acces granted! ");
  lcd.setCursor(0, 1);
  lcd.print("User: ");
  lcd.print("Oscar");
}

void lcdAccesDenied() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acces Denied!");
}

// void alarmLED() {
//   for (int i = 0; i <= 5; i++) {
//     digitalWrite(RED_LED, LOW);
//     delay(200);
//     digitalWrite(RED_LED, HIGH);
//     delay(200);
//   }
// }

// void soundListen() {
//   //for (int i = 0; i<32;i++){
//   //soundValue += analogRead(SOUND_SENSOR);
//   //}
//   soundValue = analogRead(SOUND_SENSOR);
//   //digValue = digitalRead(7);
//   //soundValue >>= 5;
//   Serial.println(soundValue, DEC);
//   Serial.println(digValue);
//   if (soundValue > 60) {
//     digitalWrite(GREEN_LED, HIGH);
//     delay(200);
//     digitalWrite(GREEN_LED, LOW);
//     delay(200);
//   }

//   delay(10);
// }

void writeToLCD(int row, int colm, char string[]) {
  lcd.clear();
  lcd.setCursor(colm, row);
  lcd.print(string);
}

void readDataFromTS() {
  // ThingSpeak.begin(client);
  // client.connect(server,80);
  // ThingSpeak.read
}


// void sendDataToTS(){
//   ThingSpeak.begin(client);
//   client.connect(server,80);
//   ThingSpeak.setField(TSField,uid);)
//   ThingSpeak.writeFields(channelID, writeAPIKey);

//   /*
//   If access denied or approved we want to log that
//   We want to be able to add new user, and delete old users
//   */
//   client.stop();
//   delay(postDelay);
// };
