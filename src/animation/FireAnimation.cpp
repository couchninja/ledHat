#include <animation/FireAnimation.h>

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

FireAnimation::FireAnimation(AccelManager * accelManager) :
				Animation(accelManager) {
	// Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

	heat = new byte[numLeds];
}

void FireAnimation::step() {
	counter++;
	// skip some frames
	if (counter % 1 == 0) {
		counter = 0;
	} else {
		return;
	}

	// Step 1.  Cool down every cell a little
	for (int i = 0; i < numLeds; i++) {
		heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / numLeds) + 2));
	}

	// ARON why does this use '-2' index twice?
	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int k = numLeds - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 3]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat
	if (random8() < SPARKING) {
		int y = random16(numLeds / 4);
		heat[y] = qadd8(heat[y], random8(160, 255));
		int y2 = random16(numLeds / 8);
		heat[y2] = qadd8(heat[y2], random8(160, 255));
	}

	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j < numLeds; j++) {
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		byte colorindex = scale8(heat[j], 240);
		CRGB color = ColorFromPalette(gPal, colorindex);
		int pixelnumber;
		if (gReverseDirection) {
			pixelnumber = (numLeds - 1) - j;
		} else {
			pixelnumber = j;
		}
		leds[pixelnumber] = color;
	}

	// Step 5. mirror leds on other side
	for (int j = 0; j < numLeds / 2; j++) {
		leds[numLeds - j - 1] = leds[j];
	}
}

FireAnimation::~FireAnimation() {
	delete heat;
}

