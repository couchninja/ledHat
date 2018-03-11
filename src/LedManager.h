#ifndef SRC_LEDMANAGER_H_
#define SRC_LEDMANAGER_H_

// This is needed to prevent flickering towards the end of the LED strip.
// It might cause reboots over time.
// Sources:
// - https://github.com/FastLED/FastLED/issues/306
// - https://github.com/FastLED/FastLED/issues/337
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include<AccelManager.h>
#include<animation/FireAnimation.h>
#include<animation/HorizonAnimation.h>

class LedManager {
public:
	LedManager(bool wifiConnected);
	void surfaceStep(AccelManager * accelManager);
	void rainbow1Step(float intensity);
	void rainbow2Step(AccelManager * accelManager);
	void rainbow3Step(AccelManager * accelManager);
	void movingDotStep(AccelManager * accelManager);
	void setPixel(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromBottom(uint8_t rIndex, uint8_t y, CHSV chsv);
	void setPixelFromBottomF(uint8_t rIndex, float y, CHSV chsv);
	uint8_t clamp(uint8_t ledCoord);
	void fillRed();
	void step(AccelManager * accelManager);
	void nextMode();
	virtual ~LedManager();
};

#endif /* SRC_LEDMANAGER_H_ */
