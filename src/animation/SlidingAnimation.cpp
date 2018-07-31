#include <animation/SlidingAnimation.h>

SlidingAnimation::SlidingAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
	brightness = new byte[LedSettings::LEDS_PER_STRIP];
}

void SlidingAnimation::step() {
	leds.fadeToBlackBy(80);
	leds.blur1d(200);

	// scaled to approx 0...255
	float intensity = accelManager->rollingAARealDiff / accelManager->rollingMaxAARealDiff * 255;

	if (intensity < 230) {
		intensity = 0;
	}

	intensity = _max(50, intensity);

	for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		brightness[i] = qsub8(brightness[i], 40);
	}

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[1] + M_PI * 0.5) / M_PI;

	// ARON try to make dots get closer when closer to front/back
	int offset = pitchNormalized * ledsPerStrip;

	int const dist = 15;

	for (int i = 0; i < LedSettings::LEDS_PER_STRIP / 2; i++) {
		if (posimodoi(i, 15) == posimodoi(offset, 15)) {
			addBlob(brightness, i, 5, false);
		}
	}

	int oppositeOffset = LedSettings::LEDS_PER_STRIP - offset;
	for (int i = LedSettings::LEDS_PER_STRIP / 2 + 1; i < LedSettings::LEDS_PER_STRIP; i++) {
		if (posimodoi(i, 15) == posimodoi(oppositeOffset, 15)) {
			addBlob(brightness, i, 5, false);
		}
	}

	float hueSteps = 255.0 / ((float) LedSettings::LEDS_PER_STRIP);

	for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		leds[i] = CHSV(
				int(i * hueSteps + hue) % 255,
				255,
				brightness[i]);
	}

	// update vars
	hue += 2;
}

SlidingAnimation::~SlidingAnimation() {
}
