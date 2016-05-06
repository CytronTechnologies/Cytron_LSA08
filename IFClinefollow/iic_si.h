//=====================================================================================
//
//	Author					: Cytron Technologies		
//	Project					: Interface Free Controller
//	Project description		: Header file for IFC-SI02
//  Date					: Nov 2009
//
//=====================================================================================
#include<p18f2685.h>

//define
//=====================================================================================
#define error_led	LATCbits.LATC2

//function prototype
//=====================================================================================
void si_1_stop(unsigned char add);
void si_2_stop(unsigned char add);
void si_1_brake(unsigned char add);
void si_2_brake(unsigned char add);
void si_1_cw(unsigned char add);
void si_2_cw(unsigned char add);
void si_1_ccw(unsigned char add);
void si_2_ccw(unsigned char add);
void si_1_cson(unsigned char add);
void si_2_cson(unsigned char add);
void si_1_csoff(unsigned char add);
void si_2_csoff(unsigned char add);
void si_1_acdoff(unsigned char add);
void si_2_acdoff(unsigned char add);
void si_1_acdon( unsigned char add);
void si_2_acdon( unsigned char add);
void si_1_instspeed(unsigned char add,unsigned char speed);				//reserved
void si_2_instspeed(unsigned char add,unsigned char speed);				//reserved
void si_1_setacc(unsigned char add, unsigned char acc);
void si_2_setacc(unsigned char add, unsigned char acc);
void si_1_speed(unsigned char add, unsigned char speed);
void si_2_speed(unsigned char add, unsigned char speed);
void si_maxfreq(unsigned char add, unsigned char maxfreq);
void si_1_enclr(unsigned char add);
void si_2_enclr(unsigned char add);
void si_1_encon(unsigned char add, unsigned long enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2);
void si_2_encon(unsigned char add, unsigned long enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2);
void si_1_econclr(unsigned char add);
void si_2_econclr(unsigned char add);
unsigned long si_1_enval(unsigned char add);
unsigned long si_2_enval(unsigned char add);
unsigned char si_1_runstat(unsigned char add);
unsigned char si_2_runstat(unsigned char add);
unsigned char si_1_spval(unsigned char add);
unsigned char si_2_spval(unsigned char add);
unsigned char si_1_OH(unsigned char add);
unsigned char si_2_OH(unsigned char add);
unsigned char si_1_dirstat(unsigned char add);
unsigned char si_2_dirstat(unsigned char add);


// ======================================================================================
// Technical Info
// This technical info will explain on the function prototype of SI'x', x= 1 or 2.
// When x=1, the function prototype is for Brushless motor 1, SI'1' on IFC-SI02
// When x=2, the function prototype is for Brushless motor 2, SI'2' on IFC-SI02
// Please make sure the address on IFC-SI02 is compatible with program
// And make sure the address each IFC card is different.
// ======================================================================================


// void si_'x'_stop(unsigned char add)
// --------------------------------------------------------------------------------------
// To off and release the stepper motor of each channel.
// Communication address = add_si1
// Example:
//			si_1_stop(add_si1);		//release channel 1 motor
//			si_2_stop(add_si1);		//release channel 2 motor
//
// -------------------------------------------------------------------------------------

// void si_'x'_brake(unsigned char add)
// -------------------------------------------------------------------------------------
// To enable the stepper motor; the motor will in brake condition. 
// Use ‘si_’x’_brake’ function compared to ‘si_’x’_stop’ function if user wants to stop 
// stepper motor immediately.
// Communication address = add_si1
// Example:
//  		si_1_brake(add_si1);	//brake channel 1 motor
//          	si_2_brake(add_si1);	//brake channel 2 motor
// ------------------------------------------------------------------------------------- 
   
// void si_'x'_cw(unsigned char add)
// -------------------------------------------------------------------------------------
// To set the direction of the motor. 
// Motor will change the direction to clockwise direction (cw). 
// User is advised to decelerate the motor before change the direction to avoid step-slipping. 
// Communication address = add_si1
// Example:
//			si_1_cw(add_si1)		//change direction of channel 1 to clockwise
//          si_2_cw(add_si1)		// change direction of channel 2 to clockwise
//
// ------------------------------------------------------------------------------------

