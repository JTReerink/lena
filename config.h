#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>
#include <Adafruit_MPR121.h>

const int numTouchpads = 6;                 // totaal aantal touchpads
const int numFruits = 2;                    // aantal touchpads met fruitstukken
const int ledPins[numTouchpads] = {         // alle LED pins
  12, 14, 25, 26, 27, 33
};
#define BUTTON_PIN 13                        // button gebruikt tijdens setup

// Globale variabelen
extern Adafruit_MPR121 cap;
extern int touchIndex;
extern bool setupFase;
const int grabThresholdMargin = 5;         // Maat van marge om flikkeren te voorkomen


extern uint16_t touchValues[numTouchpads];
extern uint16_t releaseValues[numTouchpads];
extern uint16_t grabValues[numTouchpads];

enum TouchState {
  RELEASED,
  TOUCHED,
  GRABBED
};

extern TouchState touchStates[numTouchpads];


#endif
