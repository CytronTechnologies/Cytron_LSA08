//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-MD15A
//  Date				: 21 April 2008
//
//=====================================================================================

#include<p18f2685.h>

// =====================================================================================================
// define
// =====================================================================================================

#define error_led	LATCbits.LATC2

// =====================================================================================================
// function prototype
// =====================================================================================================

void md_stop(unsigned char add);
void md_brake(unsigned char add);
void md_cw(unsigned char add);
void md_ccw(unsigned char add);
void md_speed(unsigned char add,unsigned char data);
void md_encon(unsigned char add,unsigned short enc_data, unsigned char action, unsigned char act_val);
void md_enclr(unsigned char add);
void md_alcon(unsigned char add, unsigned char autoreset);
void md_alrst(unsigned char add);
unsigned short md_enval(unsigned char add);
unsigned char md_runstat(unsigned char add);
unsigned char md_enstat(unsigned char add);
unsigned char md_spval(unsigned char add);
unsigned char md_alstat(unsigned char add);

// ====================================================================================================
// Technical Info
// this technical info will explain on the function prototype of MD15A
// Please make sure the address on IFC-MD15A card is compatible with program.
// ====================================================================================================

// void md_stop(unsigned char add)
// ----------------------------------------------------------------------------------------------------
// To stop brush motor, add = card address.
// Example:
//				md_stop(add_md1);		Stop motor on MD15A with communication 
//										address = add_md1.User need to define the 
//										address for add_md1 in the program, 
//										please refer Sample1_MD for example.
//
// ----------------------------------------------------------------------------------------------------

// void md_brake(unsigned char add)
// ----------------------------------------------------------------------------------------------------
// To brake brush motor immediately, add = card address.
// Example:
//				md_brake(add_md1);		Brake motor on MD15A with communication 
//										address = add_md1.User need to define the 
//										address for add_md1 in the program, 
//										please refer Sample1_MD for example.
//
// ----------------------------------------------------------------------------------------------------

// void md_cw(unsigned char add)
// ----------------------------------------------------------------------------------------------------
// To change motor direction to clockwise direction, add = card address.
// Example:
// 				md_cw(add_md1);			Set motor on MD15A to the direction of 
//										clockwise where communication address = add_md1.
//
// ----------------------------------------------------------------------------------------------------

// void md_ccw(unsigned char add)
// ----------------------------------------------------------------------------------------------------
// To change motor direction to counter-clockwise direction, add = card address.
// Example:
//				md_ccw(add_md1);		Set motor on MD15A to the direction of 
//										counter-clockwise where communication 
//										address = add_md1.
//										
// ----------------------------------------------------------------------------------------------------

// void md_speed(unsigned char add,unsigned char data)
// ----------------------------------------------------------------------------------------------------
// To give speed to brush motor. add = card address, data = speed value.
// Maximum speed value is 255.
// Example:
//				md_speed(add_md1, 100);	Set motor on MD15A to the speed of 100
//										where communication address = add_md1.
//
// ---------------------------------------------------------------------------------------------------

// void md_encon(unsigned char add,unsigned short enc_data, unsigned char action, unsigned char act_val)
// ---------------------------------------------------------------------------------------------------
// To set encoder configuration. add = card address, enc_data = encoder pulses value, 
// action = action for brush motor, act_value = value for action(cw, ccw, speed)
// Example:
//				md_encon(add_md1, 30000, 3, 100);	When Encoder value = 30000,
//													change motor's direction to 
//													clockwise with speed = 100.
//													where communication address = add_md1.
//												
// ---------------------------------------------------------------------------------------------------

// void md_enclr(unsigned char add)
// ---------------------------------------------------------------------------------------------------
// To clear encoder data. add = card address
// Example:
//				md_enclr(add_md1);		Clear encoder's value on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// void md_alcon(unsigned char add, unsigned char autoreset)
// --------------------------------------------------------------------------------------------------
// To set alaram configuration, add = card address, autoreset = '1' alarm autoreset,
// autoreset = '0' alarm no autoreset.
// Example:
//				md_alcon(add_md1, 1);	alarm autoreset on MD15A where 
//										communication address = add_md1.
//				md_alcon(add_md1, 0);	alarm no autoreset on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// void md_alrst(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To set alarm to manually reset, add = card address.
// User need call this function to reset alram if alarm autoreset was disabled.
// Example:
//				md_alrst(add_md1);       Manually reset alarm on MD15A where 
//										 communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// unsigned short md_enval(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read encoder value, add = card address.
// Example:
// 				md_enval(add_md1);		Read encoder value on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// unsigned char md_runstat(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read motor status, add = card address. 
// Return '1' when motor run, Return '0' when motor stop.
// Example:
// 				md_runstat(add_md1);	Read motor's status on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// unsigned char md_enstat(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read encoder status, add = card address.
// Return '1' when encoder in process, Return '0' when encoder NOT in process.
// Example:
//				md_enstat(add_md1);		Read encoder's status on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// unsigned char md_spval(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read speed value for brush motor, add = card address
// Example:
//				md_spval(add_md1);		Read motor's speed on MD15A where 
//										communication address = add_md1.
//
// --------------------------------------------------------------------------------------------------

// unsigned char md_alstat(unsigned char add)
// --------------------------------------------------------------------------------------------------
// To read alarm status at brush motor, add = card address.
// Return '1' when has alarm notification, Return '0' when NO alarm notification.
// Example:
//				md_alstat(add_md1);		Read motor's alarm status on MD15A where 
//										communication address = add_md1.
// --------------------------------------------------------------------------------------------------

