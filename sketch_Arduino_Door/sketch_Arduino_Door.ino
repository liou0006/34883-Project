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
#include <LCDPrint.h>

//! defines
#define Select_PIN 10
#define RST_PIN 5
#define LED_RED 2
#define LED_YELLOW 8
#define LED_GREEN 4
#define SOUND_SENSOR A0
#define SERVERDOOR 8

//! grouped variables for state machine
enum State {
  IDLE,
  APPROVED,
  SERVER,
  DENIED,
  PROCESSING,
  EXIT
};

//! create struct for rfid library
MFRC522 rfid(Select_PIN, RST_PIN);

//! Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//! UID keys
byte storedKey[4] = { 0xC3, 0x44, 0x22, 0x4F };  //Oscars Key
//byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0D };  // Lious Key

//! variables
byte uid[4];
int uidLength = 0; //! length of written user key, used in isMatchingKey function
State currentState;
byte isHome = 0; //! boolean value sent to thingSpeak when user enters or leaves the house

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  SPI.begin();
  rfid.PCD_Init();  //! init MFRC522 (model of rfid)
  initLcd();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SERVERDOOR, INPUT);

  currentState = IDLE;

}

  /**
   * the code is setup as a statemachine according to key value and what to display.
   *
   */
  
void loop() {
  switch (currentState) {
    case IDLE:
      IdleState();
      checkNearbyRFID();

      if (digitalRead(SERVERDOOR) && isHome == 1){
        currentState = SERVER; 
      } 

      delay(1000);

      break;

    case PROCESSING:
      ProcessState();
      RFIDREADER();
      delay(1000);

      break;

    case DENIED:
      DeniedState();
      currentState = IDLE;
      delay(1000);

      break;

    case APPROVED:
      ApprovedState();
      isHome = 1;
      Serial.write(isHome);
      currentState = IDLE;
      delay(2000);

      break;

    case SERVER:
      ApprovedServerState();
      delay(3000);
      currentState = IDLE;

      break;

    case EXIT:
      ExitState();
      isHome = 0;
      Serial.write(isHome);
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
    return false;  // UID lengths do not match
  }

  for (byte i = 0; i < length; i++) {
    if (uid[i] != storedKey[i]) {
      return false;  // Mismatch found
    }
  }
  return true;  // All bytes match
}

void IdleState() {
  writeToLCD(0, 0, "Scanning...");
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
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
  delay(250);
  digitalWrite(LED_RED, LOW);
  delay(250);
  digitalWrite(LED_RED, HIGH);
  delay(250);
  digitalWrite(LED_RED,LOW);
  delay(250);
}

void ApprovedState() {
  writeToLCD(0, 0, "Welcome home");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void ApprovedServerState() {
  writeToLCD(0, 0, "Remote access");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void ExitState() {
  writeToLCD(0, 0, "Leaving house");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
}

/**
 * @brief checks if there is a RFID chip nearby.
 * 
 */
void checkNearbyRFID() {
  if (rfid.PICC_IsNewCardPresent()) {  
    currentState = PROCESSING;
  }
}

/**
 * @brief reads the RFID chip and compares it with the stored key value.
 * 
 */
void RFIDREADER() {
  if (rfid.PICC_ReadCardSerial()) {  // read chip
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    uidLength = rfid.uid.size;
    for (byte i = 0; i < uidLength; i++) {
      uid[i] = rfid.uid.uidByte[i];
    }

    //printUID(uid, uidLength);

    //communication between for status
    if (isMatchingKey(uid, uidLength)) {
      if (isHome == 1) {
        currentState = EXIT;
      } else {
        currentState = APPROVED;
      }
    } else if (!isMatchingKey(uid, uidLength)) {
      currentState = DENIED;
    }

    rfid.PICC_HaltA();       // halt PICC
    rfid.PCD_StopCrypto1();  // stop encryption on PCD
  } else {
    currentState = IDLE;
  }
}

/**
 * @brief prints the scanned UID and the stored key value.
 * 
 */
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
