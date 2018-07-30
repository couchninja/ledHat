#include <animation/GravAnimation.h>
#include <numeric>

GravAnimation::GravAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
	brightness = new byte[LedSettings::LEDS_PER_STRIP];
}

void GravAnimation::fastStep() {
	gravFrangle = toFrangle(accelManager->gravityDelta);

	if (accelManager->rollingGravityDelta.getMagnitude() > 0.001) {
		// -0.5 ... 0,5 starting from back of hat, clockwise
		float attraction = frangleDiff(lastGravFrangle, smoothGravFrangle);

		// add bias to rotate clockwise
		if (attraction < -0.3)
			attraction += 1.0;

		float const maxAttraction = 0.1;

		attraction = clampf(attraction, -maxAttraction, maxAttraction);

		gravFrangleVelocity = lerp(gravFrangleVelocity, attraction, 0.15);
//		gravAngleVelocity = lerp(gravAngleVelocity, attraction,
//						accelManager->rollingGravityDelta.getMagnitude()*10.0);

		float const maxVelocity = 0.1;
		gravFrangleVelocity = clampf(gravFrangleVelocity, -maxVelocity, maxVelocity);
	} else {
		// friction
		gravFrangleVelocity *= 0.9;
	}

	smoothGravFrangle = normaliseFrangle(smoothGravFrangle + gravFrangleVelocity);

	motionNess = lerp(motionNess, accelManager->rollingGravityDelta.getMagnitude(), 0.005);

	// housekeeping
	lastGravFrangle = gravFrangle;
}



void GravAnimation::step() {
	bool motion = motionNess > 0.003;

//	motion = true;

	for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		brightness[i] = qsub8(brightness[i], 40);
	}

	int ledIndex = smoothGravFrangle * LedSettings::LEDS_PER_STRIP;
	ledIndex = (ledIndex + LedSettings::LEDS_PER_STRIP) % LedSettings::LEDS_PER_STRIP;
	if (motion) {
		addBlob(brightness, ledIndex, 15, false);
	}

	float hueSteps = 255.0 / ((float) LedSettings::LEDS_PER_STRIP);

	if (motion) {
		for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
			leds[i] = CHSV(int(i * hueSteps + hue) % 255, 255, brightness[i]);
		}
	} else {
		for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
			leds[i] = CHSV(int(i * hueSteps + hue) % 255, 255, 255);
		}
	}

//	leds[ledIndex] = CRGB(255, 255, 255);

// housekeeping
	hue += 2;
}

GravAnimation::~GravAnimation() {
}

