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

void LedManager::surfaceStep(AccelManager * accelManager) {
	leds.fadeToBlackBy(80);
	leds.blur1d(64);

	// from -1 (backside down) to +1 (frontside down)
	float pitchNormalized = (accelManager->ypr[1]) / (M_PI * 0.5);
	Serial.print("pitch: ");
	Serial.println(pitchNormalized);

	// 0 = back, 36*25 = right. 36*0.5 = front, 36*0.75 = left, 36*1 is back again

	// In # of strips.
	float backStripHeight = max(-pitchNormalized, 0) * NUM_OF_STRIPS;

	for (uint8_t i = 0; i < backStripHeight; i++) {
		lightPixel(0, i);
		lightPixel(LEDS_PER_STRIP - 1, i);
	}

	// In # of strips.
	float frontStripHeight = max(pitchNormalized, 0) * NUM_OF_STRIPS;
	for (uint8_t i = 0; i < frontStripHeight; i++) {
		lightPixel((LEDS_PER_STRIP - 1) * 0.5, i);
	}

	// -1 (right down) to +1 (left down)
	float rollNormalized = (accelManager->ypr[2]) / (M_PI * 0.5);
	Serial.print("roll: ");
	Serial.println(rollNormalized);

	// In # of strips.
	float leftStripHeight = max(-rollNormalized, 0) * NUM_OF_STRIPS;
	for (uint8_t i = 0; i < leftStripHeight; i++) {
		lightPixel((LEDS_PER_STRIP - 1) * 0.75, i);
	}

	// In # of strips.
	float rightStripHeight = max(rollNormalized, 0) * NUM_OF_STRIPS;
	for (uint8_t i = 0; i < rightStripHeight; i++) {
		lightPixel((LEDS_PER_STRIP - 1) * 0.25, i);
	}
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
 * ARON todo: swap coordinates
 * r: radial index [0, LEDS_PER_STRIP> clockwise
 * y: vertical index [0, NUM_OF_STRIPS> from bottom to top
 */
void LedManager::lightPixel(uint8_t r, float y) {
	uint8_t rCounterClock = LEDS_PER_STRIP - r;
	leds[y * LEDS_PER_STRIP + rCounterClock] = CHSV(255, 255, 255);
}

void LedManager::fillRed(AccelManager * accelManager) {
	// red
	leds.fill_solid(CHSV(255, 255, 20));
}

void LedManager::step(AccelManager * accelManager) {
	switch (mode) {
	case 0:
		surfaceStep(accelManager);
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
