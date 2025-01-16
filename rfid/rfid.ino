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

//! defines
#define Select_PIN 10
#define RST_PIN 5
#define RED_LED 3
#define GREEN_LED 2
#define SOUND_SENSOR A0
#define MS_PIN 8
#define YELLOW_LED 4 

//! create struct for rfid library
MFRC522 rfid(Select_PIN, RST_PIN);

//! Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//! variables
byte uid[4];
byte storedKey[4] = { 0xC3, 0x44, 0x22, 0x4F };
int uidLength = 0;
int flagKey = 0; //! wait = 0, approve = 1, deny = 2
int soundValue = 0; 
int digValue = 0; 
int ms_state = 0; 


void setup() {
  Serial.begin(9600);

  SPI.begin();
  rfid.PCD_Init();  //! init MFRC522 (model of rfid)
  Serial.println("Tap RFID/NFC Tag on reader");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(7,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MS_PIN,INPUT); 
  pinMode(YELLOW_LED,OUTPUT);
  //pinMode(SOUND_SENSOR, INPUT); 

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);


  initLcd();
}

void loop() {
  soundListen();
  motionDetect();

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
        Serial.print(uid[i], HEX);
        if (i < uidLength - 1) Serial.print(":");
      }
      Serial.println();
      Serial.print("Stored key is: ");
      for (byte i = 0; i < sizeof(storedKey); i++) {
        Serial.print(storedKey[i], HEX);
        if (i < sizeof(storedKey) - 1) Serial.print(":");
      }

      Serial.println();

      if (isMatchingKey(uid, uidLength)) {
        Serial.println("Key approved");
        flagKey = 1;
      } else if (!isMatchingKey(uid, uidLength)) {
        Serial.println("Wrong key");
        flagKey = 2;
      } else {
        flagKey = 0;
      }


      if (flagKey == 0) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        lcdAccesWait();
      } else if (flagKey == 2) {
        lcdAccesDenied();
        alarmLED();
        lcdAccesWait();
      } else {
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        lcdAccesGain();
        delay(2000);
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        flagKey = 0;
        lcdAccesWait();
      }
    

    rfid.PICC_HaltA();       // halt PICC
    rfid.PCD_StopCrypto1();  // stop encryption on PCD
  }
}
}


bool isMatchingKey(byte *uid, int length) {
  if (length != sizeof(storedKey)) {
    Serial.print("mismatch in length");
    return false;  // UID lengths do not match
  }

  for (byte i = 0; i < length; i++) {
    if (uid[i] != storedKey[i]) {
      Serial.print("mismatch in loop");
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
  lcd.print("Scan card... ");
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
  lcd.print("Acces Denied! ");
}

void alarmLED() {

  for (int i = 0; i <= 5; i++) {
    digitalWrite(RED_LED, LOW);
    delay(200);
    digitalWrite(RED_LED, HIGH);
    delay(200);
  }
}

void soundListen() {
  //for (int i = 0; i<32;i++){
    //soundValue += analogRead(SOUND_SENSOR);
  //}
  soundValue = analogRead(SOUND_SENSOR);
  //digValue = digitalRead(7);
  //soundValue >>= 5; 
  Serial.println(soundValue, DEC);
Serial.println(digValue);
  if (soundValue > 60) { 
    digitalWrite(GREEN_LED,HIGH);
    delay(200);
    digitalWrite(GREEN_LED, LOW); 
    delay(200);
  }

  delay(10);

}

void motionDetect() {
  ms_state = digitalRead(MS_PIN); 

  if(ms_state) {
  digitalWrite(YELLOW_LED,HIGH);
  } else {
  digitalWrite(YELLOW_LED,LOW);
  }

}
