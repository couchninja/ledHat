//#include "Arduino.h"
//
//#ifndef UNIT_TEST  // IMPORTANT LINE!
//
//#include<FastLED.h>
//#define NUM_LEDS 4*36
//
//CRGBArray<NUM_LEDS> leds;
//
//void setup() {
//	FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);
//	FastLED.addLeds<WS2812B, D5, GRB>(leds, NUM_LEDS);
//	}
//
//void loop(){
//  static uint8_t hue;
//  for(int i = 0; i < NUM_LEDS/2; i++) {
//    // fade everything out
//    leds.fadeToBlackBy(40);
//
//    // let's set an led value
//    leds[i] = CHSV(hue++,255,255);
//
//    // now, let's first 20 leds to the top 20 leds,
//    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
//    FastLED.delay(200);
//  }
//}
//
//#endif    // IMPORTANT LINE!
