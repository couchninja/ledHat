#include "Arduino.h"
#include <OtaManager.h>
#include <LedManager.h>
#include <AccelManager.h>

/**
 * NOTES
 * - Writing lots of values to serial will interfere with acceleromere working when com port is not connected
 */

OtaManager * otaManager;
LedManager * ledManager;
AccelManager * accelManager;

long lastLedStep = 0;
long lastOtaCheck = 0;
long msPerLedFrame = 20;
long msPerOtaCheck = 50;

void setup() {
	Serial.begin(9600);
	Serial.println("Booting");

	otaManager = new OtaManager();
	ledManager = new LedManager();
	accelManager = new AccelManager();

	pinMode(2, OUTPUT);

}

void loop() {
	digitalWrite(2, LOW);
	long now = millis();

	accelManager->step();

	if (now - lastOtaCheck > msPerOtaCheck) {
		otaManager->check();
		lastOtaCheck = now;
	}

	// this does not catch up if missing a frame
	if (now - lastLedStep > msPerLedFrame) {
		ledManager->step(accelManager);
		lastLedStep = now;
	}
}
