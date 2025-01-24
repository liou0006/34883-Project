/**
*@file LCDPrint.cpp
*
*
*@section description Description
*A library to contain simple hardcoded messages for the LCD screen, as to not clog up the main file as much, and to be used between the Arduino boards.
*
*@section author Authors
*Created by Liu Xia, Oscar Sjelle & Mads Andersen
*
*
*@date 24/1/2025
*
**/

#include "Arduino.h"
#include "LCDPrint.h"
#include <LiquidCrystal_I2C.h>

/**
 * @brief initializes the lcd.
 */
void initLcd(LiquidCrystal_I2C lcd) {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
}

/**
 * @brief clears and writes a string to the LCD at a specified row and column.
 */
void writeToLCD(LiquidCrystal_I2C lcd, int row, int colm, char string[]) {
  lcd.clear();
  lcd.setCursor(colm, row);
  lcd.print(string);
}

/**
 * @brief Prints a super cool welcome message on the LCD.
 */
void lcdPrintWelcome(LiquidCrystal_I2C lcd){
  lcd.clear();
  lcd.setCursor(0, 0);
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
