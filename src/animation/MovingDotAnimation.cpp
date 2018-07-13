#include <animation/MovingDotAnimation.h>



uint8_t hue;
uint8_t offset;

int idleCounter2 = 0;

MovingDotAnimation::MovingDotAnimation() :
		Animation() {
}

void MovingDotAnimation::step(AccelManager * accelManager) {
	leds.fadeToBlackBy(80);
	leds.blur1d(200);

	// scaled to approx 0...255
	float intensity = accelManager->rollingDiff / accelManager->rollingMaxDiff * 255; // * (numStrips - 1);

	if (intensity < 230) {
			intensity = 0;
			idleCounter2++;
		} else {
			idleCounter2 = 0;
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
//			int sat = (255.0 / 4.0) * (float) (numStrips - hline - 1);
			int sat = 150;
			leds[hline * ledsPerStrip + lineOffset] = CHSV(hue + intensity, sat, 255);
		}
	}

	// update vars
	hue++;
}

MovingDotAnimation::~MovingDotAnimation() {
}
