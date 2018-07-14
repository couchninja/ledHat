#ifndef SRC_BUTTONMANAGER_H_
#define SRC_BUTTONMANAGER_H_

#include<LedManager.h>
#include<settings/ledSettings.h>

class ButtonManager {
public:
	LedManager * ledManager;
	bool buttonWasDown = false;

	ButtonManager(LedManager * ledManager);
	void checkButton();

	virtual ~ButtonManager();
};

#endif /* SRC_BUTTONMANAGER_H_ */
