#ifndef SRC_LEDMANAGER_H_
#define SRC_LEDMANAGER_H_

// This is needed to prevent flickering towards the end of the LED strip.
// It might cause reboots over time.
// Sources:
// - https://github.com/FastLED/FastLED/issues/306
// - https://github.com/FastLED/FastLED/issues/337
#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>
#include<settings/ledSettings.h>
#include<AccelManager.h>
#include<OtaManager.h>
#include<animation/AccelAnimation.h>
#include<animation/FireAnimation.h>
#include<animation/HorizonAnimation.h>
#include<animation/MovingDotAnimation.h>
#include<animation/RainbowAnimation.h>
#include<animation/StableDollarAnimation.h>
#include<animation/BrightnessSettingsAnimation.h>

class LedManager {
public:
	bool settingsMode = false;
	AccelManager * accelManager;

	Animation * horizonAnimation;
	Animation * fireAnimation;
	Animation * movingDotAnimation;
	Animation * rainbowAnimation;
	Animation * accelAnimation;
	Animation * stableDollarAnimation;
	Animation * brightnessSettingsAnimation;

	LedManager(int otaState, AccelManager * accelManager);
	void fillRed();
	void step();
	void handleClick();
	void handleLongPress();
	virtual ~LedManager();
};

#endif /* SRC_LEDMANAGER_H_ */
