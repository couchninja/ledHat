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
#include <util/Vector3D.h>

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
	Vector3D gravity;
	// [psi, theta, phi] Euler angle container
	float euler[3];
	// [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
	float ypr[3];


	float rollingAARealDiff = 0;
	float rollingMaxAARealDiff = 0;

	Vector3D rollingGravity = Vector3D();
	Vector3D lastRollingGravity = Vector3D();
	Vector3D rollingGravityDelta= Vector3D();

	bool motionTriggered = false;
	bool consumeMotionTriggered();

	AccelManager();
	void step();
	virtual ~AccelManager();
};

#endif /* SRC_ACCELMANAGER_H_ */
