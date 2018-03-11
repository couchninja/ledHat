#ifndef SRC_ANIMATION_HORIZONANIMATION_H_
#define SRC_ANIMATION_HORIZONANIMATION_H_

#include <animation/Animation.h>

class HorizonAnimation: public Animation {
public:
	HorizonAnimation(int ledsPerStrip, int numOfStrips);
	void step(AccelManager * accelManager);
	virtual ~HorizonAnimation();
};

#endif /* SRC_ANIMATION_HORIZONANIMATION_H_ */
