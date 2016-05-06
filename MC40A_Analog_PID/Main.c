/* 
 * File:   Main.c
 * Author: inghui
 *
 * Created on May 25, 2012, 12:23 PM
 */

//Choose the mode used----------------------------------------------------------
#define LSA08_USE_ANALOG
//#define LSA08_USE_UART
//------------------------------------------------------------------------------
//LSA08 definitions come first here
#define _XTAL_FREQ 20000000
//#define BAUDRATE 57600
//#define LSA08_UE PORTCbits.RC5
//#define LSA08_UE_TRIS TRISCbits.TRISC5
#define LSA08_JPULSE PORTCbits.RC0
//define the LSA08 address------------------------------------------------------
unsigned char LSA08_ADD= 0x01;
unsigned char ERR_FLAG=0;
//------------------------------------------------------------------------------
//Choose the Uart Mode used-----------------------------------------------------
//#define LSA08_UARTMODE 0
//#define LSA08_UARTMODE 1
//#define LSA08_UARTMODE 2
//------------------------------------------------------------------------------
//Analog mode definitions
#define ANALOG_MIDVAL 115


// I/O Connections.
// Parallel 2x16 Character LCD
#define LCD_E			RE2		// E clock pin is connected to RB5
#define LCD_RS			RB6		// RS pin is used for LCD to differentiate data is command or character
#define	LCD_DATA		PORTD	// Data port of LCD is connected to PORTD, 4 bit mode

// LED on MC40A
#define LED1			RB7

// Push button on MC40A
#define SW1				RB0
#define SW2				RB1

// Digital limit switch
#define LIMIT1			RA1		//limit switch 1
#define LIMIT2			RA2		//limit switch 2

// Buzzer
#define BUZZER			RB7

// L293B, H-Bridge IC to drive either DC brush or Stepper Motor
#define ML_EN			RC1		// this pin is connected to Enable of L293 H-bridge driver, it is being use for speed control, for Motor left
#define MR_EN			RC2		// this pin is connected to Enable of L293 H-bridge driver, it is being use for speed control, for Motor right

// L293 pin for DC Brushed Motor
#define MR_1			RB2		//right motor pin 1
#define MR_2			RB3		//right motor pin 2
#define ML_1			RB4		//left motor pin 1
#define ML_2			RB5		//left motor pin 2


//Includes
#include <htc.h>
#include "LSA08.h"


//function prototypes
//LSA08 library use
void ADC_INIT(void);
unsigned short GET_ADC(void);
//initialize IO
void initIO(void);
//motor functions
void pwm_init(void);
void set_pwmr(unsigned char uc_duty_cycle);
void set_pwml(unsigned char uc_duty_cycle);
void motor(unsigned char uc_left_motor_speed,unsigned char uc_right_motor_speed);
//lcd functions
void send_lcd_data(unsigned char b_rs, unsigned char uc_data);
void set_lcd_e(unsigned char b_output);
void set_lcd_rs(unsigned char b_output);
void set_lcd_data(unsigned char uc_data);
void lcd_init(void);
void lcd_clr(void);
void lcd_home(void);
void lcd_2ndline(void);
void lcd_goto(unsigned char uc_position);
void lcd_putchar(char c_data);
void lcd_putstr(const char* csz_string);
unsigned char lcd_num(unsigned short number,unsigned char digit);

void PID_UPDATE(signed char position);
void PID_MENU(void);


__CONFIG(       FOSC_HS  &		// exernal osc
                WDTE_OFF &		// Disable Watchdog Timer.
		PWRTE_OFF  &		// Enable Power Up Timer.
		BOREN_OFF &		// Disable Brown Out Reset.
		MCLRE_ON &		// MCLR function is enabled
		LVP_OFF );		// Disable Low Voltage Programming.

#define motor_speed         180
//Global Variables
signed short proportional=0;
static signed short last_proportional=0;
signed short derivative=0;
static signed short integral=0;
signed short m_diff;
float Kp=2.5,Ki=0.004,Kd=15;
signed short max=255-motor_speed;
static unsigned char mode=0;

