#include <animation/HorizonAnimation.h>

#define LEDS_PER_STRIP 36
// ARON 9...?
#define NUM_OF_STRIPS 5
#define NUM_LEDS NUM_OF_STRIPS*LEDS_PER_STRIP

//CRGB leds[NUM_LEDS];

// ARON needed?
HorizonAnimation::HorizonAnimation() :
		Animation() {
//	this->animLeds = leds;
//	this->finished = false;
}

//void Animation::step(AccelManager * accelManager) {
void HorizonAnimation::step(AccelManager * accelManager) {
	this->animLeds->fadeToBlackBy(80);

	// from -1 (backside down) to +1 (frontside down)
	float pitchNormalized = (accelManager->ypr[1]) / (M_PI * 0.5);

	float yAvgIndex = 2;
	CHSV c = CHSV(50, 255, 255);

	// 0 = back, 36*25 = right. 36*0.5 = front, 36*0.75 = left, 36*1 is back again

	// In # of strips.
	float backStripHeight = -pitchNormalized * NUM_OF_STRIPS;

	float backY = yAvgIndex + backStripHeight;
	setPixelFromBottomF(0, backY, c);
	setPixelFromBottomF(LEDS_PER_STRIP - 1, backY, c);

	// In # of strips.
	float frontStripHeight = pitchNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.5, yAvgIndex + frontStripHeight,
			c);

	// -1 (right down) to +1 (left down)
	float rollNormalized = (accelManager->ypr[2]) / (M_PI * 0.5);

	// In # of strips.
	float leftStripHeight = -rollNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.25, yAvgIndex + leftStripHeight,
			c);

	// In # of strips.
	float rightStripHeight = rollNormalized * NUM_OF_STRIPS;
	setPixelFromBottomF((LEDS_PER_STRIP - 1) * 0.75, yAvgIndex + rightStripHeight,
			c);
}

HorizonAnimation::~HorizonAnimation() {
}

