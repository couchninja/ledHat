
#ifndef SRC_OTAMANAGER_H_
#define SRC_OTAMANAGER_H_

#include "Arduino.h"
#include "settings/WifiCredentials.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

class OtaManager {
public:
	OtaManager();
	void check();
	void updateStarting();
	virtual ~OtaManager();
};

#endif /* SRC_OTAMANAGER_H_ */
