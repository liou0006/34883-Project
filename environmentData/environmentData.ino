#include <dht.h>

dht DHT;

#define DHT11_PIN 12

int data;                    //Initialized variable to store recieved data
#include <SoftwareSerial.h>  //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial mySerial = SoftwareSerial(3, 4);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);



}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  Serial.println("part 1--------- start");
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  Serial.println("part 1--------- end");
  delay(1000);
  Serial.println("part 2--------- start");
  int sensorValue = analogRead(A0);
  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("\tVoltage: ");
  Serial.print(voltage);
  float tempeValue = (5.0 / 1023.0) * sensorValue * 100;
  Serial.print(" Temperature Value: ");
  Serial.print(tempeValue);
  Serial.println("\u00B0");
  Serial.println("part 2--------- end");


  // Format the temperature and humidity as strings
  String temperatureMessage = "Temperature: " + String(tempeValue) + "\n";
  String humidityMessage = "Humidity: " + String((float) DHT.humidity) + "\n";

  // Send the messages via SoftwareSerial
  Serial.println("Sending data...");
  //mySerial.write((byte*)&tempeValue, sizeof(tempeValue));
  //mySerial.write((byte*)&DHT.humidity, sizeof(DHT.humidity));

  mySerial.print(tempeValue);
  mySerial.print(DHT.humidity);
  
  delay(2000);
}
