/* 
 * File:   Main.c
 * Author: inghui
 *
 * Created on May 22, 2012, 2:31 PM
 */

//Choose the mode used----------------------------------------------------------
//#define LSA08_USE_ANALOG
#define LSA08_USE_UART
//------------------------------------------------------------------------------
//LSA08 definitions come first here
#define _XTAL_FREQ 20000000
#define BAUDRATE 57600
#define LSA08_UE PORTCbits.RC5
#define LSA08_UE_TRIS TRISCbits.TRISC5
//define the LSA08 address------------------------------------------------------
unsigned char LSA08_ADD= 0x01;
unsigned char ERR_FLAG=0;
//------------------------------------------------------------------------------
//Choose the Uart Mode used-----------------------------------------------------
//#define LSA08_UARTMODE 0
//#define LSA08_UARTMODE 1
#define LSA08_UARTMODE 2
//------------------------------------------------------------------------------
//Analog mode definitions
#define ANALOG_MIDVAL 120


#define	SW1		RB0
#define	SW2		RB1

#define	LED1		RB6
#define	LED2		RB7

#define DIR1            RC0
#define DIR2            RC3

#include <htc.h>
#include "LSA08.h"

void initIO(void);
unsigned char UART_REC(void);
void UART_SEND(char data);
void UART_INIT(void);
void pwm_init(void);
void set_pwmr(unsigned char uc_duty_cycle);
void set_pwml(unsigned char uc_duty_cycle);
void motorspeed(unsigned char uc_left_motor_speed,unsigned char uc_right_motor_speed);
//lcd function proto
void lcdinit(void);
void delay(unsigned long data);
void send_config(unsigned char data);
void send_char(unsigned char data);
void lcd_goto(unsigned char data);
void lcd_clr(void);
void send_string(const char *s);
unsigned char lcd_num(unsigned short number, unsigned char digit);

void PID_UPDATE(signed char position);
void PID_MENU(void);


#define M1CCW() DIR1=1;
#define M1CW() DIR1=0;
#define M2CCW() DIR2=1;
#define M2CW() DIR2=0;
#define stop() motorspeed(0,0);
#define forward() M1CW(); M2CCW();
#define backward() M2CW(); M1CCW();



// If PIC16F887 is being selected as device
__CONFIG(       FOSC_HS  &		// exernal osc
                WDTE_OFF &		// Disable Watchdog Timer.
		PWRTE_OFF  &		// Enable Power Up Timer.
		BOREN_OFF &		// Disable Brown Out Reset.
		MCLRE_ON &		// MCLR function is enabled
		LVP_OFF );		// Disable Low Voltage Programming.




#define motor_speed         200
//Global Variables
signed short proportional=0;
static signed short last_proportional=0;
signed short derivative=0;
static signed short integral=0;
signed short m_diff;
float Kp=5.7,Ki=0.08,Kd=20;
signed short max=255-motor_speed;
static unsigned char mode=0;


void main(void) {

unsigned char junctioncount=0;
signed char pos;
signed short leftspeed, rightspeed;

 //   __delay_ms(500);
    initIO();
    pwm_init();
    forward();
    stop();
    lcdinit();
     __delay_ms(100);
    // init LSA08
    LSA08_Init(); 


    PID_MENU();

    lcd_clr();
    lcd_goto(0);
    send_string("Press SW1");

    __delay_ms(500);
    __delay_ms(500); 

    while(SW1);

    lcd_clr();
    lcd_goto(0);
    send_string("LSA08 ");
    lcd_goto(20);
    send_string("Line Follow");


    while(1){

        //get the line position
        pos=LSA08_GetPosition();

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
        motorspeed((unsigned char)leftspeed, (unsigned char)rightspeed);
            

    }//endwhile

    stop(); 

  
}

//Functions: 

