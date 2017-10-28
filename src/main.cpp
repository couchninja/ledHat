//#include "Arduino.h"
//
//#include <OtaManager.h>
//#include <LedManager.h>
//
//OtaManager * otaManager;
//LedManager * ledManager;
//
//void setup() {
//  Serial.begin(9600);
//  Serial.println("Booting");
//
//  otaManager = new OtaManager();
//  ledManager = new LedManager();
//
//  pinMode(2, OUTPUT);
//
//}
//
//void loop() {
//  digitalWrite(2, LOW);
//
//	otaManager->check();
//
//	ledManager->step();
//}
