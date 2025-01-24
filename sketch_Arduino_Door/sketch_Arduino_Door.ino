/**
 *@file sketch_Arduino_Door.ino
 *
 *
 *@section description Description
 *The code is set up as a state machine that handles the RFID reader according to the incoming UID key value. Depending on the key value the state machine will display a corresponding message based of the key value. If a valid key value is scanned the system will send a isHome signal to the EP8266.
 *
 *
 *@section circuit Circuit
 *<b>LED wiring</b>\n 
 *-Denied LED (Red) to D2.\n 
 *-Processing LED (Yellow) to D8.\n 
 *-Approved LED (Green) to D4.
 *
 *
 *<b>Connection to ESP8266</b>\n 
 *-TX (UNO) to RX (ESP)\n 
 *-RX (UNO) to TX (ESP)
 *
 *<b>RFID wiring</b>\n 
 *-Vcc to 3.3V\n
 *-RST to D5\n
 *-GND to GND\n 
 *-MISO to D11\n
 *-MOSI to D10\n
 *-SCK to D9\n
 *-SS to D12
 *
 *<b>LCD wiring</b>\n 
 *-D20/SDA to SDA\n 
 *-D21/SCL to SCL
 *
 *
 *@section libraries Libraries
 *-SPI.h (Default library for configuring SPI communication)\n 
 *-MFRC522.h (Link to the used library: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc)\n 
 *-LCDPrint.h (Custom library for common LCD functions)
 *
 *
 *@section author Authors
 *Created by Liou Xia & Oscar Sjelle
 *
 *@date 24/1/2025
 *
 **/

#include <SPI.h>
#include <MFRC522.h>
#include <LCDPrint.h>

//Define pins
#define Select_PIN 10
#define RST_PIN 5
#define LED_RED 2       
#define LED_YELLOW 8
#define LED_GREEN 4
#define SOUND_SENSOR A0
#define SERVERDOOR 7


//! Grouped variables for state machine
enum State
{
  IDLE,
  APPROVED,
  SERVER,
  DENIED,
  PROCESSING,
  EXIT
};

/// Create struct for RFID library
MFRC522 rfid(Select_PIN, RST_PIN);

/// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// UID keys
byte storedKey[4] = {0xC3, 0x44, 0x22, 0x4F}; // Oscars Key
// byte storedKey[4] = { 0xE1, 0x0B, 0xCB, 0x0D };  // Lious Key

// Program variables
byte uid[4]; ///< variable used to store the scanned key
int uidLength = 0; ///< length of written user key, used in isMatchingKey function
State currentState; ///< variable used to control the state machine
byte isHome = 0; ///< boolean value sent to thingSpeak when user enters or leaves the house

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }

  SPI.begin();
  rfid.PCD_Init(); // init MFRC522 (model of rfid)
  initLcd(lcd);

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

void loop()
{
  switch (currentState)
  {
  case IDLE:
    IdleState();
    checkNearbyRFID();

    if (digitalRead(SERVERDOOR) && isHome == 1)
    {
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

bool isMatchingKey(byte *uid, int length)
{
  if (length != sizeof(storedKey))
  {
    return false; ///< return false if UID lengths do not match
  }

  for (byte i = 0; i < length; i++)
  {
    if (uid[i] != storedKey[i])
    {
      return false; ///< return false if UID values do not match
    }
  }
  return true; ///< return true if length and UID values matches
}

void IdleState()
{
  writeToLCD(lcd, 0, 0, "Scanning...");
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
}

void ProcessState()
{
  writeToLCD(lcd, 0, 0, "PROCESSING");
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void DeniedState()
{
  writeToLCD(lcd, 0, 0, "Acces Denied!");
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  delay(250);
  digitalWrite(LED_RED, LOW);
  delay(250);
  digitalWrite(LED_RED, HIGH);
  delay(250);
  digitalWrite(LED_RED, LOW);
  delay(250);
}

void ApprovedState()
{
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  lcdPrintWelcome(lcd);
}

void ApprovedServerState()
{
  writeToLCD(lcd, 0, 0, "Remote access");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
}

void ExitState()
{
  writeToLCD(lcd, 0, 0, "Leaving house");
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
}

/**
 * @brief checks if there is a RFID chip nearby.
 *
 */
void checkNearbyRFID()
{
  if (rfid.PICC_IsNewCardPresent())
  {
    currentState = PROCESSING;
  }
}

/**
 * @brief reads the RFID chip and compares it with the stored key value to determine the next-state
 *
 */
void RFIDREADER()
{
  if (rfid.PICC_ReadCardSerial())
  { // read chip
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    uidLength = rfid.uid.size;
    for (byte i = 0; i < uidLength; i++)
    {
      uid[i] = rfid.uid.uidByte[i];
    }

    // printUID(uid, uidLength);

    if (isMatchingKey(uid, uidLength))
    {
      if (isHome == 1)
      {
        currentState = EXIT;
      }
      else
      {
        currentState = APPROVED;
      }
    }
    else if (!isMatchingKey(uid, uidLength))
    {
      currentState = DENIED;
    }

    rfid.PICC_HaltA();      // halt PICC
    rfid.PCD_StopCrypto1(); // stop encryption on PCD
  }
  else
  {
    currentState = IDLE;
  }
}

/**
 * @brief prints the scanned UID and the stored key value.
 *
 */
void printUID(byte *uid, int length)
{
  Serial.print("Scanned UID: ");
  for (byte i = 0; i < uidLength; i++)
  {
    Serial.print(uid[i] < 0x10 ? " 0" : " ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();

  Serial.print("Stored key is: ");
  for (byte i = 0; i < sizeof(storedKey); i++)
  {
    Serial.print(storedKey[i] < 0x10 ? " 0" : " ");
    Serial.print(storedKey[i], HEX);
  }
  Serial.println();
}
