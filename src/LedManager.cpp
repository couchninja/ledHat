#include <LedManager.h>

CRGBArray<LedSettings::NUM_LEDS> leds;

#define NUM_MODES 3;
int mode = 0;

LedManager::LedManager(int otaState, AccelManager * accelManager) {
	Serial.println("Initializing LedManager");
	this->accelManager = accelManager;

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 50);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, LedSettings::NUM_LEDS);

	this->horizonAnimation = new HorizonAnimation(accelManager);
	this->fireAnimation = new FireAnimation(accelManager);
	this->movingDotAnimation = new MovingDotAnimation(accelManager);
	this->rainbowAnimation = new RainbowAnimation(accelManager);
	this->accelAnimation = new AccelAnimation(accelManager);
	this->stableDollarAnimation = new StableDollarAnimation(accelManager);
	this->brightnessSettingsAnimation = new BrightnessSettingsAnimation(
			accelManager);

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

	FastLED.show();

	delay(200);
}

void LedManager::step() {
	Animation * activeAnim;

	if (this->settingsMode) {
		activeAnim = brightnessSettingsAnimation;
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

	activeAnim->step();
	memmove(&leds[0], &activeAnim->leds[0], LedSettings::NUM_LEDS * sizeof(CRGB));

	FastLED.show();
}

void LedManager::handleClick() {
	if (this->settingsMode) {
		Serial.println("send click to brightnessSettingsAnimation");
		this->brightnessSettingsAnimation->handleClick();
	} else {
		mode++;
		mode %= NUM_MODES
	}
}

void LedManager::handleLongPress() {
	this->settingsMode = !this->settingsMode;
}

LedManager::~LedManager() {
	delete horizonAnimation;
}
