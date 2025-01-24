#ifndef LCDPrint_H_
#define LCDPrint_H_

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

void initLcd(LiquidCrystal_I2C lcd);
void writeToLCD(LiquidCrystal_I2C lcd, int row, int colm, char string[]);
void lcdPrintWelcome(LiquidCrystal_I2C lcd);
void lcdPrintData(LiquidCrystal_I2C lcd, float T, float H);

#endif