void main (void){

unsigned char junctioncount=0;
signed char pos;
signed short leftspeed, rightspeed;

    initIO();
    pwm_init();
    motor(0,0); 
    lcd_init();
    LSA08_Init();
    __delay_ms(100);


/* This code portion can be used to determine ANALOG_MIDVAL
ANALOG_MIDVAL is the ADC value when LSA08 LCD shows position of 35
the value is typically around 115 (2.25 volt) unless there is voltage loss in 
the connection, the value 115 is 8bits unsinged char, if the resolution of ADC
is 10bits, shift right 2 bits to get 8bits value.
2.25V=4.5/2, where 4.5volt is when line at the rightmost position (70)
2.25V is equivalent to (2.25/5)*255=115 */
////------------------------------------------------------------------------------
//    unsigned char adcval;
//    while(1){
//
//        adcval=(unsigned char) (GET_ADC()>>2);
//        lcd_goto(0);
//        lcd_num((unsigned short)adcval,3);
//        __delay_ms(300);
//    }
//
////-----------------------------------------------------------------------------
    PID_MENU();

    lcd_clr();
    lcd_goto(0);
    lcd_putstr("Press\nSW1");
    while(SW1); //wait for switch 1 being pressed


//    lcd_clr();
//    lcd_goto(0);
//    lcd_putstr("LSA08");
//    lcd_goto(20);
//    lcd_putstr("Demo");

    while(1){

        pos=LSA08_GetPosition();    //range return by analog >100
        if(pos!=127){
            pos/=(signed char)3;    //divide by 3 for smaller range
        }

        PID_UPDATE(pos);

        //calculations
        m_diff=(signed short)(Kp*(float)proportional+Ki*(float)integral+Kd*(float)derivative);

        leftspeed=motor_speed+m_diff;
        if(leftspeed>255) leftspeed=255;
        else if(leftspeed<0) leftspeed=0;

        rightspeed=motor_speed-m_diff;
        if(rightspeed>255) rightspeed=255;
        else if(rightspeed<0) rightspeed=0;

        motor((unsigned char)leftspeed, (unsigned char)rightspeed);


        lcd_goto(0);
        if(pos<0){ pos=-pos; lcd_putchar('-');}
        lcd_num((unsigned short)pos,3);
        lcd_putchar(' ');


       

    }





}

