#include <animation/StableDollarAnimation.h>

StableDollarAnimation::StableDollarAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

void StableDollarAnimation::step() {
	leds.fadeToBlackBy(150);
//	leds.blur1d(64);

// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[0] + M_PI / 2) / (M_PI);
	int offset = pitchNormalized * ledsPerStrip;

	offset %= ledsPerStrip;

	this->addDollar(offset % ledsPerStrip, 255);
	this->addDollar((offset + 9) % ledsPerStrip, 255);
	this->addDollar((offset + 18) % ledsPerStrip, 255);
	this->addDollar((offset + 27) % ledsPerStrip, 255);

//	for (uint8_t vline = 0; vline < 4; vline++) {
//		for (uint8_t hline = 0; hline < numStrips; hline++) {
//			int lineOffset = offset + ledsPerStrip / 4 * vline;
//			lineOffset %= ledsPerStrip;
//			int sat = (255.0 / 4.0) * (float) (numStrips - hline - 1);
//			leds[hline * ledsPerStrip + lineOffset] = CHSV(hue, sat, 255);
//		}
//	}

// update vars
}

StableDollarAnimation::~StableDollarAnimation() {
}
