#include <animation/GravAnimation.h>
#include <numeric>

GravAnimation::GravAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

void GravAnimation::step() {
	leds.fadeToBlackBy(80);

//	leds.fill_solid(CRGB(0, 0, 0));

	Serial.println(
			String("x ") + accelManager->gravity.x + " y " + accelManager->gravity.y + " z " + accelManager->gravity.z);

	Vector3D gravSensorDelta = Vector3D(accelManager->rollingGravityDelta);
	Vector3D gravHatDelta = grav2hat(gravSensorDelta);
	gravHatDelta.normalize();

	for (float i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		Vector3D ledCoord = getRadialCoord(i);
		float gravSim = max(0.f, gravHatDelta.dot(ledCoord) - 0.5f);

//		CHSV r = CHSV(0, 255, gravSim * 256);
//		CHSV g = CHSV(85, 255, gravSim * 256);
//		CHSV b = CHSV(171, 255, gravSim * 256);

		if (accelManager->rollingGravityDelta.getMagnitude() > 0.001)
			leds[i] += CHSV(int((hue+i)*3)%256, 255, gravSim * 256);

//		leds[i] += CRGB(gravSim * 100, gravSim * 100, gravSim * 100);

//		Serial.println(
//				String("x ") + ledCoord.x + " y " + ledCoord.y + " z " + ledCoord.z);
	}

	hue += 1;
}

GravAnimation::~GravAnimation() {
}

