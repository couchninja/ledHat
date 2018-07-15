#ifndef SRC_ANIMATION_STABLEDOLLARANIMATION_H_
#define SRC_ANIMATION_STABLEDOLLARANIMATION_H_

#include <animation/Animation.h>

class StableDollarAnimation: public Animation {
public:
	StableDollarAnimation(AccelManager * accelManager);
	void step();

	virtual ~StableDollarAnimation();
};

#endif /* SRC_ANIMATION_STABLEDOLLARANIMATION_H_ */
