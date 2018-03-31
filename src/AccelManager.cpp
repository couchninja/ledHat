#include <AccelManager.h>

/**
 * When moving this to header file, it will cause linker errors when the
 * parent header file is used more than once. Not sure why.
 */
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;

/* uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
 * quaternion components in a [w, x, y, z] format (not best for parsing
 * on a remote host such as Processing or something though)
 */
//#define OUTPUT_READABLE_QUATERNION
/* uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
 * (in degrees) calculated from the quaternions coming from the FIFO.
 * Note that Euler angles suffer from gimbal lock (for more info, see
 * http://en.wikipedia.org/wiki/Gimbal_lock)
 */
//#define OUTPUT_READABLE_EULER
/* uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
 * pitch/roll angles (in degrees) calculated from the quaternions coming
 * from the FIFO. Note this also requires gravity vector calculations.
 * Also note that yaw/pitch/roll angles suffer from gimbal lock (for
 * more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
 * */
//#define OUTPUT_READABLE_YAWPITCHROLL
/* uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
 * components with gravity removed. This acceleration reference frame is
 * not compensated for orientation, so +X is always +X according to the
 * sensor, just without the effects of gravity. If you want acceleration
 * compensated for orientation, use OUTPUT_READABLE_WORLDACCEL instead.
 */
//#define OUTPUT_READABLE_REALACCEL
/* uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
 * components with gravity removed and adjusted for the world frame of
 * reference (yaw is relative to initial orientation, since no magnetometer
 * is present in this case). Could be quite handy in some cases.
 */
//#define OUTPUT_READABLE_WORLDACCEL
// USE WITH WEMOS:
// int -> D5
// SCL -> D1
// SDA -> D2
// power & ground straight on Wemos, its slightly less than 5V which is perfect
// must be D5, is the only avaialble interrupt pin on wemos D1
#define INTERRUPT_PIN D5

// MPU control/status vars
// set true if DMP init was successful
bool dmpReady = false;
// holds actual interrupt status byte from MPU
uint8_t mpuIntStatus;
// return status after each device operation (0 = success, !0 = error)
uint8_t devStatus;
// expected DMP packet size (default is 42 bytes)
uint16_t packetSize;
// count of all bytes currently in FIFO
uint16_t fifoCount;
// FIFO storage buffer
uint8_t fifoBuffer[64];

VectorInt16 lastAaReal = VectorInt16();
VectorInt16 smoothAaReal = VectorInt16();
VectorInt16 lastSmoothAaReal = VectorInt16();

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
	mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

AccelManager::AccelManager() {
//void setup() {
	// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
			Fastwire::setup(400, true);
#endif
	// initialize device
	Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();
	pinMode(INTERRUPT_PIN, INPUT);

	// verify connection
	Serial.println(F("Testing device connections..."));
	Serial.println(
			mpu.testConnection() ?
					F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	// not sure if this is needed
	while (Serial.available() && Serial.read())
		; // empty buffer

	// load and configure the DMP
	Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788);

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(
				F("Enabling interrupt detection (Arduino external interrupt 0)..."));
		attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
		Serial.println(F("Interrupt received"));
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void AccelManager::step() {
	// if programming failed, don't try to do anything
	if (!dmpReady)
		return;

	long start = millis();

	// wait for MPU interrupt or extra packet(s) available
	while (!mpuInterrupt && fifoCount < packetSize) {
		// failback to prevent watchdog reboots if things go bad
		if (millis() - start > 500) {
			Serial.println(F("No mpu interrupts for 500 ms"));
			return;
		}
	}

	// reset interrupt flag and get INT_STATUS byte
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();

	// get current FIFO count
	fifoCount = mpu.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		// reset so we can continue cleanly
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
	} else if (mpuIntStatus & 0x02) {
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize)
			fifoCount = mpu.getFIFOCount();

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);

		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		mpu.dmpGetQuaternion(&q, fifoBuffer);
