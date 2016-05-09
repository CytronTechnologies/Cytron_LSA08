// ===================================================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description		: Header file for IFC-DI08
//  	Date				: 10 September 2008
//
// ===================================================================================================================

#include <p18f2685.h>

// ===================================================================================================================
// define
// ===================================================================================================================
#define error_led	LATCbits.LATC2

// ===================================================================================================================
// function prototype
// ===================================================================================================================

unsigned short ai_read(unsigned char add,unsigned char an_sel);
unsigned char ai_cmphl(unsigned char add, unsigned char selection, unsigned char highest_lowest);
unsigned char ai_cmp2(unsigned char add, unsigned char selection1, unsigned char selection2);
void ai_sampling_conf(unsigned char add, unsigned char an_sel, unsigned short average);
void ai_bit_conf(unsigned char add, unsigned char bit_num);
unsigned short ai_vref_read(unsigned char add);

// ===================================================================================================================
// Library Functions
// This Library Functions will explain on the function prototype of AI08
// Please make sure the address on IFc-AI08 card is compatible with program.
// ===================================================================================================================


// unsigned short ai_read(unsigned char add,unsigned char an_sel)
// -------------------------------------------------------------------------------------------------------------------
// To read the conversion stalling register which stall the final conversion result.
// add = card address; an_sel= which port want to read (1-8).
// Example:
//		ai_read(ai_add,8);		Input 8 is read. a 8 bits data will be returned
//						if the port is set to 8bits conversion meanwhile
//						a 16bits data will return with the 6 front MSB don't care
//						if the port is set to 10bits digital conversion.
//
// --------------------------------------------------------------------------------------------------------------------

// unsigned char ai_cmphl(unsigned char add, unsigned char selection, unsigned char highest_lowest)
// --------------------------------------------------------------------------------------------------------------------
// To compare the selected analog port to figure out which port has the highest or lowest value conversion.
// add = card address; selection = select which port ready to compare.
// selection is a one byte and can be devided into 8 bits variable and each bit represents every analog port.
// If a particular bit is set to 1 means the representative port is ready to be compare and vice versa.
// highest_lowest = set either to figure out the highest or lowest conversion value port.
// highest_lowest = 1(highest conversion port)
// highest -lowest =0(lowest conversion port)
// Examaple:
//		ai_cmphl(ai_add,0b00001111,1);		Port 1-4 is ready to compare with the highest 
//							conversion value port.
//
// ---------------------------------------------------------------------------------------------------------------------

// unsigned char ai_cmp2(unsigned char add, unsigned char selection1, unsigned char selection2)
// ----------------------------------------------------------------------------------------------------------------------
// To compare 2 analog input ports.
// add = card address, selection1 = compare port 1, selection2 = compare port 2
// Example:
// 		ai_cmp2(ai_add,4,8);			Compare port4 and port8.
//							if selection1 > selection2 = return value 1
//							if selection1 < selection2 = return value 0
//							if selection1 = selection2 = return value 2
//
// -----------------------------------------------------------------------------------------------------------------------

// void ai_sampling_conf(unsigned char add, unsigned char an_sel, unsigned short average)
// -----------------------------------------------------------------------------------------------------------------------
// To configure sampling rate for each port. Every 10ms, the AI08 card will refresh the conversion stalling
// register for every analog port with new conversion result not matter whether the port has connected to
// input device or not.
// add = card address, an_sel = which port is going to configure (1-8)
// average = a value needed to decide the period for the conversion stalling register to refresh with the 
// average value.average is range from 1 - 65,535.
// Example:
//		ai_sampling_conf(ai_add,1,2000);	refresh rate = average x 10ms
//							the conversion stalling register will only refresh
//							a new value at port 1 after 2000*10ms(20 second).
//
// ------------------------------------------------------------------------------------------------------------------------

// void ai_bit_conf(unsigned char add, unsigned char bit_num)
// ------------------------------------------------------------------------------------------------------------------------
// To configure the conversion of each analog inpu port either to 8bit digital value or 10bit digital value.
// add = card address, bit_num = one byte and can be divided into 8 bits variable. Each bit represent every
// analog input port.bit_num = 0(analog value is converted into 8 bit digital value).
// bit_num = 1(analog value is converted into 10bit digital value.
// Example:
// 		ai_bit_conf(ai_add,0b00001111);		AN1,AN2,AN3 and AN4 are set to 10bits analog to digital conversion
//							AN5,AN6,An7 and AN8 are set to 8bits analog to digital conversion
//							The MSB(most significant bit) is represent the AN8.
//							The LSB(least significant bit) is represent the AN1.
//
// -------------------------------------------------------------------------------------------------------------------------

// unsigned short ai_vref_read(unsigned char add)
// -------------------------------------------------------------------------------------------------------------------------
// To know the adjusted	Vref value through programming besides using multimeter.
// The voltage of Vref can be calculated through equation. User can refer IFC-AI08 card Library Functions.
// add = card address
// Example:
// 		ai_vref_read(ai_add);	This function will return a 10bits(0-1023) value 
//					and need a CP04 card to display the value.
//
// -------------------------------------------------------------------------------------------------------------------------
 								
	