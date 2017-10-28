#ifndef SRC_ACCELMANAGER_H_
#define SRC_ACCELMANAGER_H_

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

// see https://github.com/jrowberg/i2cdevlib
#include "I2Cdev.h"

class AccelManager {
public:
	float euler[3]; // [psi, theta, phi]    Euler angle container
	float ypr[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

	AccelManager();
	void step();
	virtual ~AccelManager();
};

#endif /* SRC_ACCELMANAGER_H_ */
