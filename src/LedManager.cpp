#include <LedManager.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

CRGBArray<NUM_LEDS> leds;

#define NUM_MODES 3;
int mode = 0;

HorizonAnimation * horizonAnimation = new HorizonAnimation(
LEDS_PER_STRIP,
NUM_OF_STRIPS);
FireAnimation * fireAnimation = new FireAnimation(
LEDS_PER_STRIP,
NUM_OF_STRIPS);
MovingDotAnimation * movingDotAnimation = new MovingDotAnimation(
LEDS_PER_STRIP,
NUM_OF_STRIPS);

LedManager::LedManager(bool wifiConnected) {
	Serial.println("Initializing LedManager");
	// Add entropy to random number generator; we use a lot of it.
//	random16_add_entropy( random());

	FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS);

	if (wifiConnected)
		leds.fill_solid(CHSV(100, 255, 100));
	else
		leds.fill_solid(CHSV(255, 255, 100));
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
	for (int i = 0; i < NUM_LEDS; i++) {
		CRGB old = leds[i];
		leds[i] = hsv;
		leds[i].r = (float) leds[i].r * intensity + (float) old.r * invIntensity;
		leds[i].g = (float) leds[i].g * intensity + (float) old.g * invIntensity;
		leds[i].b = (float) leds[i].b * intensity + (float) old.b * invIntensity;

//		leds[i] = old.lerp16(leds[i], 0.5);

		hsv.hue += 2;
	}
}
void LedManager::rainbow2Step(AccelManager * accelManager) {
	static uint8_t j = 0;
	j += 1;
	leds.fill_rainbow(j);
}
void LedManager::rainbow3Step(AccelManager * accelManager) {
	static uint8_t j = 0;
	j += 4;
	leds.fill_rainbow(j);
}

void LedManager::step(AccelManager * accelManager) {
	switch (mode) {
	case 0:
		horizonAnimation->step(accelManager);
		memmove(&leds[0], &horizonAnimation->leds[0],
		NUM_LEDS * sizeof(CRGB));
		break;
	case 1:
		fireAnimation->step(accelManager);
		memmove(&leds[0], &fireAnimation->leds[0], NUM_LEDS * sizeof(CRGB));
		break;
	case 2:
		movingDotAnimation->step(accelManager);
		memmove(&leds[0], &movingDotAnimation->leds[0], NUM_LEDS * sizeof(CRGB));
//		rainbow2Step(accelManager);
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
	delete horizonAnimation;
}
