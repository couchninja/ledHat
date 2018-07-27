#include <animation/RainbowAnimation.h>

RainbowAnimation::RainbowAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

void RainbowAnimation::step() {
	static uint8_t j = 0;

	for (int i = 0; i < numLeds; i++) {
		float hueSteps = 255.0 / ((float) LedSettings::LEDS_PER_STRIP);

		leds[i] = CHSV(int(i * hueSteps + j) % 256, 255, 255);
	}

	j += 4;
}

RainbowAnimation::~RainbowAnimation() {
}