void PID_MENU(void){
    unsigned char mode=0;
    unsigned long count=0;
    unsigned char kval;

    while(1){

        if(mode==3)
            break; 
        out: 
        if(SW1==0){
            count=0;
            while(SW1==0){
                count++;

                if(count>100000){                    
                    mode++;
                    if(mode>3) mode=0;
                    lcd_clr();

                    while(SW1==0);
                    goto out;
                   }
            }                       
            switch (mode){
            case 0: Kp-=0.1;
                    if(Kp<0) Kp=0;
                    break;
            case 1: Ki-=0.001;
                    if(Ki<0) Ki=0;
                    break;
            case 2: Kd-=5;
                    if(Kd<0) Kd=0;
                    break;
            }

        }


        if(SW2==0){

            while(SW2==0);
          
            switch (mode){
            case 0: Kp+=0.1;
                    if(Kp>9) Kp=9;
                    break;
            case 1: Ki+=0.001;
                    if(Ki>1) Ki=1;
                    break;
            case 2: Kd+=5;
                    if(Kd>200) Kd=200;
                    break;
            }
        }


        lcd_goto(0);
        switch(mode){

            case 0: lcd_putstr("Kp\n");
                    kval=(unsigned char)Kp;
                    lcd_num(kval,1); lcd_putchar('.');
                    lcd_num((Kp-(float)kval)*10,1);  lcd_putchar(' ');
                    break;
            case 1: lcd_putstr("Ki\n");lcd_goto(20);
                    kval=(unsigned char)Ki;
                    lcd_num(kval,1); lcd_putchar('.');
                    if((Ki-(float)kval)*1000<100) lcd_putchar('0');
                    if((Ki-(float)kval)*1000<10) lcd_putchar('0');
                    lcd_num((Ki-(float)kval)*1000,3);  lcd_putchar(' ');
                    break;
            case 2:lcd_putstr("Kd\n");lcd_goto(20);
                   kval=(unsigned char)Kd;
                   lcd_num(kval,3);
                   lcd_putchar(' ');
                    break;
        }
        
    }



}
void PID_UPDATE(signed char position){

    static signed short proportional0, proportional1,proportional2;

    if(position==127){
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
void initIO(void){

    // clear port value
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	// Initialize the I/O port direction.
	TRISA = 0b11111111;
	TRISB = 0b00000011;
	TRISC = 0b10011001;
	TRISD = 0;
	TRISE = 0b00000011;

        ANSEL=0x01;
        ANSELH=0x00;
        LED1=0;

}

/*motor functions*/
void pwm_init(void)
{
	// Setting PWM frequency
	PR2 = 254;  //setting of PWM frequency for a FOSC of 20MHZ
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

        //motor right forward
	MR_1 = 1;
	MR_2 = 0;
	//motor left forward
	ML_1 = 0;
	ML_2 = 1;
}

void set_pwmr(unsigned char uc_duty_cycle)
{
	CCPR1L = uc_duty_cycle;
}
void set_pwml(unsigned char uc_duty_cycle)
{
	CCPR2L = uc_duty_cycle;
}
void motor(unsigned char uc_left_motor_speed,unsigned char uc_right_motor_speed)
{
	//set the speed for left and right motor
	set_pwmr(uc_right_motor_speed);
	set_pwml(uc_left_motor_speed);
}

/* LCD functions */
void lcd_init(void)
{
	// Set the LCD E pin and wait for the LCD to be ready before we
	// start sending data to it.
	set_lcd_e(1);
	__delay_ms(15);

	// Configure the Function Set of the LCD.
	// Because of the LCD is initialized as 8-bit mode during start up, we need
	// to send the data in 8-bit mode to configure the LCD.
	send_lcd_data(0, 0b00111000);

	// Configure the entry mode set of the LCD.
	send_lcd_data(0, 0b00000110);

	// Configure the display on/off control of the LCD.
	send_lcd_data(0, 0b00001100);

	// Clear the LCD display.
	lcd_clr();
}

void lcd_clr(void)
{
	// Send the command to clear the LCD display.
	send_lcd_data(0, 0b00000001);
}
void lcd_home(void)
{
	// Send the command to return the cursor to the home position.
	send_lcd_data(0, 0b00000010);
}
void lcd_2ndline(void)
{
	// Send the command to jump to the second row.
	send_lcd_data(0, 0b11000000);
}
void lcd_goto(unsigned char uc_position)
{        
        if(uc_position<16)                             //cursor will be at the upper line
	{					//if the given value is (20-35) the
	 	send_lcd_data(0,0x80+uc_position);		//cursor will be at the lower line
	}					//location of the lcd cursor(2X16):
	else					// -----------------------------------------------------
	{					// | |00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15| |
	 	uc_position=uc_position-20;			// | |20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35| |
		send_lcd_data(0,0xc0+uc_position);
        }
}

void lcd_putchar(char c_data)
{
	// Send the data to display.
	send_lcd_data(1, (unsigned char)c_data);
}

void lcd_putstr(const char* csz_string)
{
	// Loop until the end of string.
	while (*csz_string != '\0') {

		// Jump to the second row if '\n' or '\r' is found.
		if (*csz_string == '\n' || *csz_string == '\r') {
			lcd_2ndline();
		}

		// Else, display the character.
		else {
			lcd_putchar(*csz_string);
		}

		// Point to next character.
		csz_string++;
	}
}
void send_lcd_data(unsigned char b_rs, unsigned char uc_data)
{
		// 8-bit Mode.
		// We only need to send the data once.
		set_lcd_rs(b_rs);
		set_lcd_data(uc_data);

		// Send a negative e pulse.
		set_lcd_e(0);
		__delay_ms(2);
		set_lcd_e(1);
		__delay_ms(1);
}

void set_lcd_e(unsigned char b_output)
{
	LCD_E = b_output;
}
void set_lcd_rs(unsigned char b_output)
{
	LCD_RS = b_output;
}
void set_lcd_data(unsigned char uc_data)
{
	LCD_DATA = uc_data;
}

unsigned char lcd_num(unsigned short number,unsigned char digit){

    unsigned char num[16];
    unsigned char k=digit-1;
    unsigned char i,j;

    num[digit]=0;

    for(i=0;i<digit;i++){
        num[k-i]=number%10+'0';
        number=number/10;
        if(number==0) break;
    }

    lcd_putstr(&num[k-i]);

    for(j=0;j<k-i;j++)
        lcd_putchar(' ');

    return(i);
    
}


void ADC_INIT(void){

    ADCON0bits.ADCS=0b10;   //FOSC/32
    ADCON0bits.CHS=0;       //channel 0

    ANSELbits.ANS0=1;       //mux

    ADCON1bits.ADFM=1;          //right justified
   
    TRISA0=1;                   //set TRISA0 input

    ADCON0bits.ADON=1;

    __delay_ms(5);
}

unsigned short GET_ADC(void){

    unsigned short adc;
    __delay_us(30);    // Delay 30us to fully charge the holding capacitor in the ADC module.
    // Start the conversion and wait for it to complete.
    ADCON0bits.GO_DONE = 1;
    while (ADCON0bits.GO_DONE == 1);

    // Return the ADC result.
    adc=ADRESH<<8;
    adc|=ADRESL;
    return (adc);
}