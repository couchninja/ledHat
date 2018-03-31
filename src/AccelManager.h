#ifndef SRC_ACCELMANAGER_H_
#define SRC_ACCELMANAGER_H_

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// see https://github.com/jrowberg/i2cdevlib
#include "I2Cdev.h"

#include "helper_3dmath.h"

class AccelManager {
public:
	// orientation/motion vars
	// [w, x, y, z] quaternion container
	Quaternion q;
	// [x, y, z] accel sensor measurements
	VectorInt16 aa;
	// [x, y, z] gravity-free accel sensor measurements
	VectorInt16 aaReal;
	// [x, y, z] world-frame accel sensor measurements
	VectorInt16 aaWorld;
	// [x, y, z] gravity vector
	VectorFloat gravity;
	// [psi, theta, phi] Euler angle container
	float euler[3];
	// [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
	float ypr[3];

	// ARON
	float rollingDiff = 0;
	float rollingMaxDiff = 0;
	bool motionTriggered = false;
	bool consumeMotionTriggered();

	AccelManager();
	void step();
	virtual ~AccelManager();
};

#endif /* SRC_ACCELMANAGER_H_ */
