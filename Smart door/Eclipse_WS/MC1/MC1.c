/*
 ================================================================================================
 Name        : MC1.c
 Author      : Ahmed Alaa
 Description : Code of the interfacing MC
 Date        : 31/10/2021
 ================================================================================================
 */
#define F_CPU 8000000UL
/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "timer0.h"
#include "util/delay.h"
#include "avr/io.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PASSWORD_SIZE 5
#define DATA_SIZE 7
#define INCORRECT 0
#define CORRECT 1
/* commands between MC's*/
#define	SAVE 0x20
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'
#define LIMIT_PASSED 0xff
#define LIMIT_NOT_PASSED 0xFE
/*******************************************************************************
 *                              Global variables                               *
 *******************************************************************************/
uint16 tick=0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function takes the password from the user two times and compare them if matched
 * and if not matched it asks the user to try again and again till a correct match
 * then sends the password to the other MC to save it
 * Args:
 * IN:
 * uint8 ptr_password
 * OUT:
 * NONE
 */
void SetPassword(uint8 *);
/*
 * Description :
 * Function takes the password from the user and store it in an array
 * Args:
 * IN:
 * uint8 ptr_password
 * OUT:
 * NONE
 */
void GetPassword(uint8 *);
/*
 * Description :
 * Function to display the status of the door
 * 1. initialize the timer.
 * 2. wait for the tick to change and change the status of the motor depending on the value of tick.
 * 3. deinitialize the timer.
 * Args:
 * IN:
 * NONE
 * OUT:
 * uint8 result
 */
void OpenDoor(void);
/*
 * Description :
 * the callback function of the timer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void CountSeconds(void);
/*
 * Description :
 * Function sends password to the other MC
 * Args:
 * IN:
 * uint8 ptr_password
 * OUT:
 * NONE
 */
void SendPassword(uint8 *ptr_password);
/*
 * Description :
 * Function makes an alarm .
 * 1. initialize the timer.
 * 2. display an error meassage on the screen.
 * 3. wait for 60 seconds .
 * 4. clear the LCD.
 * 5. deinitialize the timer.
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * NONE
 */
void Alarm(void);
/*
 * Description :
 * Function makes an alarm .
 * 1. enable the global interrupt flag.
 * 2. initialize the UART and the TWI with configurations.
 * 3. initialize the motor and the buzzer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void MC1Init(void);
/*******************************************************************************
 *                                Application                                  *
 *******************************************************************************/
int main(void){
	/*password is an array of 8 characters
	 * first 5 elements for password
	 * 6th for the enter ASCII = 13
	 * 7th for the command needed to be done the other MC
	 */
	uint8 password[7];
	uint8 operation;												/* to hold the wanted operation from the user */
	uint8 passwordComResult=INCORRECT;								/* to hold the result of the comparing password process*/
	uint8 limitStatus;												/* to hold if the number of the wrong password entries */
	MC1Init();														/* to initialize the peripherals used in MC1*/
	SetPassword(password);											/* To set the password for the first time*/

	while(1){
		LCD_clearScreen();											/* Clearing the LCD*/
		/* Printing the menu on the LCD */
		LCD_displayString("+ open door ");
		LCD_displayStringRowColumn(1,0,"- change pass");
		LCD_moveCursor(2,0);
		operation=KEYPAD_getPressedKey();							/* to get he wanted operation from the user */
		_delay_ms(250);
		switch(operation){
		case '+':
			/* a do while loop to get the password from the user and it's only terminated if the password is correct or the
			 * the password has been entered wrong for 3 times in a row  */
			do{
				GetPassword(password);								/* to get the password from the user*/
				password[6]=OPEN_DOOR;								/* putting the operation in the password array */
				SendPassword(password);								/* sending the password array to the other MC*/
				passwordComResult=UART_recieveByte();				/* getting the result of the comparison from the other MC*/
				limitStatus=UART_recieveByte();						/* getting if the limit of wrong passwords is passed or not */
			}while((passwordComResult==INCORRECT)&&(limitStatus==LIMIT_NOT_PASSED));
			/* if the limit is passed an error will be displayed on the LCD */
			if(limitStatus==LIMIT_PASSED){
				Alarm();
			}
			/* if the password is correct the door will open and display the status of the door on the LCD */
			if(passwordComResult==CORRECT){
				OpenDoor();
			}
			break;
		case '-':
			/* a do while loop to get the password from the user and it's only terminated if the password is correct or the
			 * the password has been entered wrong for 3 times in a row  */
			do{
				GetPassword(password);								/* to get the password from the user*/
				password[6]=CHANGE_PASSWORD;						/* putting the operation in the password array */
				SendPassword(password);								/* sending the password array to the other MC*/
				passwordComResult=UART_recieveByte();				/* getting the result of the comparison from the other MC*/
				limitStatus=UART_recieveByte();						/* getting if the limit of wrong passwords is passed or not */
			}while((passwordComResult==INCORRECT)&&(limitStatus==LIMIT_NOT_PASSED));
			/* if the limit is passed an error will be displayed on the LCD */
			if(limitStatus==LIMIT_PASSED){
				Alarm();
			}
			/* if the password is correct it will ask the user to enter the new password */
			if(passwordComResult==CORRECT){
				SetPassword(password);
			}
			break;
		}

	}

}
/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
/*
 * Description :
 * Function takes the password from the user two times and compare them if matched
 * and if not matched it asks the user to try again and again till a correct match
 * then sends the password to the other MC to save it
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * NONE
 */
