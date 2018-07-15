#include <ButtonManager.h>

ButtonManager::ButtonManager(LedManager * ledManager) {
	Serial.println("Initializing ButtonManager");
	this->ledManager = ledManager;
}

void ButtonManager::checkButton() {
	bool buttonIsDown = !digitalRead(LedSettings::BUTTON_PIN);

	if (!ignoreEverythingUntilNextUp
			&& this->buttonDownTimestamp != -1
			&& (millis() - this->buttonDownTimestamp) > 2000) {
		// long press
		ledManager->handleLongPress();
		ignoreEverythingUntilNextUp = true;
	}

	if (buttonIsDown != buttonWasDown) {
		if (buttonIsDown) {
			this->buttonDownTimestamp = millis();
		} else {
			if (!ignoreEverythingUntilNextUp) {
				// button went back up
				Serial.println("Button pushed");
				this->ledManager->handleClick();
			}
			this->buttonDownTimestamp = -1;
			ignoreEverythingUntilNextUp = false;
		}
	}

	buttonWasDown = buttonIsDown;

	digitalWrite(BUILTIN_LED, buttonIsDown);
}

ButtonManager::~ButtonManager() {
}
