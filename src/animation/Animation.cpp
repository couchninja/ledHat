#include <animation/Animation.h>

Animation::Animation(int ledsPerStrip, int numOfStrips) {
	this->ledsPerStrip = ledsPerStrip;
	this->numOfStrips = numOfStrips;
	numOfLeds = ledsPerStrip * numOfStrips;
	animLeds = new CRGB[numOfLeds];
	finished = false;
}

/**
 * r: radial index [0, ledsPerStrip> clockwise
 * y: vertical index [0, numOfStrips> from top to bottom
 */
void Animation::setPixel(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t yFromTop = numOfStrips - y - 1;
	setPixelFromBottom(rIndex, yFromTop, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> clockwise
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t rCounterClock = ledsPerStrip - rIndex - 1;

	animLeds[y * ledsPerStrip + rCounterClock] = chsv;
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, ledsPerStrip> clockwise
 * y: vertical index [0, numOfStrips> from bottom to top
 */
void Animation::setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv) {
	float rCounterClock = ledsPerStrip - rIndex - 1;

	uint8_t yBott = (uint8_t) y;
	uint8_t yTop = yBott + 1;

	float topPart = y - yBott;
	float bottomPart = 1.0 - topPart;

	// clamping is needed for safety since we set chsv directly
	animLeds[clamp(yBott * ledsPerStrip + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * bottomPart);

	animLeds[clamp(yTop * ledsPerStrip + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * topPart);
}

uint8_t Animation::clamp(uint8_t ledCoord) {
	return _min(_max(ledCoord, 0), (numOfLeds - 1));
}

Animation::~Animation() {
	delete animLeds;
}

