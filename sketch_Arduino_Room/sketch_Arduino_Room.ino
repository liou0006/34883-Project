/**
*@file masterCode.ino
*
*
*@section description Description
*Sending data between arduino Uno R3 and ESP8266 NodeMCU while reading values from a temperature sensor (analog) and a humidity sensor (digital)
*and uploading the data to thingSpeak to show a graph of the humidity and temperature 
*
*
*@section setup Setup
*Setup:
*-baud rate = 9600 on both arduino and ESP
*
*
*@section circuit Circuit
*How to connect the DHT11:
*-Signal (DHT) to D12 (arduino)
*-Vcc (DHT) to power (5V - breadboard)
*-GND (DHT) to GND (breadboard)
*
*How to connect TMP36GZ:
*-Vs (TMP) to GND (breadboard)
*-Vout (TMP) to A0 (arduino)
*-Gnd (TMP) to GND (arduino)
*
*Setup the breadboard with GND and 5V:
*-5V (arduino) to + (breadboard)
*-GND (arduino) to - (breadboard) 
*
*How to connect arduino to ESP8266:
*-D6 (arduino) to Rx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-D5 (arduino) to Tx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-GND (ESP) to GND (breadboard)
*
*Connect LED to ESP8266:
*-cathode (LED) to D2 (ESP)
*-anode (LED) to GND (breadboard) 
*
*
*@section libraries Libraries
*-dht.h (Download a zip file of this library from this link: https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/)
*-SoftwareSerial.h
*
*
*@section author Authors
*Created by Ask Krat, Christian Houmann
*
*
*@date Today
*
**/



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
  if(sensorFlag == 6){
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
    if(tempValue >= 35){
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
  sensorFlag += 1;
}
