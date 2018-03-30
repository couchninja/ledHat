#ifndef SRC_ANIMATION_ACCELANIMATION_H_
#define SRC_ANIMATION_ACCELANIMATION_H_

#include <animation/Animation.h>

class AccelAnimation: public Animation {
public:
	AccelAnimation();
	void step(AccelManager * accelManager);
	virtual ~AccelAnimation();
};

#endif /* SRC_ANIMATION_ACCELANIMATION_H_ */
