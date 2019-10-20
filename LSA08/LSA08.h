/*
  LSA08.h - Library to deal with Cytron Line Sensing Module: LSA08.
  Copyright (c) 2018 RoboManipal. All right reserved
  File created by : Shashank Goyal
*/

#ifndef LSA08_h
#define LSA08_h

#include "Arduino.h"
#include "DebuggerSerial.h"

#define MODE_SERIAL 0
#define MODE_ANALOG 1

#define BUFFER_OFF 0
#define BUFFER_ON 1

class LSA08
{
	// #################### Private Members ######################
	// ---------------------- Data Members -----------------------
	// Serial Line to which the LSA is Connected.
	HardwareSerial *LSASerial;
	// Mode of Receiving Values: Analog or Serial.
	int LSAMode;
	// Current Value of the LSA.
	int currentValue;
	// Last Non - 255 value of the LSA.
	int previousValue;
	// Analog Pin to which the LSA is Connected.
	int analogPin;
	// Flag value indicating Status of LSA.
	int flagValue;
	// Whether LSA is Connected on a Buffer.
	bool bufferStatus;
	// If Connected, then Control Pin for the Buffer.
	int bufferPin;
	// Active State for the Buffer Pin.
	bool bufferPinStatus;

	// --------------------- Member Functions --------------------
	// Clears the Junk Vales from the LSA.
	void ClearLSA();
	// Sets FLag Value from the Current Value of the LSA.
	void SetFlag();

public:

	// ################### Public Member Functions ################
	// DubuggerSerial Object to Print Debugger Messages.
	DebuggerSerial debugger;
	// -------------------- Constructors --------------------------
	// Empty Constructor
	LSA08();
	// Serial Mode without Buffer.
	LSA08(HardwareSerial* LSASerial);
	// Serial Mode with Buffer Control.
	LSA08(HardwareSerial* LSASerial,int bufferPin,bool bufferPinStatus);
	// Analog Mode
	LSA08(int analogPin);
	// Check for Flag Values with Equality
	bool operator == (int flagValue);
	// Check for Flag Values with Inequality
	bool operator != (int flagValue);
	// Return the LSA Value as RHS Value.
	operator int ();
	// Assign Buffer Control Values.
	void SetBuffer(int bufferPin,bool bufferPinStatus);
	// Attach Serial without Buffer.
	void AttachSerial(HardwareSerial* LSASerial);
	// Attach Serial with Buffer Control.
	void AttachSerial(HardwareSerial* LSASerial, int bufferPin, bool bufferPinStatus);
	// Attach Analog Pin.
	void AttachAnalogPin(int analogPin);
	// Returns the LSA Reading.
	int ReadLSA();
	// Returns the Last Non - 255 Value of the LSA.
	int GetPreviousValue();
};

#endif