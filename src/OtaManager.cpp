#include <OtaManager.h>

OtaManager::OtaManager() {
	Serial.println("OtaManager started");
	WiFi.mode(WIFI_STA);
	WiFi.begin(WifiCredentials::ssid, WifiCredentials::password);
	while (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("Connection Failed!");
//		delay(5000);
//		ESP.restart();
		WiFi.mode(WIFI_OFF);
		return;
	}

	connected = true;

	ArduinoOTA.onStart([]() {
		Serial.println("Start updating ");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.begin();
	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void OtaManager::check(){
	ArduinoOTA.handle();
}

OtaManager::~OtaManager() {
}

