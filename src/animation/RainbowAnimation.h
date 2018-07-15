#ifndef SRC_ANIMATION_RainbowAnimation_H_
#define SRC_ANIMATION_RainbowAnimation_H_

#include <animation/Animation.h>

class RainbowAnimation: public Animation {
public:
	RainbowAnimation(AccelManager * accelManager);
	void step();
	virtual ~RainbowAnimation();
};

#endif /* SRC_ANIMATION_RainbowAnimation_H_ */
