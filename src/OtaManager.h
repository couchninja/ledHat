
#ifndef SRC_OTAMANAGER_H_
#define SRC_OTAMANAGER_H_

#define OTA_DISABLED 0
#define OTA_CONNECTED 1
#define OTA_DISCONNECTED 2
//#define FASTLED_ALLOW_INTERRUPTS 0

#include "Arduino.h"
#include "settings/WifiCredentials.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


class OtaManager {
public:
	int state = OTA_DISABLED;
	OtaManager(bool connect);
	void check();
	virtual ~OtaManager();
};

#endif /* SRC_OTAMANAGER_H_ */
