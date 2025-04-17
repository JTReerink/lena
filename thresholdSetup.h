#ifndef THRESHOLDSETUP_H
#define THRESHOLDSETUP_H

#include <Adafruit_MPR121.h>
#include "Arduino.h"
#include "config.h"

// Let op: cap wordt extern gedeclareerd omdat deze in de .ino wordt gedefinieerd
extern Adafruit_MPR121 cap;

extern uint16_t touchValues[numTouchpads];
extern uint16_t releaseValues[numTouchpads];

void SetupFase(bool& setupFase);
void Blink(int led);

#endif
