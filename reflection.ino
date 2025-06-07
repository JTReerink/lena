#include <Wire.h>
#include <Adafruit_MPR121.h>

#include "config.h"
#include "thresholdSetup.h"
#include "ledstrip.h"

Adafruit_MPR121 cap = Adafruit_MPR121();

bool setupFase = true;
int touchIndex = 0; // Welke touchpad in de setupfase

uint16_t touchValues[numTouchpads];
uint16_t releaseValues[numTouchpads];
uint16_t grabValues[numTouchpads];

TouchState touchStates[numTouchpads];

uint16_t arrayToTD[numTouchpads];

TaskHandle_t LedAnim;

void setup() {
  Serial.begin(115200);

  // Zet de pin van de button als input
  pinMode( BUTTON_PIN, INPUT_PULLUP);

  //Zet elke pin voor de ledjes als output
  for (int i = 0; i < numTouchpads; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  for (int i=0; i < numTouchpads; i++) {
    for(int j=0; j < NUM_LEDS/numTouchpads + 1; j++) {
      if(i + j * numTouchpads >= NUM_LEDS) {
        ledstripParts[i][j] = i + (j - 1) * numTouchpads;
        continue;
      }
      ledstripParts[i][j] = i + j * numTouchpads;
    }
    shuffleArray(ledstripParts[i], NUM_LEDS/numTouchpads);

  }
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  xTaskCreatePinnedToCore(
    LedAnimCode, /* Function to implement the task */
    "Task1", /* Name of the task */
    10000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &LedAnim,  /* Task handle. */
    1   /* Core where the task should run */
  ); 


  // Check of de mpr121 werkt
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 niet gevonden. Check verbinding.");
    while (1);
  }

  Serial.println("Setup start: calibratie per touchpad");
  delay(1000);
}

void printLastTouch() {
  for (int i = 0; i < numTouchpads; i++) {
    Serial.print(arrayToTD[i]);
    if (i < numTouchpads-1) {
      Serial.print(", "); // Komma's tussen de waarden
    }
  }
  Serial.println(); // Nieuwe regel na het printen van de array
}

void loop() {
  if (setupFase) {
    SetupFase();
  } else {
    // Check voor elke touchpad of deze wordt aangeraakt
    for (int i = 0; i < numTouchpads; i++) {
      uint16_t currentValue = cap.filteredData(i);

      if (currentValue < touchValues[i]) {
        touchStates[i] = TOUCHED;
      } else {
        touchStates[i] = RELEASED;
      }
      // if (i < numFruits) { // Alleen voor de vruchten (de eerste 2 touchpads)
      //   switch (touchStates[i]) {
      //     case GRABBED:
      //       // Pas terug naar RELEASED als het duidelijk onder de grab-waarde komt
      //       if (currentValue < grabValues[i] - grabThresholdMargin) {
      //         touchStates[i] = RELEASED;
      //       }
      //       break;

      //     case TOUCHED:
      //     case RELEASED:
      //       // GRABBED wordt alleen geactiveerd als de waarde duidelijk boven de release-waarde ligt
      //       if (currentValue > grabValues[i]) {
      //         touchStates[i] = GRABBED;
      //       } else if (currentValue < touchValues[i]) {
      //         touchStates[i] = TOUCHED;
      //       } else {
      //         touchStates[i] = RELEASED;
      //       }
      //       break;
      //   }
      // } else { // Voor de bladeren, geen grab-fase
        
      // }

      // zet tijdelijke prev state
      int prevState = arrayToTD[i];
      // zet de nieuwe state
      switch (touchStates[i]) {
        case GRABBED: case TOUCHED:
          arrayToTD[i] = 1;
          break;
        case RELEASED:
          arrayToTD[i] = 0;
      }
      // alleen functie doen als er een verandering is
      if(prevState != arrayToTD[i]) {
        printLastTouch();
      }
    }


    // Print de toestand van de touchpads (debugging)
    for (int i = 0; i < numTouchpads; i++) {
      if(touchStates[i]>0) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
      // Serial.print("Vrucht ");
      // Serial.print(i);
      // Serial.print(": ");
      // Serial.printf("→ Threshold ingesteld: touch=%d, release=%d, grab=%d\n",
      //               touchValues[i], releaseValues[i], grabValues[i]);
      // Serial.println(touchStates[i]);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
