#ifndef SRC_LEDMANAGER_H_
#define SRC_LEDMANAGER_H_

// This is needed to prevent flickering towards the end of the LED strip.
// It might cause reboots over time.
// Sources:
// - https://github.com/FastLED/FastLED/issues/306
// - https://github.com/FastLED/FastLED/issues/337
#define FASTLED_ALLOW_INTERRUPTS 0
#include<FastLED.h>

class LedManager {
public:
	LedManager();
	void step();
	virtual ~LedManager();
};

#endif /* SRC_LEDMANAGER_H_ */
