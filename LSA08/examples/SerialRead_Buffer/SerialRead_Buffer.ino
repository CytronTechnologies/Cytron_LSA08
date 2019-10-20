/*
  SerialRead_Buffer.ino - Sample Code for Serial Read with Buffer from LSA08. 
  Copyright (c) 2018 RoboManipal. All Right Reserved.
  File created by : Shashank Goyal
*/

/****************************************************************************

Note: The LSA(s) work on Active LOW by Default. 
	  Set the Active Level as LOW only if using External Buffer Control.

****************************************************************************/      

#include <LSA08.h>

#define LSASerial Serial2

LSA08 L1,L2;

void setup() 
{
  	// Begin Serial Communication for the Debugger.
	Serial.begin(9600);
  	// Begin Serial Communication for the LSA.
	LSASerial.begin(38400);

  	// Attach Serial Line to the LSA 1.
	L1.AttachSerial(&LSASerial);
  	// Attach Buffer Control Pin and set Active State.
	L1.SetBuffer(5,LOW);
  	// Enable Debugger for the LSA 1.
	L1.debugger.Initialize("LSA 1", &Serial);

  	// Attach Serial Line to the LSA 2.
	L2.AttachSerial(&LSASerial);
  	// Attach Buffer Control Pin and set Active State.
	L2.SetBuffer(6,LOW);
  	// Enable Debugger for the LSA 2.
	L2.debugger.Initialize("LSA 2", &Serial);

  	// Uncomment the following to Disable Debuggers -

  	// L1.debugger.disableDebugger();
  	// L2.debugger.disableDebugger();

}

void loop() 
{
	// Read LSA 1 Value.
	L1.ReadLSA();
	// Read LSA 2 Value.
	L2.ReadLSA();
}
