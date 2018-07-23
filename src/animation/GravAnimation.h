#ifndef SRC_ANIMATION_GRAVANIMATION_H_
#define SRC_ANIMATION_GRAVANIMATION_H_

#include <animation/Animation.h>
#include <deque>

class GravAnimation: public Animation {
public:
	Vector3D gravHatLast = Vector3D();
	int hue = 0;
	int idleCounter = 0;

	GravAnimation(AccelManager * accelManager);
	void step();

	virtual ~GravAnimation();
};

#endif /* SRC_ANIMATION_GRAVANIMATION_H_ */
