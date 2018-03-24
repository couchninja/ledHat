#ifndef SRC_ANIMATION_FIREANIMATION_H_
#define SRC_ANIMATION_FIREANIMATION_H_

#include <animation/Animation.h>

class FireAnimation: public Animation {
public:
	// Array of temperature readings at each simulation cell
	byte * heat;
	int counter = 0;

	FireAnimation();
	void step(AccelManager * accelManager);
	virtual ~FireAnimation();
};

#endif /* SRC_ANIMATION_FIREANIMATION_H_ */
