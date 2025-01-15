/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>
// #include <ESP module>
// #include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 5
#define LED_RED 2
#define LED_YELLOW 7
#define LED_GREEN 4

MFRC522 rfid(SS_PIN, RST_PIN);

enum State {
  IDLE,
  APPROVED,
  DENIED,
  PROCESSING
};

// could be wrong initalization
//LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address and dimensions as per your LCD

State currentState = IDLE;
int32_t counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  // lcd.init();
  SPI.begin();      // init SPI bus
  rfid.PCD_Init();  // init MFRC522


  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {

  switch (currentState) {
    case IDLE:
      Serial.println("In: Idle");
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);
      delay(1000);


      /*
    lcd.setcursor(0,1);
    lcd.print("Scanning...");

    */

      if (rfid.PICC_IsNewCardPresent()) {
        currentState = PROCESSING;
      }
      break;

      //RFIDReader();

    case DENIED:
      Serial.println("In: Denied");
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
      /*
      lcd.print access denied
      led flashing red
      shut down

      state = I;
      */
      currentState = IDLE;
      delay(1000);
      break;

    case APPROVED:
      Serial.println("In: APPROVED");
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_YELLOW, LOW);

      /*
lcd.setcursor(0,0);
lcd.print welcome home
led flash green
thinkspeakwrite send UID data
thinkspeakread temperature
thinkspeakread humidity

//at the end
state = idle
*/
      currentState = DENIED;
      delay(1000);
      break;

    case PROCESSING:
      Serial.println("In: Processing");
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      /*
  lcd.setcursor(0,1);
  lcd.print("Wait...");
  do i need the below
  if UID != matchin
  state = 'D';
  else if == matching
  state = 'A';

  //if UID is the same go back to idle or don't print it out again. only do it one clock cycle
  else state = 'I';
*/
      if (rfid.PICC_ReadCardSerial()) {  // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        // Serial.print("RFID/NFC Tag Type: ");
        // Serial.println(rfid.PICC_GetTypeName(piccType));

        // print NUID in Serial Monitor in the hex format
        Serial.print("UID:");

        // think of how to print ID to LCD
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();

        rfid.PICC_HaltA();       // halt PICC
        rfid.PCD_StopCrypto1();  // stop encryption on PCD
      }

      if(E10BCB0D,HEX){
      currentState = APPROVED;

      }else {
        currentState = DENIED;
      }
      // }

      delay(1000);
      break;
  }
  delay(1000);
  //counter++;
}

void RFIDReader() {
  if (rfid.PICC_IsNewCardPresent()) {  // new tag is available

    if (rfid.PICC_ReadCardSerial()) {  // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      // Serial.print("RFID/NFC Tag Type: ");
      // Serial.println(rfid.PICC_GetTypeName(piccType));

      // print NUID in Serial Monitor in the hex format
      Serial.print("UID:");

      // think of how to print ID to LCD
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();

      rfid.PICC_HaltA();       // halt PICC
      rfid.PCD_StopCrypto1();  // stop encryption on PCD
    }
  }
  // change to state approce if new card is present i think
  //currentState = APPROVED;
}
 