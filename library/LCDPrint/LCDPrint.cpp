#include "Arduino.h"
#include "LCDPrint.h"
#include <LiquidCrystal_I2C.h>

/**
 * @brief Prints a super cool welcome message on the LCD, and switches back to T/H view.
 */
void lcdPrintWelcome(LiquidCrystal_I2C lcd){
  lcd.print("Welcome home!");
  delay(500);
  for(int i = 0; i<16; i++){
    delay(100);
    lcd.scrollDisplayRight();
  }
  for(int i = 0; i<16; i++){
    delay(100);
    lcd.scrollDisplayLeft();
  }
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print("Humi:");
}

/**
 * @brief Prints the temperature and humidity to the LCD
 *
 * @param T  Temperature variable.
 * @param H  Humidity variable.
 */
void lcdPrintData(LiquidCrystal_I2C lcd, float T, float H){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(0,1);
  lcd.print("Humi:");

  lcd.setCursor(7,0);
  lcd.print(T, 1);
  lcd.write(byte(0));
  lcd.print("C");

  lcd.setCursor(7,1);
  lcd.print(H, 1);
  lcd.print("%");
}
