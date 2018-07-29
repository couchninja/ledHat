#ifndef SRC_ANIMATION_GRAVANIMATION_H_
#define SRC_ANIMATION_GRAVANIMATION_H_

#include <animation/Animation.h>
#include <deque>

class GravAnimation: public Animation {
public:
	Vector3D gravHatLast = Vector3D();
	int hue = 0;
	int idleCounter = 0;

	float gravAngle = 0;
	float smoothGravAngle = 0;
	float lastGravAngle = 0;
	float gravAngleVelocity = 0;
	float gravAngleAcceleration = 0;
	float motionNess = 0;

	byte * heat;

	GravAnimation(AccelManager * accelManager);
	void step();
	void fastStep();
	void oldStep();

	virtual ~GravAnimation();
};

#endif /* SRC_ANIMATION_GRAVANIMATION_H_ */