//		mpu.dmpGetEuler(euler, &q);
		mpu.dmpGetAccel(&aa, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
		mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

#ifdef OUTPUT_READABLE_QUATERNION
		// display quaternion values in easy matrix form: w x y z
		Serial.print("quat\t");
		Serial.print(q.w);
		Serial.print("\t");
		Serial.print(q.x);
		Serial.print("\t");
		Serial.print(q.y);
		Serial.print("\t");
		Serial.println(q.z);
#endif

#ifdef OUTPUT_READABLE_EULER
		// display Euler angles in degrees
		Serial.print("euler\t");
		Serial.print(euler[0] * 180/M_PI);
		Serial.print("\t");
		Serial.print(euler[1] * 180/M_PI);
		Serial.print("\t");
		Serial.println(euler[2] * 180/M_PI);
#endif

#ifdef OUTPUT_READABLE_YAWPITCHROLL
		// display Euler angles in rads
		Serial.print("ypr\t");
		Serial.print(ypr[0]);
		Serial.print("\t");
		Serial.print(ypr[1]);
		Serial.print("\t");
		Serial.println(ypr[2]);
#endif

#ifdef OUTPUT_READABLE_REALACCEL
		// display real acceleration, adjusted to remove gravity
		Serial.print("areal\t");
		Serial.print(aaReal.x);
		Serial.print("\t");
		Serial.print(aaReal.y);
		Serial.print("\t");
		Serial.println(aaReal.z);
#endif

#ifdef OUTPUT_READABLE_WORLDACCEL
		// display initial world-frame acceleration, adjusted to remove gravity
		// and rotated based on known orientation from quaternion
		Serial.print("aworld\t");
		Serial.print(aaWorld.x);
		Serial.print("\t");
		Serial.print(aaWorld.y);
		Serial.print("\t");
		Serial.println(aaWorld.z);
#endif

		smoothAaReal.x = lastAaReal.x * 0.2 + aaReal.x * 0.8;
		smoothAaReal.y = lastAaReal.y * 0.2 + aaReal.y * 0.8;
		smoothAaReal.z = lastAaReal.z * 0.2 + aaReal.z * 0.8;

		float diff = abs(lastAaReal.x - aaReal.x) + abs(lastAaReal.y - aaReal.y)
				+ abs(lastAaReal.z - aaReal.z);

//		if ((aaReal.x > 0 && lastAaReal.x < 0) || (aaReal.x < 0 && lastAaReal.x > 0)
//				|| (aaReal.y > 0 && lastAaReal.y < 0)
//				|| (aaReal.y < 0 && lastAaReal.y > 0)
//				|| (aaReal.z > 0 && lastAaReal.z < 0)
//				|| (aaReal.z < 0 && lastAaReal.z > 0)) {
//			motionTriggered = true;
//		}

		if ((smoothAaReal.x > 0 && lastSmoothAaReal.x < 0)
				|| (smoothAaReal.x < 0 && lastSmoothAaReal.x > 0)
				|| (smoothAaReal.y > 0 && lastSmoothAaReal.y < 0)
				|| (smoothAaReal.y < 0 && lastSmoothAaReal.y > 0)
				|| (smoothAaReal.z > 0 && lastSmoothAaReal.z < 0)
				|| (smoothAaReal.z < 0 && lastSmoothAaReal.z > 0)) {
			// not really working as i had hoped, just too randomly activated
			motionTriggered = true;
		}

		rollingDiff = rollingDiff * 0.8 + diff * 0.2;
		rollingMaxDiff *= 0.99;

//		if(rollingDiff > rollingMaxDiff) {
//			motionTriggered = true;
//		}

		rollingMaxDiff = _max(rollingDiff, rollingMaxDiff);
//		Serial.println(rollingMaxDiff);
		rollingMaxDiff = _max(400.0, rollingMaxDiff);

//		if (rollingMaxDiff / rollingDiff > 0.8) {
//			motionTriggered = true;
//		}

//		Serial.println(rollingMaxDiff);

		lastAaReal.x = aaReal.x;
		lastAaReal.y = aaReal.y;
		lastAaReal.z = aaReal.z;

		lastSmoothAaReal.x = smoothAaReal.x;
		lastSmoothAaReal.y = smoothAaReal.y;
		lastSmoothAaReal.z = smoothAaReal.z;

//		rollingDiff *= 0.2;
	}
}

bool AccelManager::consumeMotionTriggered() {
	bool motionWasTriggered = motionTriggered;
	motionTriggered = false;
	return motionWasTriggered;
}

AccelManager::~AccelManager() {
}

