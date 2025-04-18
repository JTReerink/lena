#include "thresholdSetup.h"
#include "config.h"

void SetupFase() {
  static bool lastButtonState = HIGH;
  static bool waitingForRelease = true;
  static bool waitingForTouch = false;
  static bool waitingForGrab = false;
  static bool ledInitialized = false;


  bool currentButtonState = digitalRead(BUTTON_PIN);
  bool justPressed = lastButtonState == HIGH && currentButtonState == LOW;
  lastButtonState = currentButtonState;

  digitalWrite(ledPins[touchIndex], HIGH);

  if (justPressed && touchIndex < numTouchpads) {
    uint16_t val = cap.filteredData(touchIndex);

    if (waitingForRelease) {
      // zet de release value
      releaseValues[touchIndex] = val;
      Serial.printf("Vrucht %d release-waarde: %d\n", touchIndex, val);
      Serial.println("→ Raak de vrucht nu aan en druk opnieuw op de knop");
      waitingForRelease = false;
      waitingForTouch = true;
      Blink(touchIndex);
    }
    else if (waitingForTouch) {
      // zet de touch value
      touchValues[touchIndex] = val + 20; // +5 om zeker te weten dat je onder deze waarde komt (bij gebruik van button op esp +20, de waarde zakt door aanraking van electronica)
      Serial.printf("Vrucht %d touch-waarde: %d\n", touchIndex, val);

      if (touchIndex < numFruits) {
        // als we nog brij vruchten zijn:
        Serial.println("→ Pak de vrucht nu op en druk opnieuw op de knop");
        waitingForTouch = false;
        waitingForGrab = true;
        Blink(touchIndex);
      } else {
        // als het om een blad gaat:
        Serial.println("→ Threshold ingesteld voor blad");
        digitalWrite(ledPins[touchIndex], LOW);
        touchIndex++;
        waitingForRelease = true;
        waitingForTouch = false;
      }
    }
    else if (waitingForGrab) {
      // zet de grab value
      grabValues[touchIndex] = val - 2; // -2 om zeker te weten dat je boven de waarde uit komt
      Serial.printf("Vrucht %d grab-waarde: %d\n", touchIndex, val);
      Serial.printf("→ Threshold ingesteld: touch=%d, release=%d, grab=%d\n",
                    touchValues[touchIndex], releaseValues[touchIndex], grabValues[touchIndex]);

      digitalWrite(ledPins[touchIndex], LOW);
      touchIndex++;
      waitingForRelease = true;
      waitingForGrab = false;
    }

    if (touchIndex >= numTouchpads) {
      Serial.println("✅ Kalibratie voltooid!");
      setupFase = false;
    } else if (waitingForRelease) {
      Serial.println("→ Druk op de knop met de volgende vrucht/bladeren onaangeraakt.");
    }

    delay(200); // debounce
  }
}

void Blink(int ledIndex) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPins[ledIndex], HIGH);
    delay(200);
    digitalWrite(ledPins[ledIndex], LOW);
    delay(200);
  }
}
