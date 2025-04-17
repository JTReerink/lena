#include "thresholdSetup.h"

int touchIndex = 0;
bool waitingForRelease = true;

uint16_t touchValues[numTouchpads];
uint16_t releaseValues[numTouchpads];

void SetupFase(bool& setupFase) {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  digitalWrite(ledPins[touchIndex], HIGH);

  bool justPressed = lastButtonState == HIGH && currentButtonState == LOW;
  lastButtonState = currentButtonState;
  
  if (justPressed && touchIndex < numTouchpads) {
    uint16_t val = cap.filteredData(touchIndex);

    if (waitingForRelease) {
      releaseValues[touchIndex] = val;
      Serial.print("touchpad ");
      Serial.print(touchIndex);
      Serial.print(" release-waarde opgeslagen: ");
      Serial.println(val);
      Serial.println("→ Raak de touchpad nu aan en druk opnieuw op de knop");
      waitingForRelease = false;
      Blink(touchIndex);
    } else {
      digitalWrite(ledPins[touchIndex], LOW);
      touchValues[touchIndex] = val;
      Serial.print("touchpad ");
      Serial.print(touchIndex);
      Serial.print(" touch-waarde opgeslagen: ");
      Serial.println(val);

      cap.writeRegister(MPR121_TOUCHTH_0 + touchIndex, touchValues[touchIndex] + 3);
      cap.writeRegister(MPR121_RELEASETH_0 + touchIndex, releaseValues[touchIndex] - 3);

      Serial.print("→ Threshold ingesteld voor pin ");
      Serial.print(touchIndex);
      Serial.print(": touch=");
      Serial.print(touchValues[touchIndex]);
      Serial.print(", release=");
      Serial.println(releaseValues[touchIndex]);

      digitalWrite(ledPins[touchIndex], LOW);

      touchIndex++;
      waitingForRelease = true;

      if (touchIndex < numTouchpads) {
        Serial.println("→ Druk op de knop met de volgende touchpad onaangeraakt.");
      } else {
        Serial.println("✅ Kalibratie voltooid!");
        setupFase = false;
      }
    }

    delay(200); // debounce
  }
}

void Blink(int led) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[led], HIGH);
    delay(300);
    digitalWrite(ledPins[led], LOW);
    delay(300);
  }
}
