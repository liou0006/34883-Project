/**
*@file sketch_Arduino_Room.ino
*
*
*@section description Description
*Logs data from the sensors and displays them. Data is also send to the ESP through UART for transmitting to ThingSpeak.
*
*
*
*@section circuit Circuit
*How to connect the DHT11:
*-Signal (DHT) to D4 (arduino)
*
*How to connect TMP36GZ:
*-Vout (TMP) to A0 (arduino)
*
*
*How to connect arduino to ESP8266:
*-D18 (arduino) to Rx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*-D19 (arduino) to Tx (ESP) (IMPORTANT!: when uploading the code to the ESP and arduino - remove this wire from the socket)
*
*How to connect servo:
*-D37 (arduino Mega) to servo signal 
*
*How to connect lcd:
*-D20 (arduino) to SDA (LCD)
*-D21 (arduino) to SCL (LCD)
*
*
*@section libraries Libraries
*-dht.h (Download a zip file of this library from this link: https://www.circuitbasics.com/how-to-set-up-the-dht11-humidity-sensor-on-an-arduino/)
*-LiquidCrystal_I2C.h (to use the LCD-display with the I2C module)
*-Servo.h (to control the position of servo)
*-LCDPrint.h (personal library for LCD)
*-TimerInterrupt.h (to make hardware interrupts)
*
*
*@section author Authors
*Created by Ask Krat, Christian Houmann, Mads Andersen & Lukas Tallbacka
*
*
*@date 20/1/2025
*
**/

#include <dht.h> ///< Include the library for DHT sensors
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <LCDPrint.h>

#define USE_TIMER_2 true
#include <TimerInterrupt.h>
#define TIMER_INTERVAL_MS  10000L

dht DHT; ///< Create a DHT object to interact with the DHT11 sensor
const byte DHT11_PIN = 4; ///< Pin for humidity sensor.

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo stuff
Servo servo;  ///< Create servo class.
const byte servoPin = 37;  ///< Pin for motor.
int servoPos = 0; ///< Position of servo.

byte MeasFlag = 0;  ///< Flag to take a sensor measurement.
byte count = 0;     ///< counter to only send every 6th reading to ThingSpeak.
int IsHome = 0;     ///< "Boolean" to track if someone is home.

const byte MovPin = 2;  ///< Pin for motion sensor.
byte AlarmFlag = 0;     ///< Flag to start alarm logic.

const byte BuzzerPin = 11;  ///< Pin for buzzer.
const byte RstButton = 5;   ///< Pin for alarm reset button.

void setup() {
  ITimer2.init();
  ITimer2.attachInterruptInterval(TIMER_INTERVAL_MS, TimerHandler);

  pinMode(MovPin, INPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(RstButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MovPin), MovISR, RISING);

  Serial.begin(9600);
  Serial1.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);

  servo.attach(servoPin);
  servo.write(servoPos);
}

void loop() {
  // If intruders are detected while no one is home, start the alarm!!
  if(AlarmFlag == 1) {
    while(digitalRead(RstButton) == 1){
      analogWrite(BuzzerPin, 128);
      delay(50);
      analogWrite(BuzzerPin, 0);
      delay(50);
    }
    analogWrite(BuzzerPin, 0);
    AlarmFlag = 0;
  }
  
  // Read sensor values every 10 seconds. Upload to backend every minute
  if(MeasFlag == 1){
    count += 1;
    int chk = DHT.read11(DHT11_PIN);  // Read the data from the DHT11 sensor
    int sensorValue = analogRead(A0); // Read the analog value from TMP36 temperature sensor on pin A0
    float tempValue = (5.0 / 1023.0) * sensorValue * 100.0; // Convert the analog value to a temperature in Celsius

    float humValue = DHT.humidity;  // The humidity value from the DHT device 

    lcd.clear();
    if(count == 6){
      Serial1.print(tempValue);
      Serial1.write(32);
      Serial1.print(humValue);
      count = 0;
    }

    // If someone is home, write values to LCD. If the temperature is too high, open "window"
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

  // If we have received something from UART, we assume it to be the IsHome status.
  if (Serial1.available() > 0) { 
    IsHome = Serial1.parseInt();
  }
}

/**
   @brief Timer2 interrupt service routine. Sets flag every 10 seconds.
*/
void TimerHandler(){
  MeasFlag = 1;
}

/**
   @brief Alarm interrupt service routine. Sets AlarmFlag when motion is detected while no one is home.
*/
void MovISR(){
  if(IsHome == 0){
    AlarmFlag = 1;
  }
}
