#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <AccelManager.h>

#define LEDS_PER_STRIP 36
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

class Animation {
public:
//	CRGB * animLeds;
	CRGB animLeds[NUM_LEDS];bool finished;
	Animation();
	virtual void step(AccelManager * accelManager) = 0;
	void setPixel(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv);
	uint8_t clamp(uint8_t ledCoord);

	virtual ~Animation();
};

#endif /* SRC_ANIMATION_H_ */
