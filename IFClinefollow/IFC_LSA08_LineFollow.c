#include<p18f2685.h>
#include "iic.h"
#include "iic_bl.h"
#include "iic_cp.h"
#include "iic_di.h"
#include "iic_ai.h"
//#include "iic_md.h"
//#include "iic_oc.h"
//#include "iic_ai.h"
//#include "iic_ps.h"
//###################################################################################
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//######################################################################################

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00

#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards
//########################################################################################
#define ai_add		0b100011
#define di_add		0b000111
//#define oc_add		0b000110
//#define ps_add		0b010101
//#define md_add		0b000011
#define bl_add		0b000011

//########################################################################################
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
unsigned char cross;
void init(void);
void rotate_right(unsigned char sp1, unsigned char sp2);
void rotate_left(unsigned char sp1, unsigned char sp2);
void move_forward(unsigned char sp1, unsigned char sp2);

//#########################################################################################
void main(void)
{
    unsigned char ADC_value;
    signed char Line_Position=0;
    unsigned char left_right=2;

    init();
    ai_bit_conf(ai_add,0b00000000); //Set Ai card to do 8bit conversion

    

    cp1_goto(0,0);
    cp1_str("Line Follow");
    cp1_goto(1,0);
    cp1_str("Cytron's LSA08");

    while(cp1_sw(1) == 1) continue;

    cp1_clr();


    while(1)
    {
        // get line position from LSA08 through analog mode
        ADC_value=(unsigned char)ai_read(ai_add,1);

        //analog value of LSA08:
        // from 0V (ADC=0) to 4.5V(ADC=229)
        // if no line, 5V
        // if line at middle, 2.25V (ADC=114)

        //offset the value to 0 when line at middle
        //Line_Position = positive when line on right side
        //Line_Position = negative when line on left side
        Line_Position=ADC_value-115;


        if(ADC_value>235){
            if(left_right==1)
                rotate_right(160,160);
            else if(left_right==0)
                rotate_left(160,160);
        }
        else if(Line_Position>100){
            left_right=1;
            rotate_right(160,160);
//            cp1_goto(0,0);
//            cp1_str("Right ");
        }
        else if(Line_Position<-100){
            left_right=0;
            rotate_left(160,160);
//            cp1_goto(0,0);
//            cp1_str("Left  ");
        }
        else if(Line_Position>85){
            move_forward(160,30);
//            cp1_goto(0,0);
//            cp1_str("Right ");
        }
        else if(Line_Position<-85){
            move_forward(30,160);
//            cp1_goto(0,0);
//            cp1_str("Left  ");
        }
        else if (Line_Position>25){
            move_forward(160,60);
//            cp1_goto(0,0);
//            cp1_str("Right ");
        }
        else if(Line_Position<-25){
            move_forward(60,160);
//            cp1_goto(0,0);
//            cp1_str("Left  ");
        }
        else{
            move_forward(150,150);
//            cp1_goto(0,0);
//            cp1_str("Middle");
        }

    }
	
	
}


void move_forward(unsigned char sp1, unsigned char sp2){

  bl_1_cw(bl_add);
  bl_2_ccw(bl_add);
  bl_2_speed(bl_add,sp1);
  bl_1_speed(bl_add,sp2);

}
void rotate_left(unsigned char sp1, unsigned char sp2){

  bl_1_cw(bl_add);
  bl_2_cw(bl_add);
  bl_2_speed(bl_add,sp1);
  bl_1_speed(bl_add,sp2);

}
void rotate_right(unsigned char sp1, unsigned char sp2){
    
  bl_1_ccw(bl_add);
  bl_2_ccw(bl_add);
  bl_2_speed(bl_add,sp1);
  bl_1_speed(bl_add,sp2);

}
//#########################################################################################

void delay(unsigned long data)
{
	
	for( ;data>0;data-=1);
}

//#########################################################################################

void init(void)
{

	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	ADCON1=0x0F;
	iic_init();		
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//#########################################################################################
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//#########################################################################################
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}



