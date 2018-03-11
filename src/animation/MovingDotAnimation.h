#ifndef SRC_MOVINGDOTANIMATION_H_
#define SRC_MOVINGDOTANIMATION_H_

#include <animation/Animation.h>

class MovingDotAnimation: public Animation {
public:
	MovingDotAnimation(int ledsPerStrip, int numOfStrips);
	void step(AccelManager * accelManager);
	virtual ~MovingDotAnimation();
};

#endif /* SRC_MOVINGDOTANIMATION_H_ */
