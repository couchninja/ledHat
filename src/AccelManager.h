#ifndef SRC_ACCELMANAGER_H_
#define SRC_ACCELMANAGER_H_



class AccelManager {
public:
	float euler[3];         // [psi, theta, phi]    Euler angle container
	float ypr[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

	AccelManager();
	void step();
	virtual ~AccelManager();
};

#endif /* SRC_ACCELMANAGER_H_ */
