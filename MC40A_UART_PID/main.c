/*
 * File:   Main.c
 * Author: inghui
 *
 * Created on May 25, 2012, 12:23 PM
 */

//Choose the mode used----------------------------------------------------------
//#define LSA08_USE_ANALOG
#define LSA08_USE_UART
//------------------------------------------------------------------------------
//LSA08 definitions come first here
#define _XTAL_FREQ 20000000
#define BAUDRATE 57600
#define LSA08_UE PORTCbits.RC0
#define LSA08_UE_TRIS TRISCbits.TRISC0
//#define LSA08_JPULSE PORTCbits.RC0
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
//#define ANALOG_MIDVAL 115


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

#define RotateLeft()   MR_1 = 1; MR_2 = 0; ML_1 = 1; ML_2 = 0;
#define RotateRight() MR_1 = 0; MR_2 = 1;	ML_1 = 0; ML_2 = 1;
#define Forward() MR_1 = 1; MR_2 = 0; ML_1 = 0;	ML_2 = 1;
#define Brake() MR_1=1; MR_2=1; ML_1=1; ML_2=1; motor(255,255); 

//Includes
#include <htc.h>
#include "LSA08.h"


//function prototypes
//LSA08 library use
unsigned char UART_REC(void);
void UART_SEND(char data);
void UART_INIT(void);
//initialize IO
void initIO(void);
void timer_init(void);
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
void Calibrate(void);
void line_follow(void);
void junction_follow(void);
void Line_Follow(void);

void interrupt tmr0_isr(void);


__CONFIG(       FOSC_HS  &		// exernal osc
                WDTE_OFF &		// Disable Watchdog Timer.
		PWRTE_OFF  &		// Enable Power Up Timer.
		BOREN_OFF &		// Disable Brown Out Reset.
		MCLRE_ON &		// MCLR function is enabled
		LVP_OFF );		// Disable Low Voltage Programming.


//Global Variables
signed short motor_speed=     190;
signed short proportional=0;
static signed short last_proportional=0;
signed short derivative=0;
static signed short integral=0;
signed short m_diff;
float Kp=5.0,Ki=0.005,Kd=40.0;
//signed short max=255-motor_speed;
static unsigned char mode=0;
signed char pos;
unsigned char junction_mode=0;



void main (void){


    initIO();
    pwm_init();
    motor(0,0);
    lcd_init();
    LSA08_Init();
    __delay_ms(100);

    lcd_clr();
    lcd_goto(0);
    lcd_putstr("LSA08");
    lcd_goto(20);
    lcd_putstr("Demo");

    
    while(SW1);
    while(SW1==0);
    lcd_clr();
   
    PID_MENU();

    lcd_clr();
    lcd_goto(0);   
    lcd_putstr("line\nfollow");

    if (junction_mode){

          junction_follow();
    }

    else {
    
        while(1){
           line_follow();
        }
    }





}


void jfolo(unsigned char count){

    unsigned char junction_count=0;
    unsigned char timer=0;

    Forward();      // set motor move forward


    // sending display to lcd takes long time
    // junction reading and display less frequent.
    // use timer variable to count 20, then read once.
    while(junction_count<count){   //second junction
        timer++;
        if(timer>50){
            timer=0;        //clear timer
            motor(0,0);     //slow down for lcd display           
            do{
                junction_count=LSA08_GetJunction(); //check junction count
            }while(ERR_FLAG||(junction_count>10));   //checking no uart error

            lcd_goto(0);
            lcd_putchar('J'); 
            lcd_num(junction_count,3);
        }

        line_follow(); //PID line follow
    }
    Brake();
    
    __delay_ms(200);   
 

}
void junction_follow(void){
    signed char lineposition=0;
    unsigned char i=0;
    unsigned char status=0;
    
    lcd_clr();
    lcd_goto(0);
    lcd_putstr("Junction\nCount");

    __delay_ms(500);
    lcd_clr(); 

    while(1){

        do{
            status=LSA08_ClearJunction();
        }while(status==0);
       
        jfolo(2);

        RotateRight();
        motor(180,180);
        __delay_ms(500);
         
        do{
            lineposition=LSA08_GetPosition();
            LED1=1;
        }while(lineposition>20);       
        Brake();
        LED1=0;
        __delay_ms(100);

        RotateRight();
        motor(180,180);
        __delay_ms(500);
        
        do{
            LED1=1;
            lineposition=LSA08_GetPosition();
        }while(lineposition>20);        
        Brake();
        LED1=0;
        __delay_ms(100);
        

    }
}

