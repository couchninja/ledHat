#include <LedManager.h>

#define NUM_LEDS 5*36
CRGBArray<NUM_LEDS> leds;

int mode = 0;

LedManager::LedManager() {
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);

	leds.fill_solid(CHSV(255, 255, 30));
}

void LedManager::step(AccelManager * accelManager) {
	if (mode == 0) {
		static uint8_t hue;
		static uint8_t i;

		leds.fadeToBlackBy(80);
		leds.blur1d(64);

		float pitch = accelManager->ypr[1];
		i = pitch * 30;
		i %= NUM_LEDS;
		leds[i] = CHSV(hue++, 255, 255);

//	float acc = accelManager->aaReal.z;
//	i = acc / 1000;

//		static uint8_t j;
//		j += 10;
//		leds.fill_rainbow(j);
	} else {
//		leds.blur1d(64);
		leds.fill_solid(CHSV(255, 255, 255));
	}

	FastLED.show();

}

void LedManager::nextMode() {
	mode++;
	mode%=2;
}

LedManager::~LedManager() {
}
