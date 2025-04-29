#ifndef CONFIG_H
#define CONFIG_H


#include <Arduino.h>
#include <Adafruit_NeoPixel.h> //Ledstrip
#include <Adafruit_MPR121.h>   //Cap-sensor

const int numTouchpads = 6;                 // totaal aantal touchpads
const int numFruits = 2;                    // aantal touchpads met fruitstukken
const int ledPins[6] = {         // alle LED pins
  12, 14, 25, 26, 27, 33
};
#define BUTTON_PIN 13                        // button gebruikt tijdens setup

#define NeoPIN 4 // de pin waarop het signaal naar de strip gaat
constexpr int NUM_LEDS = 119; // aantal ledjes op de strip
extern int maxLedsPerSensor;
constexpr int stripBrightness = 255;
extern Adafruit_NeoPixel strip;
extern const uint32_t stripColors[];
extern int ledstripParts[numTouchpads][NUM_LEDS/numTouchpads];

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
