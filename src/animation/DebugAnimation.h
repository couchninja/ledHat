#ifndef SRC_ANIMATION_DEBUGANIMATION_H_
#define SRC_ANIMATION_DEBUGANIMATION_H_

#include <animation/Animation.h>
#include <deque>

class DebugAnimation: public Animation {
public:
	DebugAnimation(AccelManager * accelManager);
	void step();
	virtual ~DebugAnimation();
};

#endif /* SRC_ANIMATION_DEBUGANIMATION_H_ */
