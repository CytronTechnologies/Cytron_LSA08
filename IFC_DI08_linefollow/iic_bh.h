//============================================================================================================
//
//	Author			:	Cytron Technologies
//	Project			:	Interface Free Controller
//	Project description	:	Header file for IFC-BH02
//	Date			:	7 January 2009
//
//============================================================================================================
#include<p18f2685.h>

//Define
//===========================================================================================================
#define error_led	LATCbits.LATC2

//Function Prototype
//============================================================================================================ 
void bh_1_stop(unsigned char add);
void bh_1_brake(unsigned char add);
void bh_1_cw(unsigned char add);
void bh_1_ccw(unsigned char add);
void bh_1_speed(unsigned char add,unsigned char pwm);
void bh_1_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_val);
void bh_1_enclr(unsigned char add);
void bh_2_stop(unsigned char add);
void bh_2_brake(unsigned char add);
void bh_2_cw(unsigned char add);
void bh_2_ccw(unsigned char add);
void bh_2_speed(unsigned char add,unsigned char pwm);
void bh_2_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_val);
void bh_2_enclr(unsigned char add);
unsigned short bh_1_enval(unsigned char add);
unsigned char bh_1_runstat(unsigned char add);
unsigned char bh_1_enstat(unsigned char add);
unsigned char bh_1_spval(unsigned char add);
unsigned short bh_2_enval(unsigned char add);
unsigned char bh_2_runstat(unsigned char add);
unsigned char bh_2_enstat(unsigned char add);
unsigned char bh_2_spval(unsigned char add);

// ==============================================================================================================
// Technical Info
// This technical info will explain on the function prototype of bh'x', x = 1 or 2.
// When x=1, the function prototype is for brush motor 1, BH'1' on IFC-BH02
// When x=2, the function prototype is for brush motor 2, BH'2' on IFC-BH02
// Please make sure the address on IFC-BH02 is compatible with program
// And make sure the address each IFC card is different.
// ==============================================================================================================

// void bh_'x'_stop(unsigned char add)
// --------------------------------------------------------------------------------------------------------------
// To stop brush motor, add = card address.
// Communication address = add_bh1
// Example:
//			bh_1_stop(add_bh1);		Stop motor 1 on BH02
//			bh_2_stop(add_bh1);		Stop motor 2 on BH02
//
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_brake(unsigned char add)
// ---------------------------------------------------------------------------------------------------------------
// To brake brush motor immediately, add = card address.
// Communication address = add_bh1
// Example:
//			bh_1_brake(add_bh1);		brake motor 1 on BH02
//   			bh_2_brake(add_bh1);		brake motor 2 on BH02
// 
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_cw(unsigned char add)
// ---------------------------------------------------------------------------------------------------------------
// To change motor direction to clockwise direction, add = card address.
// Communication address = add_bh1
// Example:
//			bh_1_cw(add_bh1);		set motor 1 to clockwise direction
//			bh_2_cw(add_bh1);		set motor 2 to clockwise direction
//
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_ccw(unsigned char add)
// ---------------------------------------------------------------------------------------------------------------
// To change motor direction to counter-clockwise direction, add = card address.
// Communication address = add_bh1
// Example:
//			bh_1_ccw(add_bh1);		set motor 1 to counter-clockwise direction
//			bh_2_ccw(add_bh1);		set motor 2 to counter-clockwise direction
//
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_speed(unsigned char add,unsigned char pwm)
// ---------------------------------------------------------------------------------------------------------------
// To give speed to brush motor. add = card address, pwm = speed value.	
// Maximum speed value is 255.
// Communication address = add_bh1
// Example:
//			bh_1_speed(add_bh1,100);	Set motor 1 on BH02 to the speed of 100
//			bh_2_speed(add_bh1,250);	Set motor 2 on BH02 to the speed of 250
//
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_encon(unsigned char add, unsigned short enc_data, unsigned char action, unsigned char act_val)	
// ---------------------------------------------------------------------------------------------------------------
// To set encoder configuration. add = card address, enc_data = encoder pulses value, 
// action = action for brush motor, act_value = value for action(cw, ccw, speed)
// Communication address = add_bh1
// Example:
//			bh_1_encon(add_bh1,1000,4,100);		change motor 1 direction to CCW with speed 
//								100 when encoder 1 value is 1000.
//			bh_2_encon(add_bh1,500,1,0);		motor 2 stop when encoder 2 value is 500.
//
// ---------------------------------------------------------------------------------------------------------------

// void bh_'x'_enclr(unsigned char add)
// ----------------------------------------------------------------------------------------------------------------
// To clear encoder data. add = card address
// Communication address = add_bh1
// Example:
//			bh_1_enclr(add_bh1);		Clear encoder 1 value on BH02
//			bh_2_enclr(add_bh1);		Clear encoder 2 value on BH02
//
// ----------------------------------------------------------------------------------------------------------------

// unsigned short bh_'x'_enval(unsigned char add)
// ----------------------------------------------------------------------------------------------------------------
// To read encoder value, add = card address.
// Communication address = add_bh1
// Example:
//			bh_1_enval(add_bh1);		Read encoder 1 value on BH02
//			bh_2_enval(add_bh1);		Read encoder 2 value on BH02
//
// ----------------------------------------------------------------------------------------------------------------

// unsigned char bh_'x'_runstat(unsigned char add)
// -----------------------------------------------------------------------------------------------------------------
// To read motor status, add = card address. 
// Return '1' when motor run, Return '0' when motor stop.
// Communication address = add_bh1
// Example:
//			bh_1_runstat(add_bh1);		Read motor 1 status on BH02
//			bh_2_runstat(add_bh1);		Read motor 2 status on BH02
// 
// ----------------------------------------------------------------------------------------------------------------

// unsigned char bh_'x'_enstat(unsigned char add)
// ----------------------------------------------------------------------------------------------------------------
// To read encoder status, add = card address.
// Return '1' when encoder in process, Return '0' when encoder NOT in process.
// Communication address = add_bh1
// Example:
//			bh_1_enstat(add_bh1);		Read encoder 1 status on BH02
//			bh_2_enstat(add_bh1);		Read encoder 2 status on BH02
//
//------------------------------------------------------------------------------------------------------------------
	
// unsigned char bh_1_spval(unsigned char add)
// -----------------------------------------------------------------------------------------------------------------
// To read speed value for brush motor, add = card address
// Communication address = add_bh1
// Example:
//			bh_1_spval(add_bh1);		Read motor 1 speed on BH02
//			bh_2_spval(add_bh1);		Read motor 2 speed on BH02
//
// ------------------------------------------------------------------------------------------------------------------
