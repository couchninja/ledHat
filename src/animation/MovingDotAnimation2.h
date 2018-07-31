#ifndef SRC_MOVINGDOTANIMATION2_H_
#define SRC_MOVINGDOTANIMATION2_H_

#include <animation/Animation.h>

class MovingDotAnimation2: public Animation {
public:
	uint8_t hue = 0;

	byte * brightness;

	MovingDotAnimation2(AccelManager * accelManager);
	void step();
	virtual ~MovingDotAnimation2();
};

#endif /* SRC_MOVINGDOTANIMATION2_H_ */
