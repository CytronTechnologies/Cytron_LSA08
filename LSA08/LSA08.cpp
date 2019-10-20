/*
  LSA08.cpp - Library to deal with with Cytron Line Sensing Module: LSA08.
  Copyright (c) 2018 RoboManipal. All right reserved
  File created by : Shashank Goyal
*/

#include "LSA08.h"

// ##################### Private Member Functions ###############
// Clears the Junk Values Recived from the LSA.
void LSA08 :: ClearLSA()
{
	while (LSASerial->available())
	{
		char junk = LSASerial->read();
	}
}

// Assigns a Flag Value according to the Value of the LSA.
void LSA08 :: SetFlag()
{
	if(this->currentValue == 0)
		this->flagValue = 0;

	else if(this->currentValue > 0 && this->currentValue < 35)
		this->flagValue = 1;

	else if(this->currentValue == 35)
		this->flagValue = 2;

	else if(this->currentValue > 35 && this->currentValue < 70)
		this->flagValue = 3;

	else if(this->currentValue == 70)
		this->flagValue = 4;

	else if(this->currentValue >= 255)
		this->flagValue = 5;

	else
		this->flagValue = -1;	
}

// #################### Public Member Functions ###################
// ------------------------ Constructors --------------------------

// Default - Empty Constructor.
LSA08 :: LSA08()
{
	// Set Null Value for the Serial if Not Specified.
	this->LSASerial = NULL;
	// Assume Serial is NOT on any Buffer. 
	this->bufferStatus = BUFFER_OFF;
	// Mode Not Specified.
	this->LSAMode = -1;
	// Default Values for the LSA Parameters.
	this->flagValue = -1;
	this->currentValue=-1;
	this->previousValue=-1;
}

// Serial without Buffer Control
LSA08 :: LSA08(HardwareSerial* LSASerial) : LSA08()
{
	//Set Serial for the LSA.
	AttachSerial(LSASerial);
}

// Serial with Buffer Control.
LSA08 :: LSA08(HardwareSerial* LSASerial,int bufferPin,bool bufferPinStatus) : LSA08()
{
	// Set Serial and Buffer Controls for the LSA.
	AttachSerial(LSASerial,bufferPin,bufferPinStatus);
}

// Analog Mode for LSA.
LSA08 :: LSA08(int analogPin) : LSA08()
{
	// Set the Analog Pin for the LSA.
	AttachAnalogPin(analogPin);
}

// ------------------- Overloaded Constructors --------------------
// Equality Check
bool LSA08 :: operator == (int flagValue)
{
	return (this->flagValue == flagValue);
}

// Inequality Check
bool LSA08 :: operator != (int flagValue)
{
	return (this->flagValue != flagValue);
}

// Assignment Operator for Reading LSA Value to the LSA Object.
LSA08 :: operator int()
{
	return this->ReadLSA();
}

// Assign Buffer Variables for Serial.
void LSA08 :: SetBuffer(int bufferPin,bool bufferPinStatus)
{
	// LSA is Connected to Buffer.
	this->bufferStatus = BUFFER_ON;
	// Set the Select Line for Buffer Control.
	this->bufferPin = bufferPin;
	// Set the Active State for the Buffer.
	this->bufferPinStatus = bufferPinStatus;
	// Assign Pin as OUTPUT for the ARDUINO.
	pinMode(this->bufferPin,OUTPUT);
	// By Default, Do not Receive any Values.
	digitalWrite(this->bufferPin,~this->bufferPinStatus);
}

// Assign Serial to the LSA without Buffer .
void LSA08 :: AttachSerial(HardwareSerial *LSASerial)
{
	// Set Serial.
	this->LSASerial = LSASerial;
	// Set Mode as Serial.
	this->LSAMode = MODE_SERIAL;
	// Set Buffer Status as OFF.
	this->bufferStatus = BUFFER_OFF;	

	String msg = "LSA Serial initialized ";
	msg.concat(" LSA Mode: ");
	msg.concat(LSAMode);
	debugger.print(msg,INFO);
}

