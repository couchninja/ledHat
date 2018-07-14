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

LedManager::LedManager(int otaState) {
	Serial.println("Initializing LedManager");

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, LedSettings::NUM_LEDS);

	switch (otaState) {
	case OTA_DISABLED:
		leds.fill_solid(CRGB(0, 0, 255));
		break;
	case OTA_CONNECTED:
		leds.fill_solid(CHSV(100, 255, 100));
		break;
	case OTA_DISCONNECTED:
		leds.fill_solid(CHSV(255, 255, 100));
		break;
	}
//	if (wifiConnected)
//		leds.fill_solid(CHSV(100, 255, 100));
//	else
//		leds.fill_solid(CHSV(255, 255, 100));
	FastLED.show();

	delay(200);
}

void LedManager::step(AccelManager * accelManager) {
	Animation * activeAnim;

	if (this->settingsMode) {
		activeAnim = stableDollarAnimation;
	} else {
		switch (mode) {
		case 0:
			//		activeAnim = accelAnimation;
			//		activeAnim = stableDollarAnimation;
			activeAnim = movingDotAnimation;
			break;
		case 1:
			activeAnim = rainbowAnimation;
			break;
		case 2:
			activeAnim = fireAnimation;
			break;
		}
	}

	activeAnim->step(accelManager);
	memmove(&leds[0], &activeAnim->leds[0],
			LedSettings::NUM_LEDS * sizeof(CRGB));

	FastLED.show();
}

void LedManager::nextMode() {
	mode++;
	mode %= NUM_MODES
}

void LedManager::enableSettingsMode() {
	this->settingsMode = true;
}

LedManager::~LedManager() {
	delete horizonAnimation;
}
