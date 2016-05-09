//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-OC04
//  Date				: 21 April 2008
//
//=====================================================================================

#include<p18f2685.h>

// =================================================================================
// define
// =================================================================================

#define error_led	LATCbits.LATC2

// =================================================================================
// function prototype
// =================================================================================

void oc_out1(unsigned char add,unsigned char data);
void oc_out2(unsigned char add,unsigned char data);
void oc_out3(unsigned char add,unsigned char data);
void oc_out4(unsigned char add,unsigned char data);
void oc_pwm(unsigned char add,unsigned char data);
void oc_out(unsigned char add,unsigned char data);

// =================================================================================
// Library Functions
// This Library Functions will explain on the function prototype of Output Card (IFC-OC04)
// Please make sure the communication address selected on IFC-OC04 is compatible with 
// program.
// =================================================================================


// void oc_out'x'(unsigned char add,unsigned char data)
// ---------------------------------------------------------------------------------
// To defined for output condition of output port: OUT1, OUT2, OUT3 and OUT4.
// add = card address, data = output condition. 
// For output condition,1 is for activate output device and 0 for deactivate output device.
// x = 1, 2, 3, or 4.
// when x=1, the function prototype is for Output port 1, OUT1 on IFC-OC04,
// when x=2, the function prototype is for Output port 2, OUT2 on IFC-OC04,
// when x=3, the function prototype is for Output port 3, OUT3 on IFC-OC04,
// when x=4, the function prototype is for Output port 4, OUT4 on IFC-OC04.
// Example:
//			oc_out1(0b000010,1);     Activate output device connected to OUT1 on 
//									 Output Card with communication address = 0b000010.
//
//			oc_out3(add_oc1,0);      Deactivate output device connected to OUT3 on 
//									 Output Card with communication address = add_oc1.
//									 User need to define the address for add_oc1 in the
//									 program, please refer Sample1_OC for example.
//
// ---------------------------------------------------------------------------------

// void oc_pwm(unsigned char add,unsigned char data)
// ---------------------------------------------------------------------------------
// To use when user connect OUT1 port with an output device that need PWM 
// (Pulse-width modulation) function. 
// add = card address , data = pwm value. 
// The bigger the pwm value, the higher the output voltage, maximum pwm value = 255.
// Example:
//			oc_pwm(0b000110,100);     Activate output device connected to OUT1 with 
//									  PWM value = 100. The communication address for 
//									  the Output Card is 0b000110.
// ---------------------------------------------------------------------------------

// void oc_out(unsigned char add,unsigned char data)
// ---------------------------------------------------------------------------------
// To control all the output port in one time. 
// add = card address, data = all output value.
// (0000 DCBA) [A=output1, B=output2, C=output3, D=output4, active high]
// Example:
// 			oc_out(0b100010, 0b00001111); 	 Activate ALL Output Port on Output Card 
//											 with communication address = 0b100010.
//
//			oc_out (add_oc1, 0b00000000);	 Deactivate ALL Output Port on Output Card 
//											 with communication address = add_oc1.
//									 		 User need to define the address for add_oc1 in the
//									 		 program, please refer Sample1_OC for example.
// ---------------------------------------------------------------------------------
