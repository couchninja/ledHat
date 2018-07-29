#include <animation/GravAnimation.h>
#include <numeric>

GravAnimation::GravAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
	heat = new byte[LedSettings::LEDS_PER_STRIP];
}

float lerp(float one, float two, float fractionOfTwo) {
	return one * (1 - fractionOfTwo) + two * fractionOfTwo;
}

// return modulus on the positive side
float posimodo(float a, float n) {
	// this broke the heat array after a while when implemented like this for int... is this ok?
	return a - floor(a / n) * n;
}

// return modulus on the positive side
int posimodoi(int a, int n) {
	return (a % n + n) % n;
}

float normalise(float num) {
	return posimodo(num, 1);
}

// returns between -0.5 an +0.5
float normAngleDiff(float targetA, float sourceA) {
	float a = targetA - sourceA;
	return posimodo((a + 1), 1) - 0.5;
}

int clampi(int num, int lower, int upper) {
	return min(upper, max(lower, num));
}

float clampf(float num, float lower, float upper) {
	return min(upper, max(lower, num));
}

float normAngleLerp(float one, float two, float fractionOfTwo) {
	one = normalise(one);
	two = normalise(two);

	float diff = two - one;

	if (diff > 0.5) {
		diff = two - (one + 1);
	}

	float diff2 = normAngleDiff(one, two);

	return normalise(one + fractionOfTwo * diff);

	// ARON
//	return normalise(one * (1 - fractionOfTwo) + two * fractionOfTwo);
}

void GravAnimation::fastStep() {
	Vector3D gravHat = grav2hat(accelManager->gravityDelta);
	gravHat.z = 0;
	gravHat.normalize(); // probably not needed anymore

	// in rad, hat coords, counter clockwise
	gravAngle = atan2(gravHat.y, gravHat.x);

	// in [0...1], hat coords, counter clockwise
	gravAngle = gravAngle / 2 / PI;

	// in rad, hat coords, clockwise (because LEDs are mounted clockwise)
	gravAngle = 1 - gravAngle;

	// in [0...1], starting from back of hat, clockwise
	gravAngle = normalise(gravAngle + 0.25);

	if (accelManager->rollingGravityDelta.getMagnitude() > 0.001) {
		// -0.5 ... 0,5
		float attraction = normAngleDiff(lastGravAngle, smoothGravAngle);

		if (attraction < -0.2)
			attraction += 1.0;

		float const maxAttraction = 0.1;

		attraction = clampf(attraction, -maxAttraction, maxAttraction);

		gravAngleVelocity = lerp(gravAngleVelocity, attraction, 0.15);
//		gravAngleVelocity = lerp(gravAngleVelocity, attraction,
//						accelManager->rollingGravityDelta.getMagnitude()*10.0);

		float const maxVelocity = 0.1;
		gravAngleVelocity = clampf(gravAngleVelocity, -maxVelocity, maxVelocity);
	} else {
		// friction
		gravAngleVelocity *= 0.9;
	}

	smoothGravAngle = normalise(smoothGravAngle + gravAngleVelocity);

	motionNess = lerp(motionNess, accelManager->rollingGravityDelta.getMagnitude(), 0.005);

	// housekeeping
	lastGravAngle = gravAngle;
}

void GravAnimation::step() {
	bool motion = false;

	if (motionNess > 0.003) {
		motion = true;
	} else {
		motion = false;
	}
//	motion = true;

	for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		heat[i] = qsub8(heat[i], 40);
	}

	int ledIndex = smoothGravAngle * LedSettings::LEDS_PER_STRIP;
	ledIndex = (ledIndex + LedSettings::LEDS_PER_STRIP) % LedSettings::LEDS_PER_STRIP;
	if (motion) {
		heat[ledIndex] = 255;

		int const spread = 15;

		for (int i = 0; i < spread; i++) {
			int brightness = 255 - 255 / spread;
			heat[posimodoi(ledIndex - i, LedSettings::LEDS_PER_STRIP)] =
					min(heat[ledIndex - 1] + brightness, 255);
			heat[posimodoi(ledIndex + i, LedSettings::LEDS_PER_STRIP)] =
					min(heat[ledIndex + 1] + brightness, 255);
		}
	}

	float hueSteps = 255.0 / ((float) LedSettings::LEDS_PER_STRIP);

	if (motion) {
		for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
			leds[i] = CHSV(int(i * hueSteps + hue) % 255, 255, heat[i]);
		}
	} else {
		for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
			leds[i] = CHSV(int(i * hueSteps + hue) % 255, 255, 255);
		}
	}

//	leds[ledIndex] = CRGB(255, 255, 255);

	// housekeeping
	hue += 1;
}

void GravAnimation::oldStep() {
	leds.fadeToBlackBy(80);

	leds.fill_solid(CRGB(0, 0, 0));

	bool motion = false;

	if (accelManager->rollingGravityDelta.getMagnitude() > 0.005) {
		motion = true;
	}

	//	Serial.println(String("angle: ") + (gravAngle / 2 / PI));

	int ledIndex = gravAngle * LedSettings::LEDS_PER_STRIP;
	ledIndex = (ledIndex + LedSettings::LEDS_PER_STRIP) % LedSettings::LEDS_PER_STRIP;

	if (motion)
		leds[ledIndex] = CRGB(255, 255, 255);
	else
		leds[ledIndex] = CRGB(255, 0, 0);

	// housekeeping
	hue += 1;
}

GravAnimation::~GravAnimation() {
}

