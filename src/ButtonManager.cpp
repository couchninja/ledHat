#include <ButtonManager.h>

ButtonManager::ButtonManager(LedManager * ledManager) {
	Serial.println("Initializing ButtonManager");
	this->ledManager = ledManager;
}

void ButtonManager::checkButton(){
	bool buttonIsDown = !digitalRead(LedSettings::BUTTON_PIN);
	if(!buttonIsDown && this->buttonWasDown){
		Serial.println("Button pushed");
		this->ledManager->nextMode();
	}
	this->buttonWasDown = buttonIsDown;

	digitalWrite(BUILTIN_LED, buttonIsDown);
}

ButtonManager::~ButtonManager() {
}