void SetPassword(uint8 * ptr_password){
	uint8 i=0;														/* for loop counter */
	uint8 compareFlag=0;											/* a flag to indicate if the passwords are matched or not */
	uint8 repeatedPassword[7];										/* an array to hold the second entry of the password */
	LCD_clearScreen();												/* clearing the screen before printing anything*/
	do {
		compareFlag=0;
		LCD_clearScreen();											/* to clear the LCD screen */
		GetPassword(ptr_password);										/* to get the password from the user */
		LCD_moveCursor(2,0);										/* to move the cursor to the third row */
		LCD_displayString("Reenter pass"); 							/* printing the string "Reenter password "*/
		LCD_moveCursor(3,0);										/* to move the cursor to the fourth row */
		/*
		 * a for loop to get the pressed keys from the user and to print '*' on the LCD and compare the repeated password to the first one
		 */
		for(i=0;i<PASSWORD_SIZE;i++){

			_delay_ms(250);
			repeatedPassword[i]=KEYPAD_getPressedKey();
			_delay_ms(250);
			LCD_displayCharacter('*');
			if(repeatedPassword[i]!=ptr_password[i]){
				compareFlag=1;
			}
		}
		_delay_ms(250);
		repeatedPassword[5]=KEYPAD_getPressedKey();					/* to get the enter from the user */
		_delay_ms(250);
		if(compareFlag==1||((ptr_password[5]!=13)&&(repeatedPassword[5]!=13))){
			LCD_clearScreen();
			LCD_displayString("Pass not matched");					/* to display an error message if the two passwords are not matched */
			_delay_ms(1500);										/* to wait a second and a half before entering the password again*/
		}
	}while(compareFlag==1||((ptr_password[5]!=13)&&(repeatedPassword[5]!=13)));
	ptr_password[6]=SAVE;												/* getting the order into the password array */
	SendPassword(ptr_password);											/* sending the password to the other MC to save it in the EEPROM */
	LCD_clearScreen();												/* to clear the LCD*/
}
/*
 * Description :
 * Function takes the password from the user and store it in an array
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * NONE
 */
