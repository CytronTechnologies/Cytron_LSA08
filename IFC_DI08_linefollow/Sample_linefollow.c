//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Sample code for LSA08 line following with IFC-DI08
//	Project description             : PID line following
//      Date				:July 2012
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_bh.h"
#include "iic_cp.h"
#include "iic_di.h"


//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
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

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

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

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_bh 		0b000010	// Please make sure the communication address on 
						// IFC-BH02 is compatible with the address
#define add_di                 0b000001


//macros definition
#define forward()           bh_1_ccw(add_bh); bh_2_cw(add_bh);
#define backward()          bh_1_cw(add_bh); bh_2_ccw(add_bh);
#define rotate_right()      bh_1_ccw(add_bh); bh_2_ccw(add_bh);
#define rotate_left()       bh_1_cw(add_bh); bh_2_cw(add_bh);
#define stop()              bh_1_stop(add_bh); bh_2_stop(add_bh);
#define brake()             bh_1_brake(add_bh); bh_2_brake(add_bh);
#define speed(L,R)          bh_1_speed(add_bh,L);bh_2_speed(add_bh,R);
#define read_sensor()       di_read(add_di,0)
#define motor_speed         100

#define sw(num) cp1_sw(num)

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);

unsigned char Junction(void);
signed char lineposition(void);
void linefollow(signed char linepos);
void PID_UPDATE(signed char position);
//void INIT_PIDTIMER(void);
void PID_MENU(void);

//Global Variables
signed short proportional=0;
static signed short last_proportional=0;
signed short derivative=0;
static signed short integral=0;
signed short m_diff;
float Kp=2,Ki=0.05,Kd=13;
signed short max=255-motor_speed;
static unsigned char mode=0;


//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
    unsigned char junctioncount=0;
    signed char pos;
    signed short leftspeed, rightspeed;


	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write/modify the program here
	//--------------------------------------------------------------------------------

                    // User is advised not to change or remove the initialization function called
	init();     // call initialization function of the setting in program


        delay(1000);        

        stop();

        cp1_blight(100);
        cp1_str(" Line Follow");
        cp1_goto(1,0);
        cp1_str(" LSA08 PORTB");
        while(sw(1)==1);
        while(sw(1)==0); 
        cp1_clr();

        while(sw(1)==1){                 
        PID_MENU();
        }
        cp1_clr();
  
        while(1){

           //get the line position
            pos=lineposition();
            //update PID values
            PID_UPDATE(pos);

            //calculations
            m_diff=(signed short)(Kp*(float)proportional+Ki*(float)integral+Kd*(float)derivative);

            leftspeed=motor_speed+m_diff;
            if(leftspeed>255) leftspeed=255;
            else if(leftspeed<0) leftspeed=0;
            
            rightspeed=motor_speed-m_diff;
            if(rightspeed>255) rightspeed=255;
            else if(rightspeed<0) rightspeed=0;


            forward();
            speed(leftspeed, rightspeed);


            //display
            cp1_goto(0,0);
            cp1_str("pos:");
            if(pos<0){
                pos=-pos;
                cp1_char('-');
            }
            cp1_dec((unsigned char)pos,3);cp1_char(' ');


           if(Junction()==1){   //if junction crossed
                junctioncount++;
                cp1_goto(0,9);
                cp1_str("j:");                               
                cp1_dec(junctioncount,3);
                if(junctioncount>2)
                    break;
            }

        }//endwhile

        //stop line follow after cross junction for 3 times
        brake();
        speed(0,0);
        while(1); 
}//main

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void PID_MENU(void){

   unsigned char kval;   
   
   cp1_goto(0,0);
   cp1_str("Kp   Ki   Kd  ");
   cp1_goto(1,0);
   kval=(unsigned char)Kp;
   cp1_dec(kval,1); cp1_char('.');
   cp1_dec((Kp-(float)kval)*100,2);  cp1_char(' ');

   kval=(unsigned char)Ki;
   cp1_dec(kval,1); cp1_char('.');
   cp1_dec((Ki-(float)kval)*100,2);  cp1_char(' ');

   kval=(unsigned char)Kd;
   cp1_dec(kval,2); cp1_char('.');
   cp1_dec((Kd-(float)kval)*10,1);  cp1_char(' ');


   switch(mode){
       case 0: cp1_goto(0,0); cp1_str("__"); break;
       case 1: cp1_goto(0,5); cp1_str("__"); break;
       case 2: cp1_goto(0,10); cp1_str("__"); break;
   }

   if(sw(2)==0){
       while(sw(2)==0);
       mode++;
       if(mode>2) mode=0;
    }

   if(sw(3)==0){
       while(sw(3)==0);
       switch(mode){
       case 0: Kp-=0.5; if(Kp<0.0) Kp=0; break;
       case 1: Ki-=0.01; if(Ki<0.0) Ki=0; break;
       case 2: Kd-=1; if(Kd<0.0) Kd=0; break;
       }

   }
   if(sw(4)==0){
       while(sw(4)==0);
       switch(mode){
       case 0: Kp+=0.5; if(Kp>10) Kp=10; break;
       case 1: Ki+=0.01; if(Ki>3) Ki=3; break;
       case 2: Kd+=1; if(Kd>50) Kd=50; break;
       }
   }

}

void PID_UPDATE(signed char position){

    static signed short proportional0, proportional1,proportional2;
    if(position==127){      //if no line detected
        proportional=proportional0;
    }
    else{

        proportional=(signed short) position;        //p term
        derivative=proportional-proportional2;       //d term
        integral+=proportional;                      //i term
        if(integral>5000) integral=5000;             //limit the max i term
        else if(integral<-5000) integral=-5000;

        proportional2=proportional1;                 //backup the previous value
        proportional1=proportional0;
        proportional0=proportional;
    }
}

unsigned char Junction(void){
static unsigned char jstatus=0;

    if(read_sensor()==0xff){ //all sensor detect line
        if(read_sensor()==0xff){ //prevent glitches
            jstatus=1;
        }
        return(0);
    }
    else if(jstatus==1) //if previously all sensor detected line
    {
        jstatus=0;
        return(1);
    }
}

signed char lineposition(void){

    unsigned char sensorval;
    unsigned short lineposition=0;
    unsigned char i,j=0;
    unsigned char lineexist=0;

    sensorval=read_sensor();

    for(i=0;i<8;i++){

        if(sensorval&(0b10000000>>i)){
            lineposition=lineposition+i;
            j++;
            lineexist=1;
        }
        else if(lineexist==1){          
            break;
        }
    }
    if(j==0)
        return(127);
    else{
        lineposition=lineposition*10/j;

        return(35-lineposition);
    }

}

void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
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


//=====================================================================================
//	Interrupt vector
//=====================================================================================
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

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

