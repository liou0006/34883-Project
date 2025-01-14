/*
 * formalia. can't remember the doxygen syntax
 */

#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

/* Define the pins that are connected to the LCD-display
uint8_t rs = ;
uint8_t enable = ;
uint8_t D4 = ;
uint8_t D5 = ;
uint8_t D6 = ;
uint8_t D7 = ;
LiquidCrystal lcd(rs, enable, D4, D5, D6, D7);
*/

/* Define other used pins
uint8_t flagDisplay = ;
other stuff
*/

/* WiFi and ThingSpeak variables
const char* ssid = "Name";
const char* pass = "Password";
WiFiClient client;

unsigned long channelID = [];
const char *APIReadKey = "";
const char *server = "api.thingspeak.com";
*/

/* Define variables for the program
variable!! yuppiii
*/

void setup() {
  lcd.begin(16,2);
  lcdSetup();

  WiFi.begin(ssid, pass);
  Serial.begin(9600);
}

void loop() {
  if(!digitalRead(flagDisplay)){
    /* Read latest data from ThingSpeak
    ThingSpeak.begin(client);
    client.connect(server, 80); //connect(URL, Port)
    ThingSpeak.readIntField(channelID, Field, APIReadKey);
    client.stop();
    */
    
    // Send data to LCD/other Arduino to be displayed
  }
  
  // Perhaps do some other stuff
}

/// Clears the LCD-display, and writes *stuff*
void lcdSetup(){
  lcd.clear();
  lcd.setCursor(0,0);
  // do stuff
}
