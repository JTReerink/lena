#include <Wire.h>
#include <Adafruit_MPR121.h>

#include "config.h"
#include "thresholdSetup.h"

Adafruit_MPR121 cap = Adafruit_MPR121();

bool setupFase = true;

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  for (int i = 0; i < numTouchpads; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 niet gevonden. Check verbinding.");
    while (1);
  }

  Serial.println("Setup start: calibratie per vrucht");
  delay(1000);
}

void loop() {
  if (setupFase) {
    SetupFase(setupFase);
  } else {
    Serial.println("YEAH DE REST");
    delay(1000); // tijdelijk
  }
}
