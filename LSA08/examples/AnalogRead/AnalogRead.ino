/*
	AnalogRead.ino - Sample Code for Analog Read from LSA08. 
	Copyright (c) 2018 RoboManipal. All Right Reserved.
	File created by : Shashank Goyal
*/

/****************************************************************

Note: This Code is supposed to be used with 10 Bit Analog Pins.

****************************************************************/

#include <LSA08.h>

LSA08 L;

void setup() 
{
	// Begin Serial Communication for the Debugger.
	Serial.begin(9600);
	// Attach Analog Pin to the LSA. 
	L.AttachAnalogPin(A0);
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
