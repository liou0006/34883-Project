#ifndef LCDPrint_H_
#define LCDPrint_H_

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

void lcdPrintWelcome(LiquidCrystal_I2C lcd);
void lcdPrintData(LiquidCrystal_I2C lcd, float T, float H);

#endif
