//=====================================================================================
//
//	Author					: Cytron Technologies		
//	Project					: Interface Free Controller
//	Project description		: Header file for IFC-BL02
//  Date					: 21 April 2008
//						: 01 January 2010 (alarm reset function added)
//
//=====================================================================================
#include<p18f2685.h>

//define
//=====================================================================================
#define error_led	LATCbits.LATC2

//function prototype
//=====================================================================================
void bl_1_cw(unsigned char add);
void bl_2_cw(unsigned char add);
void bl_1_ccw(unsigned char add);
void bl_2_ccw(unsigned char add);
void bl_1_speed(unsigned char add, unsigned char speed);
void bl_2_speed(unsigned char add, unsigned char speed);
void bl_1_stop(unsigned char add);
void bl_2_stop(unsigned char add);
void bl_1_brake(unsigned char add);
void bl_2_brake(unsigned char add);
void bl_1_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2);
void bl_2_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2);
void bl_1_enclr(unsigned char add);
void bl_2_enclr(unsigned char add);
unsigned short bl_1_enval(unsigned char add);	
unsigned short bl_2_enval(unsigned char add);	
unsigned char bl_1_runstat(unsigned char add);	
unsigned char bl_2_runstat(unsigned char add);	
unsigned char bl_1_enstat(unsigned char add);	
unsigned char bl_2_enstat(unsigned char add);	
unsigned char bl_1_spval(unsigned char add);
unsigned char bl_2_spval(unsigned char add);	
void bl_1_alcon(unsigned char add, unsigned char on_off, unsigned char autoreset);
void bl_2_alcon(unsigned char add, unsigned char on_off, unsigned char autoreset);
void bl_1_alrst(unsigned char add);
void bl_2_alrst(unsigned char add);
unsigned char bl_1_alstat(unsigned char add);
unsigned char bl_2_alstat(unsigned char add);



// ======================================================================================
// Technical Info
// This technical info will explain on the function prototype of bl'x', x= 1 or 2.
// When x=1, the function prototype is for Brushless motor 1, BL'1' on IFC-BL02
// When x=2, the function prototype is for Brushless motor 2, BL'2' on IFC-BL02
// Please make sure the address on IFC-BL02 is compatible with program
// And make sure the address each IFC card is different.
// ======================================================================================

// void bl_'x'_cw(unsigned char add)
// --------------------------------------------------------------------------------------
// To change motor direction to clockwise,  add = card address
// communication address = add_bl1
// Example:
//				bl_1_cw(add_bl1);		set BL1 direction to clockwise
//				bl_2_cw(add_bl1);		set BL2 direction to clockwise
//
// --------------------------------------------------------------------------------------

// void bl_'x'_ccw(unsigned char add);  
// --------------------------------------------------------------------------------------
// To change motor direction to counter-clockwise, add = card address
// Communication address = add_bl1
// Example:
// 				bl_1_ccw(add_bl1);		set BL1 direction to counter-clockwise
//     			bl_2_ccw(add_bl1);		set BL2 direction to counter-clockwise
// 
// --------------------------------------------------------------------------------------

// void bl_'x'_speed(unsigned char add, unsigned char speed)
// --------------------------------------------------------------------------------------
// To give speed to brush motor. add = card address, data = speed motor value
// Maximum speed value is 255.
// Communication address = add_bl1
// Example:
//				bl_1_speed(add_bl1,speed_bl1);		set BL1 speed 
//				bl_2_speed(add_bl1,speed_bl2);		set BL2 speed 
//
// --------------------------------------------------------------------------------------

// void bl_'x'_stop(unsigned char add)
// --------------------------------------------------------------------------------------
// To stop brush motor, add = card address
// Communication address = add_bl1
// Example:
//				bl_1_stop(add_bl1);		stop BL1 on BL02
//  			bl_2_stop(add_bl1);		stop BL2 on BL02
//
// --------------------------------------------------------------------------------------

// void bl_'x'_brake(unsigned char add)
// --------------------------------------------------------------------------------------
// To brake brushless motor immediately, add = card address
// Communication address = add_bl1
// Example:
//				bl_1_brake(add_bl1);		brake BL1 immediately on BL02
//				bl_2_brake(add_bl1);		brake BL2 immediately on BL02
//
// ---------------------------------------------------------------------------------------------

// void bl_'x'_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2)
// -----------------------------------------------------------------------------------------------
// TO set encoder configuration, add = card address, enc_data = encoder pulses value, 
// action = action for brushless motor, act_value1 = value for action(cw, ccw, spped) for BL1
// act_value2 = value for action(cw, ccw, speed) for BL2.
// Communication address = add_bl1
// Example:
//				bl_1_encon(add_bl1, 1000, 9, 100,100);			Activate BL2 and change BL1's speed and direction when BL1's encoder value = 1000.			
//				bl_2_encon(add_bl1, 500, 7, 0,0);			Brake BL1 and BL2 when BL2's encoder value = 500.			
//
// -----------------------------------------------------------------------------------------------

// void bl_'x'_enclr(unsigned char add)
// -----------------------------------------------------------------------------------------------
// To clear previous encoder data. add = card address
// Communication address = add_bl1
// Example:
//				bl_1_enclr(add_bl1);		clear BL1 encoder value on BL02
//				bl_2_enclr(add_bl1);		clear BL2 encoder value on BLo2
// 
// -----------------------------------------------------------------------------------------------

// unsigned short bl_'x'_enval(unsigned char add)
// ------------------------------------------------------------------------------------------------	
// To read encoder value, add = card address.
// Communication address = add_bl1
// Examole:
//				cp1_dec(bl_1_enval(add_bl1),5);		Read BL1 encoder value to be displayed
//				cp1_dec(bl_2_enval(add_bl1),5);		Read BL2 encoder value to be displayed
//
// -------------------------------------------------------------------------------------------------

// unsigned char bl_'x'_runstat(unsigned char add)
// -------------------------------------------------------------------------------------------------
// To read motor status, add = card address.
// Return '1' when motor run
// Return '0' when motor stop
// Communication address = add_bl1
// Example:
//				if(bl_1_runstat(add_bl1)==1)		Turn ON led1 if BL1 run 
//				{
//					led1 = 1;
//				}
//
//				if(bl_2_runstat(add_bl1)==0)		Turn OFF led1 if BL2 stop
//				{
//					led1 = 0;
//				}
//
// -------------------------------------------------------------------------------------------------

// unsigned char bl_'x'_enstat(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read encoder status, add = card address
// Return '1' when encoder in process
// Return '0' when encoder NOT in process
// Comunication address = add_bl1
// Example:
//				if(bl_1_enstat(add_bl1)==1)			Turn ON led5 if encoder in process 
//				{						Encoder in process is process when encoder counting the pulse.
//					led5 = 1;
//				}
//
//				if(bl_2_enstat(add_bl1)==0)			Turn OFF led6 if encoder NOT in process
//				{						Encoder NOT in process is when encoder not counting and no pulses value.
//					led6 = 0;
//				}
//
// -----------------------------------------------------------------------------------------------------

// unsigned char bl_'x'_spval(unsigned char add)
// -----------------------------------------------------------------------------------------------------
// To store and read speed value for brushless motor, add = card address
// Communication address = add_bl1
// Example:
//				speed_bl1 = bl_1_spval(add_bl1);		Store BL1's speed value to speed_bl1 
//				speed_bl2 = bl_2_spval(add_bl1);		Store BL2's speed value to speed_bl2
//
// ------------------------------------------------------------------------------------------------------

