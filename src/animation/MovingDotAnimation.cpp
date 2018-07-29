#include <animation/MovingDotAnimation.h>

MovingDotAnimation::MovingDotAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

void MovingDotAnimation::step() {
	leds.fadeToBlackBy(80);
	leds.blur1d(200);

	// scaled to approx 0...255
	float intensity = accelManager->rollingAARealDiff / accelManager->rollingMaxAARealDiff * 255;

	if (intensity < 230) {
		intensity = 0;
	}

	intensity = _max(50, intensity);

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[2] + M_PI / 2) / (M_PI);
	offset = pitchNormalized * ledsPerStrip;

	offset %= ledsPerStrip;

	int dots = 6;

	for (uint8_t vline = 0; vline < dots; vline++) {
		for (uint8_t hline = 0; hline < numStrips; hline++) {
			int lineOffset = offset + ledsPerStrip / dots * vline;
			lineOffset %= ledsPerStrip;
			int sat = 150;
			leds[hline * ledsPerStrip + lineOffset] = CHSV(hue + intensity, sat, 255);
		}
	}

	// update vars
	hue++;
}

MovingDotAnimation::~MovingDotAnimation() {
}
