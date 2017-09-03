//#include "Arduino.h"
//
//#ifndef UNIT_TEST  // IMPORTANT LINE!
//
//#include<FastLED.h>
//#define NUM_LEDS 2*36
//
//CRGBArray<NUM_LEDS> leds;
//
//int counter = 0;
//
//void setup() {
//	FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
//	FastLED.addLeds<WS2812B, D5, GRB>(leds, NUM_LEDS);
//}
//
//void loop() {
//	static uint8_t hue;
//	if (counter % 100 == 0) {
//		for (int i = 0; i < NUM_LEDS; i++) {
//			leds[i] = CRGB::White;
//		}
//	}
//	// fade everything out
//	leds.fadeToBlackBy(40);
//	FastLED.delay(20);
//	counter++;
//}
//
//#endif    // IMPORTANT LINE!