void GetPassword(uint8 * ptr_password){
	uint8 i=0;														/* for loop counter */
	LCD_clearScreen();												/* to clear the LCD*/
	LCD_displayString("Enter pass"); 								/* printing the string "Enter pass" on the LCD */
	LCD_moveCursor(1,0);											/* moving the cursor to the second row */
	/* for loop to store the password in an array */
	for(i=0;i<PASSWORD_SIZE;i++){
		_delay_ms(250);
		ptr_password[i]=KEYPAD_getPressedKey();
		_delay_ms(250);
		LCD_displayCharacter('*');
	}
	_delay_ms(250);
	ptr_password[5]=KEYPAD_getPressedKey();								/* to get the enter from the user */
	_delay_ms(250);
}

/*
 * Description :
 * Function to display the status of the door
 * 1. initialize the timer.
 * 2. wait for the tick to change and change the status of the motor depending on the value of tick.
 * 3. deinitialize the timer.
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void OpenDoor(void){

	/*
	 * this function is responsible for initializing the timer and setting the timer's callback function
	 */

	TIMER0_SetCallback(CountSeconds);			/* setting the callback function of the timer0 */
	Timer_config timerConfig;					/* a structure to hold the configurations of the timer */
	timerConfig.timerMode=OV_MODE;
	timerConfig.initValue=6;
	timerConfig.compValue=250;
	timerConfig.prescalar=F_CPU_1024;
	TIMER0_Init(&timerConfig);

	while(tick<=1031){
		if(tick==1){
			LCD_clearScreen();
			LCD_displayString("door opening");
		}
		if(tick==469){
			/* this means a 15 seconds has passed so the door needs to be hold for 3 seconds */
			LCD_clearScreen();
			LCD_displayString("The door is open");
		}
		else if(tick==563){
			/* this means the has been open for 3 seconds so it needs to be closed */
			LCD_clearScreen();
			LCD_displayString("Door closing");
		}
		else if(tick==1031){
			/* this means the door has returned to its initial position after 33 seconds */
			/* so the motor stops and the tick timer is reset to zero and the timer is de-initalized */
			LCD_clearScreen();
			LCD_displayString("Door closed");
			TIMER0_DeInit();
			_delay_ms(4000);
			tick++;
		}
	}
	tick=0;
}

/*
 * Description :
 * the callback function of the timer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void CountSeconds(void){
	tick++ ;
}
/*
 * Description :
 * Function makes an alarm .
 * 1. initialize the timer.
 * 2. display an error meassage on the screen.
 * 3. wait for 60 seconds .
 * 4. clear the LCD.
 * 5. deinitialize the timer.
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * NONE
 */
void SendPassword(uint8 *ptr_password){
	for(uint8 i=0; i<DATA_SIZE;i++){
		UART_sendByte(ptr_password[i]);
	}
}
/*
 * Description :
 * Function makes an alarm .
 * 1. initialize the timer.
 * 2. display an error meassage on the screen.
 * 3. wait for 60 seconds .
 * 4. clear the LCD.
 * 5. deinitialize the timer.
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */

void Alarm(void){

	/*
	 * this function is responsible for initializing the timer and setting the timer's callback function
	 */

	TIMER0_SetCallback(CountSeconds);			/* setting the callback function of the timer0 */
	Timer_config timerConfig;					/* a structure to hold the configurations of the timer */
	timerConfig.timerMode=OV_MODE;
	timerConfig.initValue=6;
	timerConfig.compValue=250;
	timerConfig.prescalar=F_CPU_1024;
	TIMER0_Init(&timerConfig);

	LCD_clearScreen();
	LCD_displayString("ERROR!!!");
	while(tick<=1875){}
	tick=0;
}
/*
 * Description :
 * Function to initialize the peripherals used in the MC.
 * 1. enable the global interrupt flag.
 * 2. initialize the UART and the TWI with configurations.
 * 3. initialize the motor and the buzzer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void MC1Init(void){
	SREG|=(1<<7);
	/* Configurations and initialization of the UART*/
	UART_config uartConfig;
	uartConfig.baudRate=9600;
	uartConfig.dataSize=BIT_8;
	uartConfig.parityType=PARITY_OFF;
	uartConfig.stopBits=ONE_BIT;
	UART_init(&uartConfig);

	LCD_init();	/* Initializing the LCD*/
}
