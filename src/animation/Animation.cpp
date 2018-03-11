#include <animation/Animation.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

//CRGBArray<NUM_LEDS> leds;
//CRGB leds[NUM_LEDS];

Animation::Animation() {
//	CRGB thing[NUM_LEDS] = {0};
//	this->animLeds = thing;
	this->finished = false;
}

/**
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from top to bottom
 */
void Animation::setPixel(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t yFromTop = NUM_OF_STRIPS - y - 1;
	setPixelFromBottom(rIndex, yFromTop, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from bottom to top
 */
void Animation::setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t rCounterClock = LEDS_PER_STRIP - rIndex - 1;

	this->animLeds[y * LEDS_PER_STRIP + rCounterClock] = chsv;
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from bottom to top
 */
void Animation::setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv) {
	float rCounterClock = LEDS_PER_STRIP - rIndex - 1;

	uint8_t yBott = (uint8_t) y;
	uint8_t yTop = yBott + 1;

	float topPart = y - yBott;
	float bottomPart = 1.0 - topPart;

	// clamping is needed for safety since we set chsv directly
	this->animLeds[clamp(yBott * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h,
			chsv.s, chsv.v * bottomPart);

	this->animLeds[clamp(yTop * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h,
			chsv.s, chsv.v * topPart);
}

uint8_t Animation::clamp(uint8_t ledCoord) {
	return _min(_max(ledCoord, 0), (NUM_LEDS - 1));
}

Animation::~Animation() {
}

