#include <animation/RainbowAnimation.h>

RainbowAnimation::RainbowAnimation() :
		Animation() {
}

void RainbowAnimation::step(AccelManager * accelManager) {
	float intensity = 1.0;

	static uint8_t j = 0;
	j += 4;

	float invIntensity = 1.0 - intensity;

	CHSV hsv;
	hsv.hue = j;
	hsv.val = 255;
	hsv.sat = 240;
	for (int i = 0; i < numLeds; i++) {
		CRGB old = leds[i];
		leds[i] = hsv;
		leds[i].r = (float) leds[i].r * intensity + (float) old.r * invIntensity;
		leds[i].g = (float) leds[i].g * intensity + (float) old.g * invIntensity;
		leds[i].b = (float) leds[i].b * intensity + (float) old.b * invIntensity;

		hsv.hue += 2;
	}
}

RainbowAnimation::~RainbowAnimation() {
}

