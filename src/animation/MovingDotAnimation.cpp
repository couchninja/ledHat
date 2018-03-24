#include <animation/MovingDotAnimation.h>

uint8_t hue;
uint8_t offset;

MovingDotAnimation::MovingDotAnimation() :
		Animation() {
}

void MovingDotAnimation::step(AccelManager * accelManager) {
	leds.fadeToBlackBy(80);
	leds.blur1d(64);

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[1] + M_PI / 2) / (M_PI);
	offset = pitchNormalized * ledsPerStrip;

	offset %= ledsPerStrip;

	for (uint8_t i = 0; i < numStrips; i++) {
		leds[i * ledsPerStrip + offset] = CHSV(hue, 255, 255);
	}

	hue++;
}

MovingDotAnimation::~MovingDotAnimation() {
}

