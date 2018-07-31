#include <animation/MovingDotAnimation2.h>

MovingDotAnimation2::MovingDotAnimation2(AccelManager * accelManager) :
		Animation(accelManager) {
	brightness = new byte[LedSettings::LEDS_PER_STRIP];
}

void MovingDotAnimation2::step() {
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
//	float pitchNormalized = (accelManager->ypr[2] + M_PI / 2) / (M_PI);
//	int offset = pitchNormalized * ledsPerStrip;
	float pitchNormalized = (accelManager->ypr[2] + M_PI / 2) / (M_PI);
	int offset = pitchNormalized * ledsPerStrip * 3;

	int const dots = 3;

//	float deltaGravFrangle = toFrangle(accelManager->gravityDelta);
//
//	int motion = accelManager->rollingGravityDelta.getMagnitude() > 0.01;

	for (uint8_t dot = 0; dot < dots; dot++) {
		int ledIndex = normaliseLedIndex(offset + ledsPerStrip / dots * dot);
		addBlob(brightness, ledIndex, 15, false);
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

MovingDotAnimation2::~MovingDotAnimation2() {
}
