#include <animation/AccelAnimation.h>

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

int idleCounter = 0;

AccelAnimation::AccelAnimation() {
	CRGBPalette16 gPal = HeatColors_p;
	heat = new byte[numLeds];
}

void AccelAnimation::step(AccelManager * accelManager) {
//	leds.fadeToBlackBy(220);
//	leds.blur1d(64);

// scaled to approx 0...255
	float intensity = accelManager->rollingDiff / accelManager->rollingMaxDiff
			* 255; // * (numStrips - 1);
	// high intensity if little movement
//	float intensity = (rollingMaxDiff - rollingDiff)/rollingMaxDiff * 255; // * (numStrips - 1);

	if (intensity < 230) {
		intensity = 0;
		idleCounter++;
	} else {
		idleCounter = 0;
	}

	intensity = _max(0, intensity);
//	Serial.println(intensity);

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

	// Eirinns method, not really working for me
//	bool activated = false;
//	float totalAccel = abs(accelManager->aaReal.x) + abs(accelManager->aaReal.y)
//			+ abs(accelManager->aaReal.z);
//
//	Serial.println(totalAccel);
//	if (totalAccel > 8000)
//		activated = true;

	int lowestStrip = numStrips - 1;
	if (intensity > 0) {
		for (int r = 0; r < ledsPerStrip; r++) {
			heat[lowestStrip * ledsPerStrip + r] = qadd8(
					heat[lowestStrip * ledsPerStrip + r], random8(0, 255));
		}
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

//	leds.fill_solid(CHSV(255, 255, intensity));

	// ARON
	//	if (accelManager->consumeMotionTriggered()) {
	//		leds.fill_solid(CHSV(255, 255, 255));
	//	} else {
	//		leds.fill_solid(CHSV(255, 255, 0));
	//  }

	if (idleCounter > 10) {
		float alpha = _min(1, (idleCounter - 10) / 100.0);
		float aTime = 100;

//		Serial.println(cos(((float)idleCounter)/aTime));

		this->addDollar(1, 255.0 * ((cos((idleCounter) / aTime) + 1) / 2) * alpha);
		this->addDollar(8,
				255.0 * ((cos((idleCounter + 40) / aTime) + 1) / 2) * alpha);
		this->addDollar(15,
				255.0 * ((cos((idleCounter + 80) / aTime) + 1) / 2) * alpha);
		this->addDollar(22,
				255.0 * ((cos((idleCounter + 20) / aTime) + 1) / 2) * alpha);
		this->addDollar(29,
				255.0 * ((cos((idleCounter + 60) / aTime) + 1) / 2) * alpha);
	}
}

AccelAnimation::~AccelAnimation() {
}

