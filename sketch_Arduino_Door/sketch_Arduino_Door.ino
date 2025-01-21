/**
 * @author Liou Xia & Oscar Sjelle
 * @file Arduino_door.ino
 *
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
#define LED_RED 2
#define LED_YELLOW 7
#define LED_GREEN 4
#define SOUND_SENSOR A0

//! grouped variables for state machine
enum State {
  IDLE,
  APPROVED,
  DENIED,
  PROCESSING,
  EXIT
};

//! create struct for rfid library
MFRC522 rfid(Select_PIN, RST_PIN);

//! Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//! UID keys
//byte storedKey[4] = { 0xC3, 0x44, 0x22, 0x4F };  //Oscars Key
byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0D };  // Lious Key

//! variables
byte uid[4];
int uidLength = 0;
int soundValue = 0;
int digValue = 0;
State currentState;
byte status = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  SPI.begin();
  rfid.PCD_Init();  //! init MFRC522 (model of rfid)
  initLcd();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(SOUND_SENSOR, INPUT);

  currentState = IDLE;

  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {

    /**
   * @brief the code is setup as a statemachine according to key value and what to display.
   *
   *
   */

  switch (currentState) {
    case IDLE:
      IdleState();
      checkNearbyRFID();
      delay(1000);

      break;

    case PROCESSING:
      ProcessState();
      RFIDREADER();

      delay(1000);
      break;

    case DENIED:
      DeniedState();
      //Alarm();
      //shutdown;

      currentState = IDLE;
      delay(1000);

      break;

    case APPROVED:
      ApprovedState();
      status = 1;
      Serial.write(status);

      /*
      motor do something
      send user UID to thingspeak
      
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

    case EXIT:
      ExitState();
      status = 0;
      Serial.write(status);
      currentState = IDLE;
      delay(1000);

      break;
  }
}

/**
 * @brief compares the stored key value with the read value of the scanned key. Returns true or false
 *
 * @param uid  scanned key value.
 * @param length  length of scanned key value.
 */

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
}

void writeToLCD(int row, int colm, char string[]) {
  lcd.clear();
  lcd.setCursor(colm, row);
  lcd.print(string);
}

void IdleState() {
  writeToLCD(0, 0, "Scanning...");
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void ProcessState() {
  writeToLCD(0, 0, "PROCESSING");
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void DeniedState() {
  writeToLCD(0, 0, "Acces Denied!");
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
}

void ApprovedState() {
  writeToLCD(0, 0, "Welcome: someone");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void ExitState() {
  writeToLCD(0, 0, "Bye bye");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
}


void checkNearbyRFID() {
  if (rfid.PICC_IsNewCardPresent()) {  // look to see if chip registers something nearby
    currentState = PROCESSING;
  }
}

void RFIDREADER() {
  if (rfid.PICC_ReadCardSerial()) {  // read chip
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    uidLength = rfid.uid.size;
    for (byte i = 0; i < uidLength; i++) {
      //Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uid[i] = rfid.uid.uidByte[i];
    }

    //printUID(uid, uidLength);

    //communication between for status
    if (isMatchingKey(uid, uidLength)) {
      if (status == 1) {
        currentState = EXIT;
      } else {
        currentState = APPROVED;
      }
    } else if (!isMatchingKey(uid, uidLength)) {
      currentState = DENIED;
    }

    rfid.PICC_HaltA();       // halt PICC
    rfid.PCD_StopCrypto1();  // stop encryption on PCD
  }
}

void printUID(byte* uid, int length) {
  Serial.print("Scanned UID: ");
  for (byte i = 0; i < uidLength; i++) {
    Serial.print(uid[i] < 0x10 ? " 0" : " ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();

  Serial.print("Stored key is: ");
  for (byte i = 0; i < sizeof(storedKey); i++) {
    Serial.print(storedKey[i] < 0x10 ? " 0" : " ");
    Serial.print(storedKey[i], HEX);
  }

  Serial.println();
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