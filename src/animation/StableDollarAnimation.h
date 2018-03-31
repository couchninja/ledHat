#ifndef SRC_ANIMATION_STABLEDOLLARANIMATION_H_
#define SRC_ANIMATION_STABLEDOLLARANIMATION_H_

#include <animation/Animation.h>

class StableDollarAnimation: public Animation {
public:
	StableDollarAnimation();
	void step(AccelManager * accelManager);

	virtual ~StableDollarAnimation();
};

#endif /* SRC_ANIMATION_STABLEDOLLARANIMATION_H_ */
