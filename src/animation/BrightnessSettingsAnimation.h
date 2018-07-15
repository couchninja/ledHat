#ifndef SRC_ANIMATION_BRIGHTNESSSETTINGSANIMATION_H_
#define SRC_ANIMATION_BRIGHTNESSSETTINGSANIMATION_H_

#include <animation/Animation.h>

class BrightnessSettingsAnimation: public Animation {
public:
	int brightnessIndex = 0;
	int brightnesses [5] = {10, 20, 50, 100, 500};

	BrightnessSettingsAnimation(AccelManager * accelManager);
	void step();
	void handleClick() override;
	virtual ~BrightnessSettingsAnimation();
};

#endif /* SRC_ANIMATION_BRIGHTNESSSETTINGSANIMATION_H_ */
