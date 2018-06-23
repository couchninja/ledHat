#include <LedManager.h>

CRGBArray<LedSettings::NUM_LEDS> leds;

#define NUM_MODES 3;
int mode = 0;

Animation * horizonAnimation = new HorizonAnimation();
Animation * fireAnimation = new FireAnimation();
Animation * movingDotAnimation = new MovingDotAnimation();
Animation * rainbowAnimation = new RainbowAnimation();
Animation * accelAnimation = new AccelAnimation();
Animation * stableDollarAnimation = new StableDollarAnimation();

LedManager::LedManager(bool wifiConnected) {
	Serial.println("Initializing LedManager");

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, LedSettings::NUM_LEDS);

	if (wifiConnected)
		leds.fill_solid(CHSV(100, 255, 100));
	else
		leds.fill_solid(CHSV(255, 255, 100));
	FastLED.show();

	delay(200);
}

void LedManager::step(AccelManager * accelManager) {
	Animation * activeAnim;
	switch (mode) {
	case 0:
		activeAnim = movingDotAnimation;
		break;
	case 1:
		activeAnim = rainbowAnimation;
		break;
	case 2:
		activeAnim = accelAnimation;
		break;
	}

	activeAnim->step(accelManager);
	memmove(&leds[0], &activeAnim->leds[0], LedSettings::NUM_LEDS * sizeof(CRGB));

	FastLED.show();
}

void LedManager::nextMode() {
	mode++;
	mode %= NUM_MODES
}

LedManager::~LedManager() {
	delete horizonAnimation;
}
