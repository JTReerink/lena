#include "config.h"
#include "ledstrip.h"


// Dit bepaalt de snelheid van de golfbeweging van de wave animatie
float waveSpeed = 0.1; // Hoe sneller de golf, hoe hoger het getal
float phase = 0; // Faseverschuiving voor de golf

// Initieer ledtrip
int number[numTouchpads];  // onthoud welke LED laatste was in reeks van ledstripparts

void shuffleArray(int* array, int size) {
  for (int i = size - 1; i > 0; i--) {
    int j = random(0, i + 1); // Kies een willekeurige index
    // Verwissel array[i] en array[j]
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

void LedAnimCode( void * pvParameters) {

  for(;;) {
    for (int i = 0; i < numTouchpads; i++) {
      int currentNumber = number[i];

      if (touchStates[i] == RELEASED) {
        // Zet alle LEDs van dit segment uit als de sensor niet wordt aangeraakt
        for (int x = 0; x < maxLedsPerSensor; x++) {
          strip.setPixelColor(ledstripParts[i][x], strip.Color(0, 0, 0));
        }
      } else {
        // Zet de vorige LED uit, ook als currentNumber op 0 wordt gezet
        int previousNumber = currentNumber - 1;
        if (currentNumber == 0) {
          previousNumber = maxLedsPerSensor - 1; // als we op LED 0 zijn, zet de laatste LED uit
        }

        // Zet de vorige LED uit
        strip.setPixelColor(ledstripParts[i][previousNumber], strip.Color(0, 0, 0));

        // Zet de huidige LED aan
        Serial.printf("DEBUG: stripColors[%d] = 0x%08X\n", i, stripColors[i]);
        strip.setPixelColor(ledstripParts[i][currentNumber], stripColors[i]);

        // Verhoog currentNumber en reset naar 0 als het einde is bereikt
        currentNumber++;
        if (currentNumber >= maxLedsPerSensor) {
          currentNumber = 0;
        }

        // Sla de nieuwe waarde op
        number[i] = currentNumber;
      }

      // Update de strip
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    strip.show(); 
  }
}