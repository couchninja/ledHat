#include <LedManager.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 9
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

CRGBArray<NUM_LEDS> leds;

#define NUM_MODES 4;
int mode = 0;

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
		CRGB old = leds[i];
		leds[i] = hsv;
		leds[i].r = (float)leds[i].r * intensity + (float)old.r * invIntensity;
		leds[i].g = (float)leds[i].g * intensity + (float)old.g * invIntensity;
		leds[i].b = (float)leds[i].b * intensity + (float)old.b * invIntensity;

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

// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
#define COOLING  0.5

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 255

CRGBPalette16 gPal = HeatColors_p;
bool gReverseDirection = false;

void LedManager::fireStep() {
	// Array of temperature readings at each simulation cell
	static byte heat[NUM_LEDS];
	static int counter = 0;

	counter++;
	// skip some frames
	if (counter % 1 == 0) {
		counter = 0;
	} else {
		return;
	}

	// Step 1.  Cool down every cell a little
	for (int i = 0; i < NUM_LEDS; i++) {
		heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int k = NUM_LEDS - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat
	if (random8() < SPARKING) {
		int y = random16(NUM_LEDS/4);
		heat[y] = qadd8(heat[y], random8(160, 255));
		int y2 = random16(NUM_LEDS/8);
		heat[y2] = qadd8(heat[y2], random8(160, 255));
	}

	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j < NUM_LEDS; j++) {
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		byte colorindex = scale8(heat[j], 240);
		CRGB color = ColorFromPalette(gPal, colorindex);
		int pixelnumber;
		if (gReverseDirection) {
			pixelnumber = (NUM_LEDS - 1) - j;
		} else {
			pixelnumber = j;
		}
		leds[pixelnumber] = color;
	}

	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j < NUM_LEDS/2; j++) {
		leds[NUM_LEDS - j - 1] = leds[j];
	}
}

void LedManager::fireRainbowTrans(){
	static int frames = 0;

	// in frames
	int transFr = 200;
	int stayFr = 2000;

	// transition test values
//	int transFr = 100;
//	int stayFr = 100;

	// fire, always
	fireStep();

	float intensity;
	frames ++;
	if(frames < transFr){
		intensity = (float)frames / (float)transFr;
	} else if (frames < transFr + stayFr) {
		intensity = 1;
	} else if(frames < transFr + stayFr + transFr) {
		int inTransTime = frames - transFr - stayFr;
		intensity = 1.0 - ((float) inTransTime / (float)transFr);
	} else if(frames < transFr + stayFr + transFr + stayFr ) {
		intensity = 0;
	} else {
		intensity = 0;
		frames = 0;
	}

	rainbow1Step(intensity);
}

void LedManager::fillRed() {
// red
	leds.fill_solid(CHSV(255, 255, 20));
}

uint8_t LedManager::clamp(uint8_t ledCoord) {
	return _min(_max(ledCoord, 0), (NUM_LEDS-1));
}

void LedManager::step(AccelManager * accelManager) {
	switch (mode) {
	case 0:
		horizonStep(accelManager);
//		fireStep();
//		fireRainbowTrans();
		break;
	case 1:
//		movingDotStep(accelManager);
		fireRainbowTrans();
		break;
	case 2:
		rainbow2Step(accelManager);
		break;
	case 3:
//		fillRed(accelManager);
		fireStep();
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
