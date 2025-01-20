/**
 * @file Dummy_ESP.ino
 *
 * @section description Test
 * Does this even work?
 *
 * @author Mads Andersen & Lukas Tallbacka
 */


#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <LCDPrint.h>

#define USE_TIMER_2 true
#include <TimerInterrupt.h>
#define TIMER_INTERVAL_MS  10000L

dht DHT;
const byte DHT11_PIN = 4;

LiquidCrystal_I2C lcd(0x27, 16, 2);

/// Degree symbol for the LCD.
const byte deg[8] = {
  B00110, B01001, B01001, B00110,
  B00000, B00000, B00000,
};

// Servo stuff
Servo servo;  ///< Create servo class.
const byte servoPin = 37;  ///< Pin for motor position.
int servoPos = 0; ///< Position of servo.

byte sensorFlag = 0;


void setup() {
  ITimer2.init();
  ITimer2.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler);

  Serial.begin(9600);
  Serial1.begin(9600);

  lcd.createChar(0, deg);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  servo.attach(servoPin);
  servo.write(servoPos);
}

void loop() {
  // logging sensor values every 2 seconds
  if(sensorFlag == 1){
    int chk = DHT.read11(DHT11_PIN);
    int sensorValue = analogRead(A0);
    float tempValue = (5.0 / 1023.0) * sensorValue * 100;
    float humValue = DHT.humidity;
    lcd.clear();
    lcdPrintData(lcd, tempValue, humValue);
    Serial.println(humValue); 
    Serial.println(String(tempValue));

    Serial1.print(tempValue);
    Serial1.write(32);
    Serial1.print(humValue);

    // If the temperature is too high, "open window"
    if(tempValue >= 22){
      servoPos = 90;
      servo.write(servoPos);
    } else {
      servoPos = 0;
      servo.write(servoPos);
    }

    sensorFlag = 0;
  }
}


void TimerHandler()
{
  sensorFlag = 1;
}
