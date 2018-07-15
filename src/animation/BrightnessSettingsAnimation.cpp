#include <animation/BrightnessSettingsAnimation.h>

BrightnessSettingsAnimation::BrightnessSettingsAnimation(
		AccelManager * accelManager) :
		Animation(accelManager) {
}

void BrightnessSettingsAnimation::step() {
	leds.fill_solid(CRGB(0, 0, 0));

	for (int i = 0; i <= this->brightnessIndex; i++) {
		leds[i] = CRGB(255, 255, 255);
	}
}

void BrightnessSettingsAnimation::handleClick() {
	Serial.println(F("this->brightnessIndex:"));
	Serial.println(this->brightnessIndex);

	brightnessIndex++;
	// ARON use vector?
	brightnessIndex = this->brightnessIndex % 5;

//	FastLED.setMaxPowerInVoltsAndMilliamps(5, brightnesses[brightnessIndex]);
}

BrightnessSettingsAnimation::~BrightnessSettingsAnimation() {
}
