#include <LedManager.h>

CRGBArray<LedSettings::NUM_LEDS> leds;

LedManager::LedManager(int otaState, AccelManager * accelManager) {
	Serial.println("Initializing LedManager");
	this->accelManager = accelManager;

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 50);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, LedSettings::NUM_LEDS);

	this->brightnessSettingsAnimation = new BrightnessSettingsAnimation(accelManager);
	this->animations = vector<Animation*>();

	this->animations.push_back(new GravAnimation(accelManager));
	this->animations.push_back(new MovingDotAnimation(accelManager));
	this->animations.push_back(new RainbowAnimation(accelManager));
//	this->animations.push_back(new FireAnimation(accelManager));

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
		activeAnim = animations[mode];
	}

	activeAnim->step();
	memmove(&leds[0], &activeAnim->leds[0], LedSettings::NUM_LEDS * sizeof(CRGB));

	FastLED.show();
}

void LedManager::fastStep() {
	Animation * activeAnim;

	if (this->settingsMode) {
		activeAnim = brightnessSettingsAnimation;
	} else {
		activeAnim = animations[mode];
	}

	activeAnim->fastStep();
}

void LedManager::handleClick() {
	if (this->settingsMode) {
		Serial.println("send click to brightnessSettingsAnimation");
		this->brightnessSettingsAnimation->handleClick();
	} else {
		mode++;
		mode %= animations.size();
	}
}

void LedManager::handleLongPress() {
	this->settingsMode = !this->settingsMode;
}

LedManager::~LedManager() {
	for (int i = 0; i < animations.size(); i++) {
		delete animations[i];
	}
}
