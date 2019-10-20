/*
  SerialRead.ino - Sample Code for Serial Read without Buffer from LSA08. 
  Copyright (c) 2018 RoboManipal. All Right Reserved.
  File created by : Shashank Goyal
*/

#include <LSA08.h>

#define LSASerial Serial2

LSA08 L;

void setup() 
{
	// Begin Serial Communication for the Debugger.
	Serial.begin(9600);
	// Begin Serial Communication for the LSA.
	LSASerial.begin(38400);
	// Attach Serial Line to the LSA.
	L.AttachSerial(&LSASerial);
	// Enable Debugger for the LSA.
	L.debugger.Initialize("LSA", &Serial);

  	// Uncomment the following to Disable Debugger -
 	// L.debugger.disableDebugger();
}

void loop() 
{
  	// Read LSA Value.  
	L.ReadLSA();
}
