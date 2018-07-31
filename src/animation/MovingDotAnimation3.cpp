#include <animation/MovingDotAnimation3.h>

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 255

MovingDotAnimation3::MovingDotAnimation3(AccelManager * accelManager) :
		Animation(accelManager) {
	heat = new byte[LedSettings::LEDS_PER_STRIP];
}

void MovingDotAnimation3::step() {
	// Cool down every cell a little
	for (int i = 0; i < LedSettings::LEDS_PER_STRIP; i++) {
		heat[i] = qsub8(heat[i], 15);
	}

  // Heat from each cell drifts 'up' and diffuses a little
//	for (int k = LedSettings::LEDS_PER_STRIP - 1; k >= 0; k--) {
//		heat[k] = (heat[normaliseLedIndex(k - 1)]
//				+ heat[normaliseLedIndex(k - 2)]
//				+ heat[normaliseLedIndex(k - 3)]) / 3;
//	}

	for (int k = LedSettings::LEDS_PER_STRIP - 1; k >= 0; k--) {
		heat[k] = (heat[normaliseLedIndex(k - 1)]
				+ heat[normaliseLedIndex(k - 2)]) / 2;
	}

//	for (int k = LedSettings::LEDS_PER_STRIP - 1; k >= 0; k--) {
//		heat[k] = heat[normaliseLedIndex(k - 1)];
//	}

	// normalize to 0 ... 1 (not sure about inclusive/exclusive)
	float pitchNormalized = (accelManager->ypr[2] + M_PI * 0.5) / M_PI;
	int offset = pitchNormalized * ledsPerStrip * 2;

	int const dots = 3;

	// Randomly ignite new 'sparks' of heat
	for (uint8_t dot = 0; dot < dots; dot++) {
		int ledIndex = normaliseLedIndex(offset + ledsPerStrip / dots * dot);

		if (random8() < SPARKING) {
			int sparkIndex = normaliseLedIndex(ledIndex + random16(5));
			heat[sparkIndex] = qadd8(heat[sparkIndex], random8(160, 255));
		}
	}

  // Map from heat cells to LED colors
	for (int j = 0; j < LedSettings::LEDS_PER_STRIP; j++) {
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		byte colorindex = scale8(heat[j], 240);
		CRGB color = ColorFromPalette(HeatColors_p, colorindex);
		leds[j] = color;
	}
}

MovingDotAnimation3::~MovingDotAnimation3() {
}
