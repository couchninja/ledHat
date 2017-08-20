#include <classtest.h>
#include "Arduino.h"

#ifndef UNIT_TEST  // IMPORTANT LINE!

ClassTest * test;

void setup() {
	test = new ClassTest();

	// initialize LED digital pin as an output.
	pinMode(2, OUTPUT);
}

void loop() {
	test->test = ! test->test;

	digitalWrite(2, test->test);
	delay(1000);
}

#endif    // IMPORTANT LINE!