// void si_'x'_ccw(unsigned char add)
// ------------------------------------------------------------------------------------
// To set the direction of the motor to counter clockwise (ccw). 
// User is advised to decelerate the motor before change the direction to avoid step-slipping.  
// Communication address = add_si1
// Example:
//          si_1_ccw(add_si1);		//change direction of channel 1 to counter clockwise
//          si_2_ccw(add_si1);		// change direction of channel 2 to counter clockwise
//
// ------------------------------------------------------------------------------------

// void si_'x'_cson(unsigned char add)
// void si_'x'_csoff(unsigned char add)
// ------------------------------------------------------------------------------------
// To set the microstepping (if available) of the stepper driver card. 
// si_’x’_cson will set the CS pin high while si_’x’_csoff will set the CS pin low. 
// Communication address = add_si1
// Example:
//			si_1_cson (add_si1);	//on channel 1 microstepping 
//			si_2_csoff (add_si1);	//off channel 2 microstepping
//
// ------------------------------------------------------------------------------------

// void si_'x'_acdoff(unsigned char add)
// void si_'x'_acdon( unsigned char add)
// -------------------------------------------------------------------------------------
// To set the automatic current cut back (if available) of the stepper driver.
// si_’x’_acdon will set the automatic current cut back ON, 
// while si_’x’_acdoff will set OFF the automatic current cut back release.
// Communication address = add_si1
// Example:
//          si_1 _acdon (add_si1);	//on channel 1 automatic current cut back 
//  		si_2_acdoff (add_si1);	//off channel 2 automatic current cut back
//
// -----------------------------------------------------------------------------------

// void si_'x'_setacc(unsigned char add, unsigned char acc)
// -----------------------------------------------------------------------------------
// To set the acceleration value of each channel. 
// The value of acceleration, ‘acc’ is 1-255. 
// Communication address = add_si1
// Example:
//			si_1 _setacc(add_si1, 255);		//set channel 1 acceleration value to be 255 (maximum). 
//			si_2_setacc(add_si1, 50);		//set channel 2 acceleration value to be 50.
//
// ------------------------------------------------------------------------------------

// void si_'x'_speed(unsigned char add, unsigned char speed)
// ------------------------------------------------------------------------------------
// To set the channels to start accelerate or decelerate at the acceleration value set 
// by si_’x’_setacc function to a desired speed value (speed). 
// Communication address = add_si1
// Example:
//			si_1 _ speed (add_si1, 255);	//set channel 1 accelerate to the speed of 255. 
//			si_2_ speed (add_si1,100);	//set channel 2 accelerate or decelerate to speed of 100.
//
// ------------------------------------------------------------------------------------

// void si_maxfreq(unsigned char add, unsigned char maxfreq)
// ------------------------------------------------------------------------------------
// To set the maximum frequency, maxfreq of both channels.
// This maximum frequency is also representing the pulse frequency of the maximum speed in each channel. 
// The frequency range is from 25-200 (25 is 2.5kHz and 200 is 20kHz). 
// The maximum frequency will be defaulted to 5kHz if user do not set it. 
// User is advised to set the maximum frequency at the beginning of the program.
// Communication address = add_si1
// Example:
//			si_maxfreq(add_si1, 50);	//the maximum frequency is set to 5kHz. 
//
// -----------------------------------------------------------------------------------

