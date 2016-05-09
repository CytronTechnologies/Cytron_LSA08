//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller
//	Project description	: Header file for IFC-CP04
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

void cp1_char(unsigned char data);
void cp2_char(unsigned char data);
void cp1_str(static const rom char *s);
void cp2_str(static const rom char *s);
void cp1_dec(unsigned long data,unsigned char num_dig);
void cp2_dec(unsigned long data,unsigned char num_dig);
void cp1_bin(unsigned short data,unsigned char num_dig);
void cp2_bin(unsigned short data,unsigned char num_dig);
void cp1_config(unsigned char data);
void cp2_config(unsigned char data);
unsigned char cp1_sw(unsigned char button_number);
unsigned char cp2_sw(unsigned char button_number);
void cp1_blight(unsigned char data);
void cp2_blight(unsigned char data);
void cp1_clr(void);
void cp2_clr(void);
void cp1_goto(unsigned char r,unsigned char c);
void cp2_goto(unsigned char r,unsigned char c);

// =================================================================================
// Library Functions
// This Library Functions will explain on the function prototype of cp'x', x= 1 or 2.
// when x=1, the function prototype is for Control Panel 1, CP1 on IFC-CP04,
// when x=2, the function prototype is for Control Panel 2, CP2 on IFC-CP04,
// Please make sure the Control Panel selected on IFC-CP04 is compatible with 
// program.
// =================================================================================

// void cp'x'_char(unsigned char data)
// ---------------------------------------------------------------------------------
// To display character on LCD, data= character to be display on LCD.
// Example: 
//			cp1_char('A');      	 Select CP1, display character 'A' on LCD.
//			cp2_char('c');			 Select CP2, display character 'c' on LCD.
//
// ---------------------------------------------------------------------------------

// void cp'x'_str(static const rom char *s);
// ---------------------------------------------------------------------------------
// To display string on LCD, *s= string to send.
// Example:
//			cp1_str("Cytron");		 Select CP1, display string 'Cytron' on LCD.
//			cp2_str("Technologies"); Select CP2, display string 'Technologies' on LCD.
//
// ---------------------------------------------------------------------------------

// void cp'x'_dec(unsigned long data,unsigned char num_dig);
// ---------------------------------------------------------------------------------
// To display decimal number on LCD, data = number, num_dig = number of digit to display.
// Example:
//			cp1_dec(123456,6);		 Select CP1, display decimal number '123456' on LCD.
//			cp2_dec(654321,6);		 Select CP2, display decimal number '654321' on lCD.
//
// ---------------------------------------------------------------------------------

// void cp'x'_bin(unsigned short data,unsigned char num_dig);
// ---------------------------------------------------------------------------------
// To display binary number on LCD, data = number, num_dig = number of digit to display.
// Example:
//			cp1_bin(0b101010,6);	 Select CP1, display binary number '101010' on LCD.
//			cp2_bin(0b111000,6);	 Select CP2, display binary number '111000' on LCD.
//
// ---------------------------------------------------------------------------------

// void cp'x'_config(unsigned char data);
// ---------------------------------------------------------------------------------
// To set the LCD configuration. For more information about LCD configuration 
// please refer to our website www.cytron.com.my.
// However, user can control the LCD by using the other function without setting the
// configuration yourself. This is a function for advance development.
// Example:
//			cp1_config(44);			 Select CP1, change the LCD configuration
//			cp2_config(42);			 Select CP2, change the LCD configuration
//
// ---------------------------------------------------------------------------------

// unsigned char cp'x'_sw(unsigned char button_number);
// ---------------------------------------------------------------------------------
// To define which switch will be tested. button_number  = switch to be test.
// All the switches on IFC-CP04 are active low, which means it will give value ¡®0¡¯ to 
// microcontroller when it is being pressed. 
// Example:
//         if (cp1_sw(4)= =0)		 Test SW4 on CP1, if pressed, led1 will ON.
//			{
//				led1 = 1;			
//			} 
//
// User can also read all switch value in one byte (0000DCBA, D=SW4 C=SW3 B=SW2 A=SW1) 
// by calling this function as below:
//
//				cp1_sw(0)   
//
// ---------------------------------------------------------------------------------

// void cp'x'_blight(unsigned char data);
// ---------------------------------------------------------------------------------
// To adjust the backlight of LCD screen. data=backlight brightness (0-255)
// Example:
//				cp1_blight (255);	 Select CP1, set maximum brightness for LCD
//				cp2_blight (10);     Select CP2, set minimum brightness for LCD
//
// ---------------------------------------------------------------------------------

// void cp'x'_clr(void);
// ---------------------------------------------------------------------------------
// To clear LCD display.It is recommended for user to call this function when user  
// wants to display new message on LCD screen. If this function is not used, user will  
// see previous message on the LCD screen.
// Example:
//				cp1_clr();			 Select CP1, clear LCD display
//				cp2_clr();			 Select CP2, clear LCD display
//
// ---------------------------------------------------------------------------------

// void cp'x'_goto(unsigned char r,unsigned char c);
// ---------------------------------------------------------------------------------
// To define cursor location to start up a sentence, character or decimal number. 
// Char r is defined as row and char c is defined as column.
// ---------------------------------------------------------------------------
// | |0,0|0,1|0,2|0,3|0,4|0,5|0,6|0,7|0,8|0,9|0,10|0,11|0,12|0,13|0,14|0,15| |
// | |1,0|1,1|1,2|1,3|1,4|1,5|1,6|1,7|1,8|1,9|1,10|1,11|1,12|1,13|1,14|1,15| |
// ---------------------------------------------------------------------------
// Example:
// 				cp1_goto(0,3);		 Select CP1, cursor goto row 0, column 3.
// 				cp2_goto(1,7);		 Select CP2, cursor goto row 1, column 7.	
//
// ---------------------------------------------------------------------------------
