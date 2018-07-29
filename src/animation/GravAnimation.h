#ifndef SRC_ANIMATION_GRAVANIMATION_H_
#define SRC_ANIMATION_GRAVANIMATION_H_

#include <animation/Animation.h>
#include <deque>

class GravAnimation: public Animation {
public:
	Vector3D gravHatLast = Vector3D();
	int hue = 0;
	int idleCounter = 0;

	// these are all fractional angles (1 is a full circle). starting from back of hat, clockwise
	float gravFrangle = 0;
	float smoothGravFrangle = 0;
	float lastGravFrangle = 0;
	float gravFrangleVelocity = 0;
	float gravFrangleAcceleration = 0;
	float motionNess = 0;

	byte * brightness;

	GravAnimation(AccelManager * accelManager);
	void step();
	void fastStep();
	void oldStep();

	virtual ~GravAnimation();
};

#endif /* SRC_ANIMATION_GRAVANIMATION_H_ */