// void si_'x'_enclr(unsigned char add)
// -----------------------------------------------------------------------------------
// To clear the value for previous pulses counted by encoder. 
// Communication address = add_si1
// Example:
// 			si_1_enclr(add_si1);		//clear channel 1 encoder
//			si_2_enclr(add_si1);		//clear channel 2 encoder
//
// ----------------------------------------------------------------------------------

  
// void si_'x'_encon(unsigned char add, unsigned long enc_data, unsigned char action, unsigned char act_value1, unsigned char act_value2);
// -----------------------------------------------------------------------------------
// To set the motor’s action based on the encoder’s value. 
// ‘enc_data’ is the value to compare with the pulses from encoder which can count up to 16777215. 
// ‘action’ is what the stepper motor will do, which are : 
// 			none (0) 
//			release (1)
//			brake (2) 
//			cw (3)
//		 	ccw (4) 
//			speed (5) 
//			SI1SI2 release(6) 
//			SI1SI2 brake(7)
//			SI1SI2 cw(8) 
//			SI1SI2 ccw(9)
//			SI1cw SI2ccw(10) 
//			SI1ccw SI2cw(11).
// The ‘action’ will be executed after pulses from encoder reach the value set in ‘enc_data’. 
// ‘act_value1’and act_value2 are the speed of SI1 and SI2 respectively when execute the ‘action’. 
// ‘act_value’ for ‘action’ cw(3), ccw(4), speed(5), SI1SI2cw(8), SI1SI2 ccw(9), 
// BL1cw BL2ccw(10) or BL1ccw BL2cw(11) is at the range of 0 to 255 while ‘act_value’ for ‘action’ none(0),
// stop(1), brake(2), SI1SI2 stop(6), SI1SI2 brake(7) is 0.  
// Communication address = add_si1
// Example:
//			si_1_encon(add_si1,80000, 5, 1,0);	
//			si_2_encon(add_si1,196990, 2, 1,0);
//
// -------------------------------------------------------------------------------------

// void si_'x'_econclr(unsigned char add)
// -------------------------------------------------------------------------------------
// To clear the previously called encoder functions. 
// All encoder functions which were called by si_’x’_encon( ) will be cleared at once.
// Communication address = add_si1
// Example:
// 			si_1_enconclr(add_si1);		// clear channel 1 encoder functionS.
//			si_2_enconclr(add_si1);		// clear channel 2 encoder functionS
//
// ------------------------------------------------------------------------------------

// unsigned long si_'x'_enval(unsigned char add)
// ------------------------------------------------------------------------------------
// To read the encoder value stored each channel. 
// The encoder can count up to 16777215 pulses. 
// Communication address = add_si1
// Example:
//			si_1_enval(add_si1);	//retrieve channel 1 encoder value
//			si_2_enval(add_si1);	//retrieve channel 2 encoder value
//
// ------------------------------------------------------------------------------------

// unsigned char si_'x'_runstat(unsigned char add)
// ------------------------------------------------------------------------------------
// To read motor status which will return ‘1’ if channel is set to run
// return ‘0’ if channel is set to stop or brake in the program. 
// This function is useful when user wants to check the motor’s status. 
// Communication address = add_si1
// Example:
//			if (si_1_runstatl(add_si1)==1) 			//Turn on led1 if motor1 run
//			{
//     				led1=1;
//			}	
//			if (si_2_runstatl(add_si1)==0) 			//Turn off led1 if motor2 stop
//			{
//				led1=0;
//			}
//
// -----------------------------------------------------------------------------------

// unsigned char si_'x'_spval(unsigned char add)
// -----------------------------------------------------------------------------------
// To read the speed value. Maximum value for speed is 255 while minimum is 0.
// User can call this function to display speed value on IFC-CP04’s LCD.
// Communication address = add_si1
// Example:
//			cp1_dec(si_1_spval(add_si1), 3);  		//display channel 1 speed on cp1
//			cp1_dec(si_2_spval(add_si1), 3);			//display channel 2 speed on cp1
//
// ------------------------------------------------------------------------------------

// unsigned char si_'x'_OH(unsigned char add)
// ------------------------------------------------------------------------------------
// To read the overheat signal (if available) from the stepper indexer card.
// Communication address = add_si1
// Example:
//			si_1_OH(add_si1);	//channel 1 overheat signal
//			si_2_OH(add_si1);	//channel 2 overheat signal
//
// ------------------------------------------------------------------------------------

// unsigned char si_'x'_dirstat(unsigned char add)
// ------------------------------------------------------------------------------------
// To read the direction of the motor of each channel respectively.
// Communication address = add_si1
// Example:
//			si_1_dirstat(add_si1);	//channel 1 direction
//			si_2_dirstatadd_si1);	//channel 2 direction
//
// -----------------------------------------------------------------------------------





