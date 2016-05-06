//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-DI08
//  Date				: 21 April 2008
//
//=====================================================================================

#include <p18f2685.h>

// =================================================================================
// define
// =================================================================================
#define error_led	LATCbits.LATC2

// =================================================================================
// function prototype
// =================================================================================

unsigned char di_read(unsigned char add,unsigned char selection);
void di_c1con(unsigned char add,unsigned char on_off);
void di_c2con(unsigned char add,unsigned char on_off);
void di_c1clr(unsigned char add);
void di_c2clr(unsigned char add);
unsigned short di_c1val(unsigned char add);	
unsigned short di_c2val(unsigned char add);	

// ========================================================================== 
// Library Functions
// This Library Functions will explain on the function prototype of DI08.
// Please make sure the address on IFC-DI08 is compatible with program.
// ==========================================================================

// unsigned char di_read(unsigned char add,unsigned char selection)
// --------------------------------------------------------------------------
// To read the digital input in byte or per input. 
// add = card address, selection = input selection 
// Example:
//			di_read(0b000010,3);	Read status of Digital Input 3 on DI08 
//									with communication address = 0b000010.
//
//			di_read(0b000011,5);	Read status of Input 5 on DI08
//									with communication address = 0b000011.
//
//			di_read(add_di1,0);		Read status of ALL input on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.
//
// --------------------------------------------------------------------------

// void di_c1con(unsigned char add,unsigned char on_off)
// --------------------------------------------------------------------------
// To set the condition of counter 1 on DI08. 
// DI1 can be set as 16-Bits counter which can count up to 65,535.
// add = card address, on_off = 16-Bits counter condition. 
// on_off = 1, enable DI1 as Counter 1; on_off =0, disable DI1 as Counter 1.
// Example:
//			di_c1con(add_di1,1);	Enable counter1 on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.
//
// 
// --------------------------------------------------------------------------

// void di_c2con(unsigned char add,unsigned char on_off)
// --------------------------------------------------------------------------
// To set the condition of counter 2 on DI08. 
// DI2 can be set as 16-Bits counter which can count up to 65,535.
// add = card address, on_off = 16-Bits counter condition. 
// on_off = 1, enable DI2 as Counter 2; on_off =0, disable DI2 as Counter 2.
// Example:
//			di_c2con(add_di1,1);	Enable counter2 on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.
//
// --------------------------------------------------------------------------

// void di_c1clr(unsigned char add)
// --------------------------------------------------------------------------
// To clear counter1.Value counted at counter1 will be deleted.
// add = card address.
// Example:
//			di_c1clr(add_di1);		clear counter1 value on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.			
//
// --------------------------------------------------------------------------

// void di_c2clr(unsigned char add)
// --------------------------------------------------------------------------
// To clear counter2.Value counted at counter2 will be deleted.
// add = card address.
// Example:
//			di_c2clr(add_di1);		clear counter2 value on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.			
//			
// --------------------------------------------------------------------------

// unsigned short di_c1val(unsigned char add)
// --------------------------------------------------------------------------
// To read value at counter1. add = card address
// Example:
//			di_c1Val(add_di1);		Read value at counter1 on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.			
//
// --------------------------------------------------------------------------
			
// unsigned short di_c2val(unsigned char add)
// --------------------------------------------------------------------------
// To read value at counter2. add = card address
// Example:
//			di_c2Val(add_di1);		Read value at counter2 on DI08
//									with communication address = add_di1.
//									User need to define the address for add_di1 in the
//									program, please refer Sample1_DI for example.			
//
// --------------------------------------------------------------------------