void PID_MENU(void){

   unsigned char kval;
   unsigned long count=0;


   while(1){
       lcd_goto(0);
       send_string("Kp   Ki   Kd  ");
       lcd_goto(20);
       kval=(unsigned char)Kp;
       lcd_num(kval,1); send_char('.');
       lcd_num((Kp-(float)kval)*100,2);  send_char(' ');

       kval=(unsigned char)Ki;
       lcd_num(kval,1); send_char('.');
       if((Ki-(float)kval)*100<10) send_char('0'); 
       lcd_num((Ki-(float)kval)*100,2);  send_char(' ');

  
       kval=(unsigned char)Kd;
       lcd_num(kval,2);
       send_char(' ');
      //send_char('.');
//       lcd_num((Kd-(float)kval)*10,1);  send_char(' ');


       switch(mode){
           case 0: lcd_goto(0); send_string("__"); break;
           case 1: lcd_goto(5); send_string("__"); break;
           case 2: lcd_goto(10); send_string("__"); break;
       }

       if(SW1==0){

           count=0;
           while(SW1==0){

               count++;

               if(count>100000){
                   mode++;
                    if(mode>2) mode=0;
                   break;
               }
           }
           if(SW2==0) break; 
           switch(mode){
               case 0: Kp-=0.2; if(Kp<0.0) Kp=0; break;
               case 1: Ki-=0.01; if(Ki<0.0) Ki=0; break;
               case 2: Kd-=1; if(Kd<0.0) Kd=0; break;
           }

        }
       if(SW2==0){
           while(SW2==0);
           switch(mode){
           case 0: Kp+=0.2; if(Kp>9) Kp=9; break;
           case 1: Ki+=0.01; if(Ki>3) Ki=3; break;
           case 2: Kd+=1; if(Kd>50) Kd=50; break;
           }
       }

       
   }

}


void PID_UPDATE(signed char position){

   static signed short proportional0, proportional1,proportional2;
    if(position==127){      //if no line detected
        proportional=proportional0;
    }
    else{

        proportional=(signed short) position;                      //p term
        derivative=proportional-proportional2;  //d term
        integral+=proportional;                     //i term
        if(integral>2000) integral=2000;           //limit the max i term
        else if(integral<-2000) integral=-2000;
        proportional2=proportional1;  //backup the previous value
        proportional1=proportional0;
        proportional0=proportional;


    }

}

void initIO(void)
{


    //setup digital IO
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // Initialize the I/O port direction.
    TRISA = 0b11111111;
    TRISB = 0b00000011;
    TRISC = 0b10010000;
    TRISD = 0;
    TRISE = 0b00000011;

    ANSEL=0x00;
    ANSELH=0x00;

    LED1=0; LED2=0;

}
void pwm_init(void)
{
	// Setting PWM frequency = 4.90KHz at 8MHz OSC Freq
	PR2 = 254;
	T2CKPS1 = 0;
	T2CKPS0 = 1;	// Timer 2 prescale = 4.

	CCPR1L = 0;		// Duty cycle = 0;
        CCPR2L = 0;
	TMR2ON = 1;		// Turn on Timer 2.

	//configuration for CCP1CON
	P1M1 = 0;		//CCP1, P1A as single output
	P1M0 = 0;

	DC1B1 = 0;		// 2 LSB of 10 PWM, make it 00
	DC1B0 = 0;

	CCP1M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP1M2 = 1;
	CCP1M1 = 0;
	CCP1M0 = 0;

	//configuration for CCP2CON
	CCP2X = 0;		// 2 LSB of 10 PWM, make it 00
	CCP2Y = 0;

	CCP2M3 = 1;		// Configure CCP1 module to operate in PWM mode.
	CCP2M2 = 1;
	CCP2M1 = 0;
	CCP2M0 = 0;  
        
}

void set_pwmr(unsigned char uc_duty_cycle)
{
	CCPR1L = uc_duty_cycle;
}

void set_pwml(unsigned char uc_duty_cycle)
{
	CCPR2L = uc_duty_cycle;
}
void motorspeed(unsigned char uc_left_motor_speed,unsigned char uc_right_motor_speed)
{
	//set the speed for left and right motor
	set_pwmr(uc_right_motor_speed);
	set_pwml(uc_left_motor_speed);
}

