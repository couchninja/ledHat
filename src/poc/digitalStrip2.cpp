////#include "Arduino.h"
//
//#ifndef UNIT_TEST  // IMPORTANT LINE!
//
//#define FASTLED_ALLOW_INTERRUPTS 0
//#include<FastLED.h>
//#include <ESP8266WiFi.h>
//
//#define NUM_LEDS 10*36
//
//CRGBArray<NUM_LEDS> leds;
//
//void setup() {
//	FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
//	FastLED.addLeds<WS2812B, D5, GRB>(leds, NUM_LEDS);
//}
//
//void loop() {
//	static uint8_t hue;
//	static uint8_t i;
//	//WiFi.mode(WIFI_OFF);
//
//	//for(int i = 0; i < NUM_LEDS; i++) {
//	// let's set an led value
//	leds.fadeToBlackBy(80);    // fade everything out
//	leds[i] = CHSV(hue++, 255, 255);
//	i++;
//	if (i >= NUM_LEDS) {
//		i = 0;
//	}
//	// now, let's first 20 leds to the top 20 leds,
//	//leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
//	FastLED.delay(50);
//	FastLED.show();
//	//yield();
//	//}
//
//}
//
//#endif    // IMPORTANT LINE!
