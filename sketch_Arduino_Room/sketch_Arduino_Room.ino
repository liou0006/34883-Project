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

byte MeasFlag = 0;
byte count = 0;
int IsHome = 0;

const byte MovPin = 2;
byte MovFlag = 0;

const byte BuzzerPin = 11;
const byte RstButton = 5;

void setup() {
  ITimer2.init();
  ITimer2.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler);

  pinMode(MovPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(RstButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MovPin), MovISR, RISING);

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

  if(MovFlag == 1) {
    while(digitalRead(RstButton) == 1){
      analogWrite(BuzzerPin, 128);
      delay(50);
      analogWrite(BuzzerPin, 0);
      delay(50);
    }
    analogWrite(BuzzerPin, 0);
    MovFlag = 0;
  }
  // logging sensor values every 60 seconds
  if(MeasFlag == 1){
    count += 1;
    int chk = DHT.read11(DHT11_PIN);
    int sensorValue = analogRead(A0);
    float tempValue = ((float)map(sensorValue, 0, 1023, 0, 5000))/10.0;

    float humValue = DHT.humidity;
    lcd.clear();
    if(count == 6){
      Serial1.print(tempValue);
      Serial1.write(32);
      Serial1.print(humValue);
      count = 0;
    }

    if (IsHome == 1) {
      lcdPrintData(lcd, tempValue, humValue);

      // If the temperature is too high, "open window"
      if(tempValue >= 35){
        servoPos = 90;
        servo.write(servoPos);
      } else {
        servoPos = 0;
        servo.write(servoPos);
      }
    }
    MeasFlag = 0;
  }

  if (Serial1.available() > 0) { 
    IsHome = Serial1.parseInt();
  }
}


void TimerHandler(){
  MeasFlag = 1;
}

void MovISR(){
  if(IsHome == 0){
    MovFlag = 1;
  }
}
