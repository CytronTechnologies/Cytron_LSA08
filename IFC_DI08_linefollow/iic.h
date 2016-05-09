//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-MB00
//  Date				: 21 April 2008
//
//=====================================================================================

#include<p18f2685.h>

//define
//==========================================================================
#define busy_led	LATCbits.LATC1

//function prototype
//==========================================================================

void iic_init(void);
unsigned char iic_start_write(unsigned char add);
unsigned char iic_start_read(unsigned char add);
unsigned char iic_send(unsigned char data);
void iic_stop(void);
unsigned char iic_rec_ack (void);
unsigned char iic_rec_notack (void);
