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
#include<vector>
#include<animation/AccelAnimation.h>
#include<animation/BrightnessSettingsAnimation.h>
#include<animation/DebugAnimation.h>
#include<animation/FireAnimation.h>
#include<animation/GravAnimation.h>
#include<animation/HorizonAnimation.h>
#include<animation/MovingDotAnimation.h>
#include<animation/MovingDotAnimation2.h>
#include<animation/RainbowAnimation.h>
#include<animation/StableDollarAnimation.h>

using namespace std;

class LedManager {
public:
	bool settingsMode = false;
	AccelManager * accelManager;

	int mode = 0;
	Animation * brightnessSettingsAnimation;
	vector<Animation*> animations;

	LedManager(int otaState, AccelManager * accelManager);
	void fillRed();
	void step();
	void fastStep();
	void handleClick();
	void handleLongPress();
	virtual ~LedManager();
};

#endif /* SRC_LEDMANAGER_H_ */
