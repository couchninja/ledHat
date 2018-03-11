#include "Arduino.h"
#include <OtaManager.h>
#include <LedManager.h>
#include <AccelManager.h>

/**
 * NOTES
 * - Writing lots of values to serial will interfere with accelerometer working when com port is not connected
 */

OtaManager * otaManager;
LedManager * ledManager;
AccelManager * accelManager;

// Doesn't work with every pin; maybe because of pull-up resistor
#define BUTTON_PIN D3

#define MS_PER_LED_UPDATE 20
#define MS_PER_OTA_CHECK 50

// read serial output with: platformio -f -c eclipse device monitor --baud 9600
#define BAUDRATE 9600

long lastLedStep = 0;
long lastOtaCheck = 0;
bool buttonIsDown = false;
bool buttonWasDown = false;

void checkButton(){
	buttonIsDown = !digitalRead(BUTTON_PIN);
	if(!buttonIsDown && buttonWasDown){
		Serial.println("Button pushed");
		ledManager->nextMode();
	}
	buttonWasDown = buttonIsDown;

	digitalWrite(BUILTIN_LED, buttonIsDown);
}

void setup() {
	Serial.begin(BAUDRATE);
	Serial.println("Booting");
	pinMode(2, OUTPUT);
	pinMode(BUTTON_PIN, INPUT);

	otaManager = new OtaManager();
	// havent seen this work yet but maybe the button wiring is broken
	while(!digitalRead(BUTTON_PIN)) {
		Serial.println("Button down at boot: check for updates");
		otaManager->check();
		delay(500);
		checkButton();
	}

	// ARON new needed? is it different syntax?
	ledManager = new LedManager(otaManager->connected);

	accelManager = new AccelManager();
}

void loop() {
	checkButton();

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

