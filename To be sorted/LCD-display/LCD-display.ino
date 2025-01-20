/**
 * @file LCD-display.ino
 *
 * @author Lukas Tallbacka & Mads Andersen
 */

#include <LiquidCrystal.h>
#include <Servo.h>
#include "LCDPrint.h"

// Define the pins that are connected to the LCD-display
uint8_t rs = 2; ///< Reset pin for LCD.
uint8_t enable = 3; ///< Enable pin for LCD.
uint8_t D4 = 10;  ///< D4 pin for LCD.
uint8_t D5 = 11;  ///< D5 pin for LCD.
uint8_t D6 = 12;  ///< D6 pin for LCD.
uint8_t D7 = 13;  ///< D7 pin for LCD.
LiquidCrystal lcd(rs, enable, D4, D5, D6, D7);


/// Degree symbol for the LCD.
const byte deg[8] = {
  B00110, B01001, B01001, B00110,
  B00000, B00000, B00000,
};

/* Define other used pins
uint8_t flagDisplay = ;
other stuff
*/

// Servo stuff
Servo servo;  ///< Create servo class.
uint8_t servoPin = 37;  ///< Pin for motor position.
int servoPos = 0; ///< Position of servo.


void setup() {
  lcd.createChar(0, deg);
  lcd.begin(16,2);

  servo.attach(servoPin);
  servo.write(servoPos);
  Serial.begin(9600);
  Serial1.begin(9600);
}


void loop() {
  // If we have received 2 floats (2x4 bytes) and some other split character, we read the data and post it to the LCD
  if(Serial1.available() >= 0){
    float T = 0;
    float H = 0;
    //getData(&T, &H);
    
    lcd.clear();
    lcdPrintWelcome(lcd);
    lcdPrintData(lcd, T, H);

    // If the temperature is too high, "open window"
    if(T >= 22){
      servoPos = 90;
      servo.write(servoPos);
    } else {
      servoPos = 0;
      servo.write(servoPos);
    }
  }
  
  // Perhaps do some other stuff
}
