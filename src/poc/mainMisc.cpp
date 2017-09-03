//#include "Arduino.h"
//
//#ifndef UNIT_TEST  // IMPORTANT LINE!
//
//// constants won't change. They're used here to set pin numbers:
//const int analogueInPin = A0;
//const int ledPin =  4;
//const int onboardLedPin =  2;
//
//int buttonState = 0;
//
//void setup() {
//  // initialize the LED pin as an output:
////  pinMode(ledPin, OUTPUT);
//  pinMode(onboardLedPin, OUTPUT);
//  // initialize the pushbutton pin as an input:
//  pinMode(analogueInPin, INPUT);
//}
//
//bool ledState = false;
//
//void loop() {
//
//  // read the state of the pushbutton value:
////  buttonState = digitalRead(analogueInPin);
//
//	int readValue = analogRead(analogueInPin);
//
//  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
////  if (buttonState == HIGH) {
////    // turn LED on:
////    digitalWrite(ledPin, HIGH);
////  } else {
////    // turn LED off:
////    digitalWrite(ledPin, LOW);
////  }
//
////    digitalWrite(ledPin, !buttonState);
////    digitalWrite(onboardLedPin,!(readValue > 200));
//
//	ledState = !ledState;
//	digitalWrite(onboardLedPin, ledState);
//	delay(max(100, readValue));
//}
//
//#endif    // IMPORTANT LINE!
