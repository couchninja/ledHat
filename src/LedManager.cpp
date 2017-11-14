#include <LedManager.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

CRGBArray<NUM_LEDS> leds;

int mode = 0;

LedManager::LedManager(bool wifiConnected) {
	Serial.println("Initializing LedManager");
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);

	if (wifiConnected)
		leds.fill_solid(CHSV(100, 255, 100));
	else
		leds.fill_solid(CHSV(255, 255, 100));
	FastLED.show();

	delay(200);
}

void LedManager::step(AccelManager * accelManager) {
	if (mode == 0) {
		static uint8_t hue;
		static uint8_t offset;

		leds.fadeToBlackBy(80);
		leds.blur1d(64);

		// from 0 to 1
		float pitchNormalized = (accelManager->ypr[1] + M_PI / 2) / (M_PI);
//		Serial.print("normalized: ");
//		Serial.println(pitchNormalized);

		offset = pitchNormalized * LEDS_PER_STRIP;
//		Serial.print("offset: ");
//		Serial.println(offset);

		offset %= LEDS_PER_STRIP;

		for (uint8_t i = 0; i < NUM_OF_STRIPS; i++) {
			leds[i * LEDS_PER_STRIP + offset] = CHSV(hue, 255, 255);;
		}

		hue++;
	} else if(mode == 1) {
		static uint8_t j = 0;
		j += 10;
		leds.fill_rainbow(j);
	}	else {
		// red
		leds.fill_solid(CHSV(255, 255, 255));
	}

	FastLED.show();

}

void LedManager::nextMode() {
	mode++;
	mode %= 2;
}

LedManager::~LedManager() {
}
