#include <animation/HorizonAnimation.h>

HorizonAnimation::HorizonAnimation(AccelManager * accelManager) :
Animation(accelManager) {
}

void HorizonAnimation::step() {
	leds.fadeToBlackBy(80);

	// from -1 (backside down) to +1 (frontside down)
	float pitchNormalized = -(accelManager->ypr[2]) / (M_PI * 0.5);

	float yAvgIndex = 2;
	CHSV c = CHSV(50, 255, 255);

	// 0 = back, 36*25 = right. 36*0.5 = front, 36*0.75 = left, 36*1 is back again

	// In # of strips.
	float backStripHeight = -pitchNormalized * numStrips;

	float backY = yAvgIndex + backStripHeight;
	setPixelFromBottomF(0, backY, c);
	setPixelFromBottomF(ledsPerStrip - 1, backY, c);

	// In # of strips.
	float frontStripHeight = pitchNormalized * numStrips;
	setPixelFromBottomF((ledsPerStrip - 1) * 0.5, yAvgIndex + frontStripHeight,
			c);

	// -1 (right down) to +1 (left down)
	float rollNormalized = -(accelManager->ypr[1]) / (M_PI * 0.5);

	// In # of strips.
	float leftStripHeight = rollNormalized * numStrips;
	setPixelFromBottomF((ledsPerStrip - 1) * 0.75, yAvgIndex + leftStripHeight,
			c);

	// In # of strips.
	float rightStripHeight = -rollNormalized * numStrips;
	setPixelFromBottomF((ledsPerStrip - 1) * 0.25, yAvgIndex + rightStripHeight,
			c);
}

HorizonAnimation::~HorizonAnimation() {
}

