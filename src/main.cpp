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
#define MS_PER_BUTTON_CHECK 50

// read serial output with:
//    pio -f -c eclipse device monitor --baud 9600
#define BAUDRATE 9600

long lastLedStep = 0;
long lastOtaCheck = 0;
long lastButtonCheck = 0;

long steps = 0;

void setup() {
	Serial.begin(BAUDRATE);
	Serial.println("Booting");
	pinMode(2, OUTPUT);
	pinMode(LedSettings::BUTTON_PIN, INPUT);

	otaManager = new OtaManager(true);
	// haven't seen this work yet but maybe the button wiring is broken
	while (!digitalRead(LedSettings::BUTTON_PIN)) {
		Serial.println("Button down at boot: check for updates");
		otaManager->check();
		delay(500);
	}

	accelManager = new AccelManager();
	ledManager = new LedManager(otaManager->state, accelManager);
	buttonManager = new ButtonManager(ledManager);
}

void loop() {
	long now = millis();

	// the button state is a bit jittery with on/off, so only check with intervals
	if (now - lastButtonCheck > MS_PER_BUTTON_CHECK) {
		buttonManager->checkButton();
		lastButtonCheck = now;
	}

	accelManager->step();

	if (now - lastOtaCheck > MS_PER_OTA_CHECK) {
		otaManager->check();
		lastOtaCheck = now;
	}

	// this does not catch up if missing a frame
	if (now - lastLedStep > MS_PER_OTA_CHECK) {
		ledManager->step();
		lastLedStep = now;
	}

	steps++;
}

