#include <LedManager.h>

#define NUM_LEDS 5*36
CRGBArray<NUM_LEDS> leds;

LedManager::LedManager() {
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);
}

void LedManager::step(AccelManager * accelManager) {
	static uint8_t hue;
	static uint8_t i;

	leds.fadeToBlackBy(80);    // fade everything out
	float pitch = accelManager->ypr[1];
//	float pitch = 2;
	i = (pitch * 30);
	i %= NUM_LEDS;

	leds[i] = CHSV(hue++, 255, 255);
	FastLED.show();
}

LedManager::~LedManager() {
}
