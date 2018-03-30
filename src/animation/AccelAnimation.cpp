#include <animation/AccelAnimation.h>

AccelAnimation::AccelAnimation() {
}

float lastAAX = 0;
float lastAAY = 0;
float lastAAZ = 0;

float rollingDiff = 0;
float rollingMaxDiff = 0;

void AccelAnimation::step(AccelManager * accelManager) {
	leds.fadeToBlackBy(220);
//	leds.blur1d(64);

// from -1 (backside down) to +1 (frontside down)
//	float pitchNormalized = -(accelManager->ypr[2]) / (M_PI * 0.5);

//	float intensity = 0;

//	if (lastAAX < 0 && accelManager->aaWorld.x > 0) {
//		intensity = 255;
//		CHSV c = CHSV(255, 255, intensity);
//		leds.fill_solid(c);
//	} else if (lastAAX > 0 && accelManager->aaWorld.x < 0) {
//		intensity = 255;
//		CHSV c = CHSV(255, 255, intensity);
//		leds.fill_solid(c);
//	}
//
//	if (lastAAY < 0 && accelManager->aaWorld.y > 0) {
//		intensity = 255;
//		CHSV c = CHSV(100, 255, intensity);
//		leds.fill_solid(c);
//	} else if (lastAAY > 0 && accelManager->aaWorld.y < 0) {
//		intensity = 255;
//		CHSV c = CHSV(100, 255, intensity);
//		leds.fill_solid(c);
//	}

//	if (abs(accelManager->aaWorld.x) > 4000) {
//		CHSV c = CHSV(255, 255, 255);
//		leds.fill_solid(c);
//	} else if (abs(accelManager->aaWorld.y) > 4000) {
//		CHSV c = CHSV(50, 255, 255);
//		leds.fill_solid(c);
//	} else {
//		CHSV c = CHSV(50, 255, 0);
//		leds.fill_solid(c);
//	}

//	CHSV c = CHSV(255, 255, abs(accelManager->aaWorld.y) / 100);
//	leds.fill_solid(c);

	float diff = abs(lastAAX - accelManager->aaReal.x)
			+ abs(lastAAY - accelManager->aaReal.y)
			+ abs(lastAAZ - accelManager->aaReal.z);
	rollingDiff += diff;
	rollingMaxDiff *= 0.99;
	rollingMaxDiff = _max(rollingDiff, rollingMaxDiff);
	rollingMaxDiff = _max(1000.0, rollingMaxDiff);

	// scaled to approx 0...1
	float intensity = rollingDiff / rollingMaxDiff * 255.0 * 4.0;

	Serial.println(rollingMaxDiff);

	for (int i = numStrips-1; i >= 0; i--) {
		CHSV c = CHSV(255, 255, intensity);
		intensity -= 255;
		intensity = _max(0, intensity);
		for (int r = 0; r < ledsPerStrip; r++) {
			leds[i * ledsPerStrip + r] = c;
		}
	}

//	CHSV c = CHSV(255, 255, intensity);
//	leds.fill_solid(c);

//	float intensity = _min(1, fabs(pitchNormalized));

//	float intensity = abs(accelManager->aaReal.x) / 10 * 255;

// ARON more useful to work from direction changes i guess.

//	Serial.println(255*intensity);

//	Serial.println("intensity");
//	Serial.println(255*intensity);

	lastAAX = accelManager->aaReal.x;
	lastAAY = accelManager->aaReal.y;
	lastAAZ = accelManager->aaReal.z;
	rollingDiff *= 0.2;
}

AccelAnimation::~AccelAnimation() {
}

