#include "Arduino.h"

#include <OtaManager.h>
#include <LedManager.h>
#include <AccelManager.h>

OtaManager * otaManager;
LedManager * ledManager;
AccelManager * accelManager;

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

	otaManager->check();
	accelManager->step();
	ledManager->step(accelManager);
}
