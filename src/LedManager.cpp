#include <LedManager.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

CRGBArray<NUM_LEDS> leds;

#define NUM_MODES 4;
int mode = 0;

LedManager::LedManager(bool wifiConnected) {
	Serial.println("Initializing LedManager");
	FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);

	if (wifiConnected)
		leds.fill_solid(CHSV(100, 255, 100));
	else
		leds.fill_solid(CHSV(255, 255, 100));
	FastLED.show();

	delay(200);
}

/**
 * Random reboots on horizon test mode, why?
 * - extensive logging is off
 * - seems to happen when tipping over but not always
 * -- divide by zero? -> i dont, but maybe lib -> but movingDotStep also use the same code accel and no probs
 * -- loose contact? -> shaking doesnt crash it
 * -- cpu overheat? -> running long is not a problem
 * -- out of array? (but fastled seems to guard for this)
 * - seems to trigger several times in a while...
 *
 * Hokey! After all the testing, the leds turned off uless you squeeze the contacts from the side
 * Hopefully this is all because of loose contacts (would explain mode switching also).
 * If problem still occurs -> comment out code in setPixelFromBottomF and see what happens.
 * call rainbow mode as graphical debug.
 * Did problem also occur with setPixelFromBottom? cant remember.
 */
void LedManager::horizonStep(AccelManager * accelManager) {
	leds.fadeToBlackBy(80);

	// from -1 (backside down) to +1 (frontside down)
	float pitchNormalized = (accelManager->ypr[1]) / (M_PI * 0.5);

	float yAvgIndex = 2;
	CHSV c = CHSV(50, 255, 255);

	// 0 = back, 36*25 = right. 36*0.5 = front, 36*0.75 = left, 36*1 is back again

	// In # of strips.
	float backStripHeight = -pitchNormalized * NUM_OF_STRIPS;

	float backY = yAvgIndex + backStripHeight;
	setPixelFromBottomF(0, backY, c);
	setPixelFromBottomF(LEDS_PER_STRIP - 1, backY, c);

	// In # of strips.
	float frontStripHeight = pitchNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.5, yAvgIndex + frontStripHeight,
			c);

	// -1 (right down) to +1 (left down)
	float rollNormalized = (accelManager->ypr[2]) / (M_PI * 0.5);

	// In # of strips.
	float leftStripHeight = -rollNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.25, yAvgIndex + leftStripHeight,
			c);

	// In # of strips.
	float rightStripHeight = rollNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.75, yAvgIndex + rightStripHeight,
			c);
}

void LedManager::rainbowStep(AccelManager * accelManager) {
	static uint8_t j = 0;
	j += 10;
	leds.fill_rainbow(j);
}

void LedManager::movingDotStep(AccelManager * accelManager) {
	static uint8_t hue;
	static uint8_t offset;

	leds.fadeToBlackBy(80);
	leds.blur1d(64);

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[1] + M_PI / 2) / (M_PI);
	offset = pitchNormalized * LEDS_PER_STRIP;

	offset %= LEDS_PER_STRIP;

	for (uint8_t i = 0; i < NUM_OF_STRIPS; i++) {
		leds[i * LEDS_PER_STRIP + offset] = CHSV(hue, 255, 255);
	}

	hue++;
}

/**
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from top to bottom
 */
void LedManager::setPixel(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t yFromTop = NUM_OF_STRIPS - y - 1;
	setPixelFromBottom(rIndex, yFromTop, chsv);
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from bottom to top
 */
void LedManager::setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv) {
	uint8_t rCounterClock = LEDS_PER_STRIP - rIndex - 1;

	leds[y * LEDS_PER_STRIP + rCounterClock] = chsv;
}

/**
 * Resilient agains out-of-bounds coordinates (thanks to fastLed)
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from bottom to top
 */
void LedManager::setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv) {
	float rCounterClock = LEDS_PER_STRIP - rIndex - 1;

	uint8_t yBott = (uint8_t) y;
	uint8_t yTop = yBott + 1;

	float topPart = y - yBott;
	float bottomPart = 1.0 - topPart;

	// clamping is needed for safety since we set chsv directly
	leds[clamp(yBott * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * bottomPart);

	leds[clamp(yTop * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * topPart);
}

uint8_t LedManager::clamp(uint8_t ledCoord) {
	return min(max(ledCoord, 0), NUM_LEDS-1);
}

void LedManager::fillRed(AccelManager * accelManager) {
// red
	leds.fill_solid(CHSV(255, 255, 20));
}

void LedManager::step(AccelManager * accelManager) {
	switch (mode) {
	case 0:
		horizonStep(accelManager);
		break;
	case 1:
		movingDotStep(accelManager);
		break;
	case 2:
		rainbowStep(accelManager);
		break;
	case 3:
		fillRed(accelManager);
		break;
	}

	FastLED.show();
}

void LedManager::nextMode() {
	mode++;
	mode %= NUM_MODES
	;
}

LedManager::~LedManager() {
}
