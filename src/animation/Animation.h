#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_

#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <AccelManager.h>
#include <settings/ledSettings.h>


class Animation {
public:
	int ledsPerStrip = LedSettings::LEDS_PER_STRIP;
	int numStrips = LedSettings::NUM_STRIPS;
	int numLeds = LedSettings::NUM_LEDS;
	AccelManager * accelManager;

	Animation(AccelManager * accelManager);

	// Tried to make these not initialize with the header file: not sure how,
	// because this will need to be a reference, and if that is initialized
	// inside this class it will always go out of scope. Maybe make a helper
	// wrapper to hold the pointer?
	CRGB rawleds[LedSettings::NUM_LEDS];
	CPixelView<CRGB> leds = CPixelView<CRGB>(rawleds, LedSettings::NUM_LEDS);

	virtual void step() = 0;
	virtual void handleClick();

	void setPixel(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromTop(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromTopF(uint8_t rIndex, float y, CHSV chsv);
	void setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv);
	uint8_t clamp(uint8_t ledCoord);
	void addDollar(uint8_t r, uint8_t opacity);

	virtual ~Animation();
};

#endif /* SRC_ANIMATION_H_ */