void line_follow(void){
    signed short leftspeed, rightspeed;

    pos=LSA08_GetPosition();
    PID_UPDATE(pos);

    //calculations
    m_diff=(signed short) ((Kp*(float)proportional)+(Ki*(float)integral)+(Kd*(float)derivative));

    leftspeed=motor_speed+m_diff;
    if(leftspeed>255) leftspeed=255;
    else if(leftspeed<0) leftspeed=0;

    rightspeed=motor_speed-m_diff;
    if(rightspeed>255) rightspeed=255;
    else if(rightspeed<0) rightspeed=0;

    motor((unsigned char)leftspeed, (unsigned char)rightspeed);
}
void Calibrate(void){

     LSA08_Calibrate();

    RotateRight(); motor(180,180);
    __delay_ms(600);

    RotateLeft();
    __delay_ms(600);__delay_ms(600);

    RotateRight(); motor(180,180);
    __delay_ms(600);

    motor(0,0);Forward();
    lcd_clr();

}
void PID_MENU(void){
    unsigned char mode=0;
    unsigned long count=0;
    unsigned char kval;

    while(1){

        if(SW1==0){
            while(SW1==0);  //wait for switch release
            mode++;
            if (mode>2) mode=0;

        }
        else if(SW2==0 ){
            while(SW2==0);  //wait for switch release

            switch (mode){
                case 0:   return;                   //start
                case 1:   junction_mode^=1;  break; //mode change line follow/ junction count
                case 2:   Calibrate();       break; //calibrate LSA08
            }
        }
        //LCD display
        lcd_goto(0);
        switch (mode){

            case 0: lcd_putstr("Start\n");
                    lcd_putstr("PressSW2");
                    break;
            case 1: lcd_putstr("Mode \n");
                    if(junction_mode) lcd_putstr("Junction");
                    else            lcd_putstr("Line Flw");
                    break;
            case 2: lcd_putstr("Calb \n");
                    lcd_putstr("PressSW2");
                    break;
        }

    }
        
//         out:
//        if(SW1==0){
//            count=0;
//            while(SW1==0){
//                count++;
//
//                if(count>60000){
//                    mode++;
//                    if(mode>5) mode=0;
//                    lcd_clr();
//
//                    while(SW1==0);
//                    goto out;
//                   }
//            }
//
//            switch (mode){
//            case 0: Kp-=0.1;
//                    if(Kp<0) Kp=0;
//                    break;
//            case 1: Ki-=0.001;
//                    if(Ki<0) Ki=0;
//                    break;
//            case 2: Kd-=1.0;
//                    if(Kd<0.0) Kd=0.0;
//                    break;
//
//            }
//
//        }
//
//
//        if(SW2==0){
//
//            while(SW2==0);
//
//            switch (mode){
//            case 0: Kp+=0.1;
//                    if(Kp>9) Kp=9;
//                    break;
//            case 1: Ki+=0.001;
//                    if(Ki>5) Ki=5;
//                    break;
//            case 2: Kd+=1.0;
//                    if(Kd>50.0) Kd=50.0;
//                    break;
//            case 3: return;
//            case 5: Calibrate(); break;
//            case 4: junction_mode^=1;  break;
//
//            }
//        }
//
//
//        lcd_goto(0);
//        switch(mode){
//
//            case 0: lcd_putstr("Kp  \n");
//                    kval=(unsigned char)Kp;
//                    lcd_num(kval,1); lcd_putchar('.');
//                    lcd_num((Kp-(float)kval)*10,1);
//                    lcd_putchar(' ');
//                    break;
//            case 1: lcd_putstr("Ki  \n");lcd_goto(20);
//                    kval=(unsigned char)Ki;
//                    lcd_num(kval,1); lcd_putchar('.');
//                    if((Ki-(float)kval)*1000<100) lcd_putchar('0');
//                    if((Ki-(float)kval)*1000<10) lcd_putchar('0');
//                    lcd_num((Ki-(float)kval)*1000,3);
//                    lcd_putchar(' ');
//                    break;
//            case 2:lcd_putstr("Kd  \n");lcd_goto(20);
//                   kval=(unsigned char)Kd;
//                   lcd_num(kval,3);
//                   lcd_putchar(' ');
//                    break;
//            case 3: lcd_putstr("start\n");
//                    lcd_putstr("     ");
//                    break;
//            case 5: lcd_putstr("calb\n");
//                    lcd_putstr("      ");
//                    break;
//            case 4: lcd_putstr("mode\n");
//                    if(junction_mode) lcd_putstr("junction");
//                    else            lcd_putstr("line_flw");
//                    break;
//        }
//
//    }



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
        if(integral>5000) integral=5000;           //limit the max i term
        else if(integral<-5000) integral=-5000;
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
	TRISC = 0b10011000;
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

       //motor foward
        Forward();
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
        return (rec_data);					//return the data received
    }
}

void UART_DUMP(void){
    unsigned char dump;

    while (RCIF == 1) //wait for data
    {
        dump=UART_REC();
    }
}