#ifndef SRC_MOVINGDOTANIMATION3_H_
#define SRC_MOVINGDOTANIMATION3_H_

#include <animation/Animation.h>

class MovingDotAnimation3: public Animation {
public:
	byte * heat;

	MovingDotAnimation3(AccelManager * accelManager);
	void step();
	virtual ~MovingDotAnimation3();
};

#endif /* SRC_MOVINGDOTANIMATION3_H_ */