//LCD functions----------------------------------------------------------------
#define	rs			RB4	//RS pin of the LCD display
#define	e			RB5	//E pin of the LCD display
#define	lcd_data	PORTD		//LCD 8-bit data PORT

void lcdinit(void){

    TRISD=0x00;
    TRISB4=0;
    TRISB5=0;
        //configure lcd
    send_config(0b00000001);			//clear display at lcd
    send_config(0b00000010);			//lcd return to home
    send_config(0b00000110);			//entry mode-cursor increase 1
    send_config(0b00001100);			//display on, cursor off and cursor blink off
    send_config(0b00111000);			//function set

    //display startup message
    lcd_clr();							//clear lcd

}
void delay(unsigned long data)		//delay function, the delay time
{					//depend on the given value
	for( ;data>0;data--);
}

void send_config(unsigned char data)	//send lcd configuration
{
	rs=0;					//set lcd to configuration mode
	lcd_data=data;				//lcd data port = data
	e=1;					//pulse e to confirm the data
	delay(50);
	e=0;
	delay(50);
}

void send_char(unsigned char data)		//send lcd character
{
 	rs=1;					//set lcd to display mode
	lcd_data=data;				//lcd data port = data
	e=1;					//pulse e to confirm the data
	delay(10);
	e=0;
	delay(10);
}

void lcd_goto(unsigned char data)		//set the location of the lcd cursor
{						//if the given value is (0-15) the
 	if(data<16)                             //cursor will be at the upper line
	{					//if the given value is (20-35) the
	 	send_config(0x80+data);		//cursor will be at the lower line
	}					//location of the lcd cursor(2X16):
	else					// -----------------------------------------------------
	{					// | |00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15| |
	 	data=data-20;			// | |20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35| |
		send_config(0xc0+data);         // -----------------------------------------------------
	}
}

void lcd_clr(void)				//clear the lcd
{
 	send_config(0x01);
	delay(600);
}

void send_string(const char *s)			//send a string to display in the lcd
{
  	while (s && *s)send_char (*s++);
}


unsigned char lcd_num(unsigned short number, unsigned char digit){
    unsigned char num[16];
    unsigned char k=digit-1;
    unsigned char i,j;

    num[digit]=0;

    for(i=0;i<digit;i++){
        num[k-i]=number%10+'0';
        number=number/10;
        if(number==0) break;
    }

    send_string(&num[k-i]);

    for(j=0;j<k-i;j++)
        send_char(' ');

    return(i);
}



/*uart function*/
void UART_INIT(void){

      //set port direction
    TRISCbits.TRISC7=1;     //RX
    TRISCbits.TRISC6=0;     //TX

#ifdef LSA08_UE_TRIS
    LSA08_UE_TRIS=0;
#endif

     // Initialize UART.
    TXSTAbits.BRGH = 1;     // Select high speed baud rate.
    BAUDCTLbits.BRG16=1;    // Baud 16bits

    SPBRG =0x56; SPBRGH=0x00; //57600
    RCSTAbits.SPEN = 1;												// Enable serial port.
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;

}

void UART_SEND(char data)
{
	while(!TRMT) ; //wait for previous transmit completion
	TXREG=data;

}

unsigned char UART_REC(void)			//receive uart value
{
    unsigned long waitcount=0;
    unsigned char rec_data;

    if(RCSTAbits.OERR){
        RCSTAbits.CREN=0;
        RCSTAbits.CREN=1;
        ERR_FLAG=1;
        return(255);
    }
    // Read the received data.
    while (RCIF == 0) //wait for data
    {
        waitcount++;
        if (waitcount > 15000){ //	break if wait too long, no incoming data
            ERR_FLAG=1;
            return (255); //no line
        }
    }

    rec_data = RCREG;

    if (FERR == 1) {
        while(RCIF) rec_data=RCREG;
        ERR_FLAG=1;
        return (255);
    }
    else{
        ERR_FLAG=0;
        return rec_data;					//return the data received
    }
}

void UART_DUMP(void){
    unsigned char dump;

    while (RCIF == 1) //wait for data
    {
        dump=UART_REC();
    }
}