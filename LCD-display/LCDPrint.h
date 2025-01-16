#ifndef LCDPrint_H_
#define LCDPrint_H_

#include "Arduino.h"
#include <LiquidCrystal.h>

void lcdPrintWelcome(LiquidCrystal lcd);
void lcdPrintData(LiquidCrystal lcd, float T, float H);

#endif
