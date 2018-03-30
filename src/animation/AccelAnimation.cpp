#include <animation/AccelAnimation.h>

VectorInt16 lastAaReal = VectorInt16();

float rollingDiff = 0;
float rollingMaxDiff = 0;

// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
#define COOLING  10

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 255

// Array of temperature readings at each simulation cell
byte * heat;

// ARON make classvar
CRGBPalette16 gPal2 = HeatColors_p;

AccelAnimation::AccelAnimation() {
	CRGBPalette16 gPal = HeatColors_p;
	heat = new byte[numLeds];
}

void AccelAnimation::step(AccelManager * accelManager) {
//	leds.fadeToBlackBy(220);
//	leds.blur1d(64);

	float diff = abs(lastAaReal.x - accelManager->aaReal.x)
			+ abs(lastAaReal.y - accelManager->aaReal.y)
			+ abs(lastAaReal.z - accelManager->aaReal.z);
	rollingDiff += diff;
	rollingMaxDiff *= 0.99;
	rollingMaxDiff = _max(rollingDiff, rollingMaxDiff);
	rollingMaxDiff = _max(1000.0, rollingMaxDiff);

	// scaled to approx 0...255
	float intensity = rollingDiff / rollingMaxDiff * 255; // * (numStrips - 1);

	if (intensity < 200)
		intensity = 0;

//	Serial.println(rollingMaxDiff);

// Step 1.  Cool down every cell a little
	for (int i = 0; i < numLeds; i++) {
		heat[i] = qsub8(heat[i], random8(0, COOLING));
	}

//	for (int y = numStrips - 1; y >= 0; y--) {
//		CHSV c = CHSV(255, 255, intensity);
//		intensity -= 255;
//		intensity = _max(0, intensity);
//		for (int r = 0; r < ledsPerStrip; r++) {
//			heat[y * ledsPerStrip + r] = _min(255, intensity);
//		}
//	}

	int lowestStrip = numStrips - 1;
	for (int r = 0; r < ledsPerStrip; r++) {
		heat[lowestStrip * ledsPerStrip + r] = qadd8(
				heat[lowestStrip * ledsPerStrip + r], random8(0, intensity));
	}

	// Heat from each cell drifts 'up' and diffuses a little
	// We do this by going *down* trough the vertical lines
	for (int y = 0; y < numStrips; y++) {
		for (int r = 0; r < ledsPerStrip; r++) {
			if (y == 3) {
				heat[y * ledsPerStrip + r] = heat[y * ledsPerStrip + r] * 0.4
						+ heat[(y + 1) * ledsPerStrip + r] * 0.6;
			} else if (y == 4) {
				// ARON cast?
				heat[y * ledsPerStrip + r] = heat[y * ledsPerStrip + r] * 0.7;
			} else {
				heat[y * ledsPerStrip + r] = heat[y * ledsPerStrip + r] * 0.6
						+ heat[(y + 1) * ledsPerStrip + r] * 0.3
						+ heat[(y + 2) * ledsPerStrip + r] * 0.1;
			}
		}
	}

//	float intensity = abs(accelManager->aaReal.x) / 10 * 255;

//	Serial.println("intensity");
//	Serial.println(255*intensity);

// Map from heat cells to LED colors
	for (int i = 0; i < numLeds; i++) {
		// Scale the heat value from 0-255 down to 0-??
		byte colorindex = scale8(heat[i], 255);
		CRGB color = ColorFromPalette(gPal2, colorindex);
		leds[i] = color;
	}

	lastAaReal.x = accelManager->aaReal.x;
	lastAaReal.y = accelManager->aaReal.y;
	lastAaReal.z = accelManager->aaReal.z;

	rollingDiff *= 0.2;
}

AccelAnimation::~AccelAnimation() {
}

