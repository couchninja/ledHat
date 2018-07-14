#include "Arduino.h"
#include <OtaManager.h>
#include <LedManager.h>
#include <AccelManager.h>
#include <ButtonManager.h>

/**
 * NOTES
 * - Writing lots of values to serial will interfere with accelerometer working when com port is not connected
 */

OtaManager * otaManager;
LedManager * ledManager;
AccelManager * accelManager;
ButtonManager * buttonManager;

#define MS_PER_LED_UPDATE 20
#define MS_PER_OTA_CHECK 50

// read serial output with:
//    pio -f -c eclipse device monitor --baud 9600
#define BAUDRATE 9600

long lastLedStep = 0;
long lastOtaCheck = 0;

void setup() {
	Serial.begin(BAUDRATE);
	Serial.println("Booting");
	pinMode(2, OUTPUT);
	pinMode(LedSettings::BUTTON_PIN, INPUT);

	otaManager = new OtaManager();
	// haven't seen this work yet but maybe the button wiring is broken
	while(!digitalRead(LedSettings::BUTTON_PIN)) {
		Serial.println("Button down at boot: check for updates");
		otaManager->check();
		delay(500);
	}

	ledManager = new LedManager(otaManager->connected);
	accelManager = new AccelManager();
	buttonManager = new ButtonManager(ledManager);
}

void loop() {
	buttonManager->checkButton();

	long now = millis();

	accelManager->step();

	if (now - lastOtaCheck > MS_PER_OTA_CHECK) {
		otaManager->check();
		lastOtaCheck = now;
	}

	// this does not catch up if missing a frame
	if (now - lastLedStep > MS_PER_OTA_CHECK) {
		ledManager->step(accelManager);
		lastLedStep = now;
	}
}

