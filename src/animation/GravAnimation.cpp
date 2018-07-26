#include <animation/GravAnimation.h>
#include <numeric>

GravAnimation::GravAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

float lerp(float one, float two, float fractionOfTwo) {
	return one * (1 - fractionOfTwo) + two * fractionOfTwo;
}

// return modulus on the positive side
float posimodo(float a, float n) {
	return a - floor(a / n) * n;
}

float normalise(float num) {
	return posimodo(num, 1);
}

float normAngleDiff(float targetA, float sourceA) {
	float a = targetA - sourceA;
	return posimodo((a + 1), 1) - 0.5;
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

void GravAnimation::step() {
	leds.fadeToBlackBy(80);

////	leds.fill_solid(CRGB(0, 0, 0));
//
//	Vector3D gravSensorDelta = Vector3D(accelManager->rollingGravityDelta);
//	Vector3D gravHatDelta = grav2hat(gravSensorDelta);
//	gravHatDelta.normalize();
//
//	for (float i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
//		Vector3D ledCoord = getRadialCoord(i);
//		float gravSim = max(0.f, gravHatDelta.dot(ledCoord) - 0.5f);
//
////		CHSV r = CHSV(0, 255, gravSim * 256);
////		CHSV g = CHSV(85, 255, gravSim * 256);
////		CHSV b = CHSV(171, 255, gravSim * 256);
//
//		if (accelManager->rollingGravityDelta.getMagnitude() > 0.001)
//			leds[i] += CHSV(int((hue+i)*3)%256, 255, gravSim * 256);
//
////		leds[i] += CRGB(gravSim * 100, gravSim * 100, gravSim * 100);
//
////		Serial.println(
////				String("x ") + ledCoord.x + " y " + ledCoord.y + " z " + ledCoord.z);
//	}

	leds.fill_solid(CRGB(0, 0, 0));

	Vector3D gravHat = grav2hat(accelManager->rollingGravityDelta);
	gravHat.z = 0;
	gravHat.normalize(); // probably not needed anymore

	// in rad, hat coords, counter clockwise
	float gravAngle = atan2(gravHat.y, gravHat.x);

	// in [0...1], hat coords, counter clockwise
	gravAngle = gravAngle / 2 / PI;

	// in rad, hat coords, clockwise (because LEDs are mounted clockwise)
	gravAngle = 1 - gravAngle;

	// in [0...1], starting from back of hat, clockwise
	gravAngle = normalise(gravAngle + 0.25);

	bool motion = false;

	if (accelManager->rollingGravityDelta.getMagnitude() > 0.005) {
		motion = true;

//		float dampenedGravAngle = normAngleLerp(lastGravAngle, gravAngle, 0.3);
//		gravAngle = normAngleLerp(lastGravAngle, gravAngle, 0.8);

		float nowAngleDiff = normAngleDiff(lastGravAngle, gravAngle);
		gravAngleVelocity = lerp(gravAngleVelocity, nowAngleDiff, 0.8);
		gravAngleVelocity = max(-0.1f, min(0.1f, gravAngleVelocity));

		gravAngle = normalise(lastGravAngle + gravAngleVelocity);

//		int ledIndex1 = gravAngle * LedSettings::LEDS_PER_STRIP;
//		ledIndex1 = (ledIndex1 + LedSettings::LEDS_PER_STRIP) % LedSettings::LEDS_PER_STRIP;
//		leds[ledIndex1] = CRGB(0, 255, 0);

//		gravAngle = normAngleLerp(lastGravAngle, gravAngle, 0.3);
//		gravAngle = normalise(lastGravAngle + gravAngleVelocity);

	} else {
		gravAngle = normalise(lastGravAngle + gravAngleVelocity);
		gravAngleVelocity *= 0.9;
	}

//	Serial.println(String("angl e: ") + (gravAngle / 2 / PI));

	int ledIndex = gravAngle * LedSettings::LEDS_PER_STRIP;
	ledIndex = (ledIndex + LedSettings::LEDS_PER_STRIP) % LedSettings::LEDS_PER_STRIP;

	if (motion)
		leds[ledIndex] = CRGB(255, 255, 255);
	else
		leds[ledIndex] = CRGB(255, 0, 0);

	// housekeeping
	hue += 1;
	lastGravAngle = gravAngle;
}

GravAnimation::~GravAnimation() {
}

