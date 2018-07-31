#ifndef SRC_SLIDINGANIMATION_H_
#define SRC_SLIDINGANIMATION_H_

#include <animation/Animation.h>

class SlidingAnimation: public Animation {
public:
	uint8_t hue = 0;

	byte * brightness;

	SlidingAnimation(AccelManager * accelManager);
	void step();
	virtual ~SlidingAnimation();
};

#endif /* SRC_SLIDINGANIMATION_H_ */
