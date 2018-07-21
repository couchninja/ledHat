#ifndef SRC_ANIMATION_DEBUGANIMATION_H_
#define SRC_ANIMATION_DEBUGANIMATION_H_

#include <animation/Animation.h>
#include <deque>

class DebugAnimation: public Animation {
public:
	DebugAnimation(AccelManager * accelManager);
	void step();
	float dotProduct(VectorFloat v1, VectorFloat v2);

	VectorFloat accel2hat(VectorFloat v);
	VectorFloat hat2accel(VectorFloat v);
	VectorFloat grav2hat(VectorFloat v);
	VectorFloat hat2grav(VectorFloat v);
	virtual ~DebugAnimation();
};

#endif /* SRC_ANIMATION_DEBUGANIMATION_H_ */
