#include "config.h"

int maxLedsPerSensor = NUM_LEDS / numTouchpads;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NeoPIN, NEO_GRB + NEO_KHZ800);
const uint32_t stripColors[] = {
  strip.Color(0, 0, 255),     // blauw
  strip.Color(230, 50, 0),    // oranje
  strip.Color(255, 255, 0),   // geel
  strip.Color(250, 0, 0),     // rood
  strip.Color(0, 255, 0),      // groen
  strip.Color(75, 214, 212)  // licht blauw
};
int ledstripParts[numTouchpads][NUM_LEDS / numTouchpads] = {0};
