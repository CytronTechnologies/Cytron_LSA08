// =================================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-RS08
//  Date				: 20 March  2009
//
// =================================================================================================

#include<p18f2685.h>

// =================================================================================================
// Define
// =================================================================================================
#define error_led	LATCbits.LATC2

// =================================================================================================
// Function prototype
// =================================================================================================
void rs_en(unsigned char add,unsigned char servo_channel, unsigned char enable);
void rs_pos_sp(unsigned char add, unsigned char servo_channel, unsigned int pos,unsigned char ramp);
unsigned int rs_read_pos(unsigned char add, unsigned char request_servo_channel);

// =================================================================================================
// Library Functions
// This Library functions will explain on the function prototype of IFC-RS08
// Please make sure the address on IFC-RS08 card is compatible with program.
// Also make sure the address for each IFC card is different.
// =================================================================================================

//----------------------------------------------------------------------------------------------------
// void rs_en(unsigned char add,unsigned char servo_channel, unsigned char enable)
// ----------------------------------------------------------------------------------------------------
// To enable or disable RC servo's channel
// add = card address
// servo_channel =  To determine which RC servo channel used
//					value 0 is to select entire channels
//					value more than 8 will select channel 8
// enable = value 1 is to enable selected channel
//			value 0 is to disable selected channel
// Example:
//			rs_pos_sp(add_rs,8,1);		// enable channel 8, RC servo at channel 8 will hold at its position
//			rs_pos_sp(add_rs,5,0);		// disable channel 5, RC servo at channel 5 will become loose
//
// ------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
// void rs_pos_sp(unsigned char add, unsigned char servo_channel, unsigned int pos,unsigned char ramp)
// ----------------------------------------------------------------------------------------------------
// To determine channel, position and speed of RC servo used.
// add = card address.
// Communication address = add_rs
// servo_channel =  To determine RC servo channel used.
//					value 0 is to select entire channels
//					value more than 8 will select channel 8
// pos = RC servo position, 0 step to 5000 step (0.5ms to 2.5 ms pulse)
//		 Initial position for IFC-RS08 is 2500 step 
// ramp = Speed rate for RC servo go to the position request by user.The rate is from 1 - 255.
// 		  The higher value, the faster servo will move to its position.
// 		  '1' indicates that the servo will run at slowest speed.
// 		  '255' will run at the fastest speed.
// 		  '0' is special case which the servo will run at it normal speed. This speed normally faster than any of the speed rate.
// Example:
//			rs_pos_sp(add_rs, 8, 4500, 20);	//Rc servo channel 8 move to 4500 steps position with speed rate = 20
//			rs_pos_sp(add_rs, 7, 100, 0);	//RC servo channel 7 move to 100 steps position as fast as possible
// ------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------
// unsigned int rs_read_pos(unsigned char add, unsigned char request_servo_channel)
// ------------------------------------------------------------------------------------------------------
// To read the current servo position from RC servo channels.
// add = card address.
// request_servo_channel = servo channels request by user
// Example:
//			position= rs_read_pos(add_rs,8);			//Read position of RC servo channel 8
//														//The return value is 0-5000 and store in variable position
//											
// ------------------------------------------------------------------------------------------------------
