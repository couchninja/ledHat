//#include "Arduino.h"
//
//#ifndef UNIT_TEST  // IMPORTANT LINE!
//
//#include "FastLED.h"
//
//#define NUM_LEDS 4*36
//
//CRGBArray<NUM_LEDS> leds;
//bool on = false;
//
//void setup() {
//	FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
//	FastLED.addLeds<WS2812B, D5, GRB>(leds, 0, NUM_LEDS/2);
//	FastLED.addLeds<WS2812B, D6, GRB>(leds, NUM_LEDS/2, NUM_LEDS/2);
//
//	// sanity check delay - allows reprogramming if accidently blowing power w/leds
//	delay(2000);
//
//	pinMode(2, OUTPUT);
//}
//
//// This function runs over and over, and is where you do the magic to light
//// your leds.
//void loop() {
//	// Move a single white led
//	for (int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
//		// Turn our current led on to white, then show the leds
//		leds[whiteLed] = CRGB::White;
//
//		// Show the leds (only one of which is set to white, from above)
//		FastLED.show();
//
//		digitalWrite(2, whiteLed % 20 > 10);
//
//		// Wait a little bit
//		delay(20);
//
//		// Turn our current led back to black for the next loop around
//		leds[whiteLed] = CRGB::Black;
//	}
//}
//
//#endif    // IMPORTANT LINE!
//
