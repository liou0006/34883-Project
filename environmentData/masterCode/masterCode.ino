#include <dht.h>

dht DHT;

#define DHT11_PIN 12

int data;                    //Initialized variable to store recieved data
#include <SoftwareSerial.h>  //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
//defined the pins for softwareserial pins 5 and 6
SoftwareSerial s(5,6);

void setup() {
  //starts the program
  Serial.begin(9600);
  s.begin(9600);



}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  // Serial.println("part 1--------- start");
  // Serial.print("Humidity = ");
   //Serial.println(DHT.humidity);
  // Serial.println("part 1--------- end");
  // delay(1000);
  // Serial.println("part 2--------- start");
   int sensorValue = analogRead(A0);
  // Serial.print("Analog Value: ");
  // Serial.print(sensorValue);
  //float voltage = sensorValue * (5.0 / 1023.0);
  // Serial.print("\tVoltage: ");
  // Serial.print(voltage);
  //calculates the tempValue
   float tempValue = (5.0 / 1023.0) * sensorValue * 100;
  // Serial.print(" Temperature Value: ");
  // Serial.print(tempeValue);
  // Serial.println("\u00B0");
  // Serial.println("part 2--------- end");


  // Format the temperature and humidity as strings
  //String temperatureMessage = "Temperature: " + String(tempeValue) + "\n";
  //String humidityMessage = "Humidity: " + String((float) DHT.humidity) + "\n";

  // Send the messages via SoftwareSerial
  //Serial.println("Sending data...");
  //mySerial.write((byte*)&tempeValue, sizeof(tempeValue));
  //mySerial.write((byte*)&DHT.humidity, sizeof(DHT.humidity));

  //mySerial.print(tempeValue);
  //mySerial.print(DHT.humidity);
  // if (s.write(123)) {
    
  //   Serial.println("hello world");
  // }

  //s.write(DHT.humidity); // Send the tuple as a string
  //Serial.println(DHT.humidity);
  //sends the humidity and tempValue to the ESP8266 module
  s.print(String(DHT.humidity)+ "," + String(tempValue) + "\n");
  Serial.println(DHT.humidity);
  Serial.println(String(tempValue));
  //Serial.println(DHT.temperature);
  //Serial.println("hello world");
  
  delay(2000);
}
