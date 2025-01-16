//#include <dht.h> TEMP
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
//#include <LCDPrint.h> TEMP


//dht DHT; TEMP
const byte DHT11_PIN 4

// Define the pins that are connected to the LCD-display
const byte rs = 2; ///< Reset pin for LCD.
const byte enable = 3; ///< Enable pin for LCD.
const byte D4 = 10;  ///< D4 pin for LCD.
const byte D5 = 11;  ///< D5 pin for LCD.
const byte D6 = 12;  ///< D6 pin for LCD.
const byte D7 = 13;  ///< D7 pin for LCD.
LiquidCrystal lcd(rs, enable, D4, D5, D6, D7);

/// Degree symbol for the LCD.
const byte deg[8] = {
  B00110, B01001, B01001, B00110,
  B00000, B00000, B00000,
};

/// Software serial setup
int data;                    //Initialized variable to store recieved data
SoftwareSerial s(5,6);       //Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU

// Servo stuff
Servo servo;  ///< Create servo class.
const byte servoPin = 37;  ///< Pin for motor position.
int servoPos = 0; ///< Position of servo.

//count used for running certain code intermittently 
unsigned int count

void setup() {
  Serial.begin(9600);
  s.begin(9600);

  lcd.createChar(0, deg);
  lcd.begin(16,2);

  servo.attach(servoPin);
  servo.write(servoPos);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  // If we have received 2 floats (2x4 bytes) and some other split character, we read the data and post it to the LCD
  if(Serial1.available() >= 9){
    float T = Serial1.parseFloat();
    float H = Serial1.parseFloat();
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

  // logging sensor values every 2 seconds
  if(count%20 == 0){
    int sensorValue = analogRead(A0);
    float tempValue = (5.0 / 1023.0) * sensorValue * 100;

    //s.print(String(DHT.humidity)+ "," + String(tempValue) + "\n"); TEMP
    //Serial.println(DHT.humidity); TEMP
    //Serial.println(String(tempValue)); TEMP
  }
  
  delay(10);
  count++
}
