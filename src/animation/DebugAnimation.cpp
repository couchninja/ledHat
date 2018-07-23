#include <animation/DebugAnimation.h>


DebugAnimation::DebugAnimation(AccelManager * accelManager) :
		Animation(accelManager) {
}

void DebugAnimation::step() {
//	leds.fadeToBlackBy(80);
//
//	// from -1 (backside down) to +1 (frontside down)
//	float pitchNormalized = -(accelManager->ypr[2]) / (M_PI * 0.5);
//
//	float yAvgIndex = 2;
//	CHSV c = CHSV(50, 255, 255);
//
//	// 0 = back, 36*25 = right. 36*0.5 = front, 36*0.75 = left, 36*1 is back again
//
//	// In # of strips.
//	float backStripHeight = -pitchNormalized * numStrips;
//
//	float backY = yAvgIndex + backStripHeight;
//	setPixelFromBottomF(0, backY, c);
//	setPixelFromBottomF(ledsPerStrip - 1, backY, c);
//
//	// In # of strips.
//	float frontStripHeight = pitchNormalized * numStrips;
//	setPixelFromBottomF((ledsPerStrip - 1) * 0.5, yAvgIndex + frontStripHeight,
//			c);
//
//	// -1 (right down) to +1 (left down)
//	float rollNormalized = -(accelManager->ypr[1]) / (M_PI * 0.5);
//
//	// In # of strips.
//	float leftStripHeight = rollNormalized * numStrips;
//	setPixelFromBottomF((ledsPerStrip - 1) * 0.75, yAvgIndex + leftStripHeight,
//			c);
//
//	// In # of strips.
//	float rightStripHeight = -rollNormalized * numStrips;
//	setPixelFromBottomF((ledsPerStrip - 1) * 0.25, yAvgIndex + rightStripHeight,
//			c);

	leds.fill_solid(CRGB(0, 0, 0));

//	Serial.println(
//			String("x ") + accelManager->gravity.x + " y " + accelManager->gravity.y + " z " + accelManager->gravity.z);

//	Serial.println("start loop");

	VectorFloat x = VectorFloat(1, 0, 0);
	VectorFloat y = VectorFloat(0, 1, 0);
	VectorFloat z = VectorFloat(0, 0, 1);

//	for (float i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
//		VectorFloat ledCoord = VectorFloat();
//		ledCoord.z = 0;
//
//		// ARON make direction a variable
//		// angle fraction, from back of the hat, clockwise (when looking from the top)
//		// ARON check boundaries
//		// ARON optimize
//		float angle = (i / LedSettings::LEDS_PER_STRIP);
//
//		// make counter clockwise
//		angle = 1. - angle;
//		// rotate to correct frame (+x to the right of the hat, +y to the front)
//		angle = angle - 0.25;
//		// convert to rads
//		angle = angle * 2. * PI;
//
//		ledCoord.x = cos(angle);
//		ledCoord.y = sin(angle);
//
////		VectorFloat vaa = VectorFloat(accelManager->aaReal.x, accelManager->aaReal.y, accelManager->aaReal.z);
//		VectorFloat grav = VectorFloat(accelManager->gravity.x, accelManager->gravity.y, accelManager->gravity.z);
//
//		grav.normalize();
//
//		VectorFloat gravHat = accel2hat(grav);
//
//		float xSim = max(0.f, dotProduct(ledCoord, gravHat)-0.9f);
//		float ySim = max(0.f, dotProduct(ledCoord, gravHat)-0.9f);
//		float zSim = max(0.f, dotProduct(ledCoord, gravHat)-0.9f);
//
//		CHSV r = CHSV(0, 255, (xSim)*256);
//		CHSV g = CHSV(85, 255, (ySim)*256);
//		CHSV b = CHSV(171, 255, (zSim)*256);
//
//		leds[i] = r + g + b;
//	}

	for (float i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		VectorFloat ledCoord = VectorFloat();
		ledCoord.z = 0;

		// ARON make direction a variable
		// angle fraction, from back of the hat, clockwise (when looking from the top)
		// ARON check boundaries
		// ARON optimize
		float angle = (i / LedSettings::LEDS_PER_STRIP);

		// make counter clockwise
		angle = 1. - angle;
		// rotate to correct frame (+x to the right of the hat, +y to the front)
		angle = angle - 0.25;
		// convert to rads
		angle = angle * 2. * PI;

		ledCoord.x = cos(angle);
		ledCoord.y = sin(angle);

//		VectorFloat xRot = VectorFloat(1, 0, 0);
//		xRot = hat2accel(xRot);
//		xRot.rotate(&accelManager->q);
//		xRot = accel2hat(xRot);
//		float xSim = max(0.f, dotProduct(ledCoord, xRot)-0.9f);
//
//		VectorFloat yRot = VectorFloat(0, 1, 0);
//		yRot = hat2accel(yRot);
//		yRot.rotate(&accelManager->q);
//		yRot = accel2hat(yRot);
//		float ySim = max(0.f, dotProduct(ledCoord, yRot)-0.9f);
//
//		VectorFloat zRot = VectorFloat(0, 0, 1);
//		zRot = hat2accel(zRot);
//		zRot.rotate(&accelManager->q);
//		zRot = accel2hat(zRot);
//		float zSim = max(0.f, dotProduct(ledCoord, zRot)-0.9f);

//		VectorFloat yRot = VectorFloat(0, 1, 0);
//		yRot.rotate(&accelManager->q);
//		VectorFloat xRot = VectorFloat(1, 0, 0);
//		xRot.rotate(&accelManager->q);

		// visualize XYZ coords

		// this does not do proper gamma adjustments
		//		leds[i] = CRGB(
		//				128. + 128. * ledCoord.x,
		//				128 + 128. * ledCoord.y,
		//				128. + 128. * ledCoord.z
		//				);

		// this handles gamma better, but need to double check
//		CHSV r = CHSV(0, 255, (128. + 128. * ledCoord.x));
//		CHSV g = CHSV(85, 255, (128. + 128. * ledCoord.y));
//		CHSV b = CHSV(171, 255, (128. + 128. * ledCoord.z));

//		leds[i] = r + g + b;

//		ledCoord.rotate(&accelManager->q);

//		float similarity = max(0.f, dotProduct(ledCoord, y) - 0.9f);

//		leds[i] = CHSV(0, 255, (256 * similarity));

//		Serial.println(
//				String("x ") + ledCoord.x + " y " + ledCoord.y + " z " + ledCoord.z);
	}

//	VectorFloat zRot = VectorFloat(0,0,1);
//	zRot.rotate(&accelManager->q);
//	VectorFloat yRot = VectorFloat(0,1,0);
//	yRot.rotate(&accelManager->q);
//	VectorFloat xRot = VectorFloat(1,0,0);
//	xRot.rotate(&accelManager->q);

//	Serial.println(dotProduct(x, xRot));

//	leds.fill_solid(CRGB(256*max(0.f, dotProduct(x, xRot)), 0, 0));
//	Serial.println(
//			String("x ") + xRot.x + " y " + xRot.y + " z " + xRot.z);

//	leds.fill_solid(CRGB(256 * max(0.f, dotProduct(z, zRot)), 0, 0));
//	Serial.println(
//			String("x ") + zRot.x + " y " + zRot.y + " z " + zRot.z);

//	float  = -(accelManager->ypr[2]) / (M_PI * 0.5);

	///// GRAVHAT

	Vector3D gravSensorDelta = Vector3D(accelManager->rollingGravityDelta);
	Vector3D gravHatDelta = grav2hat(gravSensorDelta);

//	VectorFloat gravHat = grav2hat(Vector3D(accelManager->gravity.x,
//			accelManager->gravity.y,
//			accelManager->gravity.z));
//	VectorFloat gravHatDelta = Vector3D(
//			gravHatLast.x - gravHat.x,
//			gravHatLast.y - gravHat.y,
//			gravHatLast.z - gravHat.z
//			);
	gravHatDelta.normalize();

	for (float i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		VectorFloat ledCoord = VectorFloat();
		ledCoord.z = 0;

		// ARON make direction a variable
		// angle fraction, from back of the hat, clockwise (when looking from the top)
		// ARON check boundaries
		// ARON optimize
		float angle = (i / LedSettings::LEDS_PER_STRIP);

		// make counter clockwise
		angle = 1. - angle;
		// rotate to correct frame (+x to the right of the hat, +y to the front)
		angle = angle - 0.25;
		// convert to rads
		angle = angle * 2. * PI;

		ledCoord.x = cos(angle);
		ledCoord.y = sin(angle);

		float gravSim = max(0.f, gravHatDelta.dot(ledCoord) - 0.5f);

		CHSV r = CHSV(0, 255, (gravSim) * 256);
		CHSV g = CHSV(85, 255, (gravSim) * 256);
		CHSV b = CHSV(171, 255, (gravSim) * 256);

		leds[i] = r + g + b;

		//		VectorFloat yRot = VectorFloat(0, 1, 0);
		//		yRot.rotate(&accelManager->q);
		//		VectorFloat xRot = VectorFloat(1, 0, 0);
		//		xRot.rotate(&accelManager->q);

		// visualize XYZ coords

		// this does not do proper gamma adjustments
		//		leds[i] = CRGB(
		//				128. + 128. * ledCoord.x,
		//				128 + 128. * ledCoord.y,
		//				128. + 128. * ledCoord.z
		//				);

		// this handles gamma better, but need to double check
		//		CHSV r = CHSV(0, 255, (128. + 128. * ledCoord.x));
		//		CHSV g = CHSV(85, 255, (128. + 128. * ledCoord.y));
		//		CHSV b = CHSV(171, 255, (128. + 128. * ledCoord.z));

		//		leds[i] = r + g + b;

		//		ledCoord.rotate(&accelManager->q);

		//		float similarity = max(0.f, dotProduct(ledCoord, y) - 0.9f);

		//		leds[i] = CHSV(0, 255, (256 * similarity));

				Serial.println(
						String("x ") + ledCoord.x + " y " + ledCoord.y + " z " + ledCoord.z);
	}

//	gravHatLog.push_back(gravHat);
//	if(gravHatLog.size() > 10) {
//		gravHatLog.pop_front();
//	}
//	gravHatLast = gravHat;
}

DebugAnimation::~DebugAnimation() {
}