// Assign Serial to the LSA with Buffer Control.
void LSA08 :: AttachSerial(HardwareSerial *LSASerial,int bufferPin,bool bufferPinStatus)
{
	// Call Overloaded Implemtation.
	this->AttachSerial(LSASerial);
	String msg = "LSA Buffer State: ON";
	// Set Buffer Status as ON along with Control Parameters.
	this->SetBuffer(bufferPin,bufferPinStatus);
	msg.concat("Buffer Pin: ");
	msg.concat(bufferPin);
	msg.concat("Active State: ");
	msg.concat(bufferPinStatus);
	debugger.print(msg,INFO);
}

// Assign Analog Pin for Analog Mode.
void LSA08 :: AttachAnalogPin(int analogPin)
{
	// Set NULL as Analog Mode is being Used.
	this->LSASerial = NULL;
	// Set Mode as Analog.
	this->LSAMode = MODE_ANALOG;
	// Assign Analog Pin.
	this->analogPin = analogPin;
	// Set Buffer Status as OFF for Analog Mode.
	this->bufferStatus = BUFFER_OFF;
	// Set Analog Pin as INPUT type.
	pinMode(analogPin,INPUT);

	String msg = "LSA Set on Analog Pin: ";
	msg.concat(analogPin);
	debugger.print(msg,INFO);
}

// Recieve Analog values.
int LSA08 :: ReadLSA()
{
	// ======================= Serial Mode ========================
	if(this->LSAMode == MODE_SERIAL)
	{
		// If Buffer Mode is Enabled.
		if(this->bufferStatus)
		{
			// Activate the Respective Select Pin.
			digitalWrite(this->bufferPin,this->bufferPinStatus);
		}	
		// Clear LSA for Junk Values.
		this->ClearLSA();
		// Wait for LSA to send the Data.
		while (!(LSASerial->available()));
		// Receive the Data.
		while (LSASerial->available())
		{
			// If the Current Value was not 255 Change the Previous Value.
			if (this->currentValue != 255)
				this->previousValue = this->currentValue;
			// Read the LSA Value.
			this->currentValue = LSASerial->read();
		}
		this->ClearLSA();
		// If Buffer Mode is Enabled.
		if(this->bufferStatus)
		{	
			// Deactivate the Respective Select Pin.
			digitalWrite(this->bufferPin,~(this->bufferPinStatus));
		}	
	}

	// ======================== Analog Mode =======================
	else if(LSAMode == MODE_ANALOG)
	{
		// If the Current Value was not 255 Change the Previous Value.
		if(this->currentValue != 255)
			this->previousValue = this->currentValue;

		// Read Raw Analog value.
		int raw_analog_value = analogRead(this->analogPin);

		// Mapping the Value from 1023 (10 Bit) to 0-70 Convention.
		if (raw_analog_value <= 920)
			this->currentValue = map(raw_analog_value, 0, 920, 0, 70);
		else
			this->currentValue = 255;

		// Round off to nearest Multiple of 5.
		int temp = currentValue%5;
		if(temp!=0)
		{
			this->currentValue-=temp;
			if(temp>2)
				this->currentValue+=5;
		}
	}

	// ERROR: Unknown Mode 
	else
	{
		debugger.print("LSA mode not Set.",ERROR);
		return -1;
	}

	// Set the Flag Value according to the Current Value.
	SetFlag();

	// #################### Debugger Output #################
	// Previous Value: <Previous Value> Current Value: <Current Value> Flag Value: <Flag Value>
	String msg = "Previous Value: ";
	msg.concat(this->previousValue);
	msg.concat(" Current Value: ");
	msg.concat(this->currentValue);		
	msg.concat(" Flag Value: ");
	msg.concat(this->flagValue);
	debugger.print(DEBUG,msg);
	// Return Current Value.
	return this->currentValue;
}

// Returns the Last Non - 255 Value from the LSA.
int LSA08 :: GetPreviousValue()
{
	return this->previousValue;
}