#include <animation/Animation.h>

Animation::Animation(AccelManager * accelManager) {
	this->accelManager = accelManager;
}

void Animation::handleClick() {
	Serial.println("Animation::handleClick");
}

/**
 * r: radial index [0, ledsPerStrip> clockwise
 * y: vertical index [0, numOfStrips> from top to bottom
 */
void Animation::setPixel(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t yFromTop = numStrips - y - 1;
	setPixelFromTop(rIndex, yFromTop, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> counterclockwise. 0 is the back of the hat.
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv) {
	this->setPixelFromTop(rIndex, numStrips - y - 1, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> counterclockwise. 0 is the back of the hat.
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv) {
	this->setPixelFromTopF(rIndex, numStrips - y - 1, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> counterclockwise. 0 is the back of the hat.
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromTop(uint8_t rIndex, uint8_t y, CHSV chsv) {
	if (y < numStrips) {
		rIndex %= ledsPerStrip;
		uint8_t rCounterClock = ledsPerStrip - rIndex - 1;
		leds[y * ledsPerStrip + rCounterClock] = chsv;
	}
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> counterclockwise. 0 is the back of the hat.
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromTopF(uint8_t rIndex, float y, CHSV chsv) {
	rIndex %= ledsPerStrip;
	if (y < numStrips) {
		float rCounterClock = ledsPerStrip - rIndex - 1;

		uint8_t yBott = (uint8_t) y;
		uint8_t yTop = yBott + 1;

		float topPart = y - yBott;
		float bottomPart = 1.0 - topPart;

		// clamping is needed for safety since we set chsv directly
		leds[clamp(yBott * ledsPerStrip + rCounterClock)].setHSV(chsv.h, chsv.s,
				chsv.v * bottomPart);

		leds[clamp(yTop * ledsPerStrip + rCounterClock)].setHSV(chsv.h, chsv.s,
				chsv.v * topPart);
	}
}

uint8_t Animation::clamp(uint8_t ledCoord) {
	return _min(_max(ledCoord, 0), (numLeds - 1));
}

#define ANGLE_PER_LED 1.0f/((float)LedSettings::LEDS_PER_STRIP)

Vector3D Animation::getRadialCoord(int ledIndex){
	VectorFloat ledCoord = VectorFloat();
			ledCoord.z = 0;

			// ARON make direction a variable
			// angle fraction, from back of the hat, clockwise (when looking from the top)
			float angle = ledIndex * ANGLE_PER_LED;

			// make counter clockwise
			angle = 1. - angle;
			// rotate to correct frame (+x to the right of the hat, +y to the front)
			angle = angle - 0.25;
			// convert to rads
			angle = angle * 2.f * PI;

			ledCoord.x = cos(angle);
			ledCoord.y = sin(angle);

			return ledCoord;
}

VectorFloat Animation::accel2hat(VectorFloat v) {
	return VectorFloat(v.y, -v.z, -v.x);
}

VectorFloat Animation::hat2accel(VectorFloat v) {
	return VectorFloat(-v.z, v.x, -v.y);
}

VectorFloat Animation::grav2hat(VectorFloat v) {
	return VectorFloat(v.y, v.z, -v.x);
}

VectorFloat Animation::hat2grav(VectorFloat v) {
	return VectorFloat(-v.z, v.x, v.y);
}

void Animation::addDollar(uint8_t r, uint8_t opacity) {
	CHSV dGreen = CHSV(80, 255, opacity);
	CHSV lGreen = CHSV(80, 150, opacity);
	this->setPixelFromTop(r + 1, 0, dGreen);
	this->setPixelFromTop(r + 2, 0, dGreen);
	this->setPixelFromTop(r + 3, 0, dGreen);
	this->setPixelFromTop(r, 1, dGreen);
	this->setPixelFromTop(r + 1, 2, dGreen);
	this->setPixelFromTop(r + 3, 2, dGreen);
	this->setPixelFromTop(r + 4, 3, dGreen);
	this->setPixelFromTop(r + 1, 4, dGreen);
	this->setPixelFromTop(r + 2, 4, dGreen);
	this->setPixelFromTop(r + 3, 4, dGreen);

	this->setPixelFromTop(r + 2, 1, lGreen);
	this->setPixelFromTop(r + 2, 2, lGreen);
	this->setPixelFromTop(r + 2, 3, lGreen);
}

Animation::~Animation() {
	delete leds;
}

