//==========================================================================
//
//	Author			: Cytron Technologies
//	Project			: Interface Free Controller
//	Project description	: Header file for IFC-PS01
//	Date			: 9 September 2008
//
//===========================================================================================================

#include<p18f2685.h>

//============================================================================================================
//define
//============================================================================================================

#define error_led	LATCbits.LATC2
#define p_select	0
#define p_joyl		1
#define p_joyr		2
#define p_start		3
#define p_up		4
#define p_right		5
#define p_down		6
#define p_left		7

#define p_l2		8
#define p_r2		9
#define p_l1		10
#define p_r1		11
#define p_triangle	12
#define p_circle	13
#define p_cross		14
#define p_square	15

#define joy_lx		0
#define joy_ly		1
#define joy_rx		2
#define joy_ry		3

#define joy_lu		4
#define joy_ld		5
#define joy_ll		6		
#define joy_lr		7
#define joy_ru		8
#define joy_rd		9
#define joy_rl		10
#define joy_rr		11

//=============================================================================================================
//function prototype
//=============================================================================================================

unsigned char ps_sw(unsigned char add, unsigned char button_number);
unsigned char ps_joy(unsigned char add, unsigned char joy_stick);
void ps_vibrate(unsigned char add, unsigned char motor, unsigned char value);
unsigned char ps_stat(unsigned char add);

// ============================================================================================================
// Library Functions
// this Library Functions will explain on the function prototype of Ps01
// Please make sure the address on IFC-PS01 card is compatible with program.
// ============================================================================================================


// unsigned char ps_sw(unsigned char add, unsigned char button_number)
// ------------------------------------------------------------------------------------------------------------
// To read digital value of PS Controller button
// add = card address, button_number = button number/name for PS controller as define above
// Examples:
//			if(ps_sw(ps_add,13)==0)cp1_str("circle");		LCD will display "circle" 
//										when circle button is pressed.
//			if(ps_sw(ps_add,p_circle)==0)cp1_str("circle");		LCD will display "circle" 
//										when circle button is pressed.
// -------------------------------------------------------------------------------------------------------------


// unsigned char ps_joy(unsigned char add, unsigned char joy_stick)
// -------------------------------------------------------------------------------------------------------------
// To read analog joystick value of PS controller
// add = card address,joy_stick = joystick axis as define above
// Example:
//			cp1(ps_joy(ps_add,2),3);		LCD will display return value of joystick axis 
//								when joy_rx is pressed.
// 			cp1_dec(ps_joy(ps_add,joy_rx),3);	LCD will display return value of joystick axis
//								when joy_rx is pressed.
// 
// --------------------------------------------------------------------------------------------------------------


// void ps_vibrate(unsigned char add, unsigned char motor, unsigned char value)
// --------------------------------------------------------------------------------------------------------------
// To control the vibration motor of PS controller.
// add = card address, motor = which motor to control(1 or 2), value = 1 or 0 for motor	1,
// value = 0 - 255 for motor 2.
// Example:
// 			if(cp1_sw(2)==0)ps_vibrate(ps_add,2,100);	motor2 vibrate with value = 100
//			if(cp1_sw(2)==0)ps_vibrate(ps_add,1,1);		motor1 ON(vibrate) 
//
//
// --------------------------------------------------------------------------------------------------------------


// unsigned char ps_stat(unsigned char add)
// --------------------------------------------------------------------------------------------------------------
// To get connection status of PS Controller.
// add = card address
// Example:
// 			if(!ps_stat(ps_add))cp1_str("no con");		LCD display "no con" if no PS Controller
//									connect to IFC-PS01 card.
//
//
// --------------------------------------------------------------------------------------------------------------


