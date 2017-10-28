#include <LedManager.h>
#define NUM_LEDS 5*36
CRGBArray<NUM_LEDS> leds;

LedManager::LedManager() {
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);
}

void LedManager::step() {
	static uint8_t hue;
	static uint8_t i;

	leds.fadeToBlackBy(80);    // fade everything out
	leds[i] = CHSV(hue++, 255, 255);
	i++;
	if (i >= NUM_LEDS) {
		i = 0;
	}
	// now, let's first 20 leds to the top 20 leds,
	//leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
	FastLED.delay(50);
	FastLED.show();
}

LedManager::~LedManager() {
}
