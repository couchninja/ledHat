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
	float pitchNormalized = (accelManager->ypr[0] + M_PI / 2) / (M_PI);
	offset = pitchNormalized * ledsPerStrip;

	offset %= ledsPerStrip;

	for (uint8_t vline = 0; vline < 4; vline++) {
		for (uint8_t hline = 0; hline < numStrips; hline++) {
			int lineOffset = offset + ledsPerStrip / 4 * vline;
			lineOffset %= ledsPerStrip;
			int sat = (255.0 / 4.0) * (float) (numStrips - hline - 1);
			leds[hline * ledsPerStrip + lineOffset] = CHSV(hue, sat, 255);
		}
	}

	hue++;
}

MovingDotAnimation::~MovingDotAnimation() {
}
