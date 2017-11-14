#ifndef SRC_LEDMANAGER_H_
#define SRC_LEDMANAGER_H_

#include<AccelManager.h>
// This is needed to prevent flickering towards the end of the LED strip.
// It might cause reboots over time.
// Sources:
// - https://github.com/FastLED/FastLED/issues/306
// - https://github.com/FastLED/FastLED/issues/337
#define FASTLED_ALLOW_INTERRUPTS 0

#include<FastLED.h>

class LedManager {
public:
	LedManager(bool wifiConnected);
	void step(AccelManager * accelManager);
	void nextMode();
	virtual ~LedManager();
};

#endif /* SRC_LEDMANAGER_H_ */
