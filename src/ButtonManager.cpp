#include <ButtonManager.h>

ButtonManager::ButtonManager(LedManager * ledManager) {
	Serial.println("Initializing ButtonManager");
	this->ledManager = ledManager;
}

void ButtonManager::checkButton() {
	bool buttonIsDown = !digitalRead(LedSettings::BUTTON_PIN);

	if (this->buttonDownFrames > 2000) {
		this->ledManager->enableSettingsMode();
	}

	if (!buttonIsDown && this->buttonDownFrames > 0) {
		// button went back up
		Serial.println("Button pushed");
		this->ledManager->nextMode();
	} else {

	}

	if (buttonIsDown) {
		this->buttonDownFrames++;
	} else {
		this->buttonDownFrames = 0;
	}

	digitalWrite(BUILTIN_LED, buttonIsDown);
}

ButtonManager::~ButtonManager() {
}
