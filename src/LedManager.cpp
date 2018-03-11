#include <LedManager.h>

// ARON deduplicate
#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

CRGBArray<NUM_LEDS> animleds;

#define NUM_MODES 3;
int mode = 0;

HorizonAnimation * horizonAnimation = new HorizonAnimation(LEDS_PER_STRIP, NUM_OF_STRIPS);
FireAnimation * fireAnimation = new FireAnimation(LEDS_PER_STRIP, NUM_OF_STRIPS);

LedManager::LedManager(bool wifiConnected) {
	Serial.println("Initializing LedManager");
	// Add entropy to random number generator; we use a lot of it.
//	random16_add_entropy( random());

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
	FastLED.addLeds<WS2812B, D6, GRB>(animleds, NUM_LEDS);

	if (wifiConnected)
		animleds.fill_solid(CHSV(100, 255, 100));
	else
		animleds.fill_solid(CHSV(255, 255, 100));
	FastLED.show();

	delay(200);
}

void LedManager::rainbow1Step(float intensity) {
	static uint8_t j = 0;
	j += 4;
//	Serial.print("intensity 	");
//	Serial.println(intensity);

//	leds.fill_rainbow(j);
//	intensity = 0.01;
	float invIntensity = 1.0 - intensity;
//	leds.fadeToBlackBy(intensity * 255.0);

	CHSV hsv;
	hsv.hue = j;
	hsv.val = 255;
	hsv.sat = 240;
	for( int i = 0; i < NUM_LEDS; i++) {
		CRGB old = animleds[i];
		animleds[i] = hsv;
		animleds[i].r = (float)animleds[i].r * intensity + (float)old.r * invIntensity;
		animleds[i].g = (float)animleds[i].g * intensity + (float)old.g * invIntensity;
		animleds[i].b = (float)animleds[i].b * intensity + (float)old.b * invIntensity;

//		leds[i] = old.lerp16(leds[i], 0.5);

		hsv.hue += 2;
	}
}
void LedManager::rainbow2Step(AccelManager * accelManager) {
	static uint8_t j = 0;
	j += 1;
	animleds.fill_rainbow(j);
}
void LedManager::rainbow3Step(AccelManager * accelManager) {
	static uint8_t j = 0;
	j += 4;
	animleds.fill_rainbow(j);
}

void LedManager::movingDotStep(AccelManager * accelManager) {
	static uint8_t hue;
	static uint8_t offset;

	animleds.fadeToBlackBy(80);
	animleds.blur1d(64);

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[1] + M_PI / 2) / (M_PI);
	offset = pitchNormalized * LEDS_PER_STRIP;

	offset %= LEDS_PER_STRIP;

	for (uint8_t i = 0; i < NUM_OF_STRIPS; i++) {
		animleds[i * LEDS_PER_STRIP + offset] = CHSV(hue, 255, 255);
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

	animleds[y * LEDS_PER_STRIP + rCounterClock] = chsv;
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
	animleds[clamp(yBott * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * bottomPart);

	animleds[clamp(yTop * LEDS_PER_STRIP + rCounterClock)].setHSV(chsv.h, chsv.s,
			chsv.v * topPart);
}

//void LedManager::fireRainbowTrans(){
//	static int frames = 0;
//
//	// in frames
//	int transFr = 200;
//	int stayFr = 2000;
//
//	// transition test values
////	int transFr = 100;
////	int stayFr = 100;
//
//	// fire, always
//	fireStep();
//
//	float intensity;
//	frames ++;
//	if(frames < transFr){
//		intensity = (float)frames / (float)transFr;
//	} else if (frames < transFr + stayFr) {
//		intensity = 1;
//	} else if(frames < transFr + stayFr + transFr) {
//		int inTransTime = frames - transFr - stayFr;
//		intensity = 1.0 - ((float) inTransTime / (float)transFr);
//	} else if(frames < transFr + stayFr + transFr + stayFr ) {
//		intensity = 0;
//	} else {
//		intensity = 0;
//		frames = 0;
//	}
//
//	rainbow1Step(intensity);
//}


uint8_t LedManager::clamp(uint8_t ledCoord) {
	return _min(_max(ledCoord, 0), (NUM_LEDS-1));
}

void LedManager::step(AccelManager * accelManager) {
	switch (mode) {
	case 0:
		horizonAnimation->step(accelManager);
		memmove(&animleds[0], &horizonAnimation->animLeds[0], NUM_LEDS * sizeof( CRGB));
		break;
	case 1:
		fireAnimation->step(accelManager);
		memmove(&animleds[0], &fireAnimation->animLeds[0], NUM_LEDS * sizeof( CRGB));
		break;
	case 2:
		rainbow2Step(accelManager);
		break;
	}

	FastLED.show();
}

void LedManager::nextMode() {
	mode++;
	mode %= NUM_MODES;
}

LedManager::~LedManager() {
	delete horizonAnimation;
}
