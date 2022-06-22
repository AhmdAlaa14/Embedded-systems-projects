/*
 ================================================================================================
 Name        : MC2.c
 Author      : Ahmed Alaa
 Description : Code of the controlling MC
 Date        : 31/10/2021
 ================================================================================================
 */
#define F_CPU 8000000UL
/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "motor.h"								/* to use the motor */
#include "buzzer.h"								/* to use the buzzer */
#include "uart.h"								/* to send and receive data from the other MC*/
#include "external_eeprom.h"					/* to store data in */
#include "twi.h"								/* to use the init function */
#include "timer0.h"								/* to count seconds */
#include "util/delay.h"							/* to use the delay function */
#include "avr/io.h"								/* to enable global interrupt flag */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE 5
#define DATA_SIZE 7
#define INCORRECT 0
#define CORRECT 1
#define PASSSWORD_ADDRESS 0x0300
#define NUMBER_OF_OVERFLOWS_PER_SECOND 31
/********* Command between MC's********/
#define	SAVE 0x20
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'
#define LIMIT_PASSED 0xFF
#define LIMIT_NOT_PASSED 0xFE

/*******************************************************************************
 *                              Global variables                               *
 *******************************************************************************/
static const uint16 passwordAddress=PASSSWORD_ADDRESS;
static uint16 tick=0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Function compares the received password from the other MC
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * uint8 result
 */
uint8 ComparePassword(uint8* ptr_password);
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
 * Function to open the door
 * 1. initialize the timer.
 * 2. wait for the tick to change and change the state of the motor depending on the value of tick.
 * 3. deinitialize the timer.
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void OpenDoor(void);
/*
 * Description :
 * Function save the received password from the other MC in EEPROM instead of the old one
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void ChangePassword(uint8 *ptr_password);
/*
 * Description :
 * Function receives password from the other MC
 * Args:
 * IN:
 * uint8 * ptr_password
 * OUT:
 * NONE
 */
void RecievePassword(uint8 *ptr_password);
/*
 * Description :
 * Function makes an alarm .
 * 1. initialize the timer.
 * 2. turn on the the buzzer.
 * 3. wait for 60 seconds .
 * 4. turn off the buzzer.
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
 * initialize the used peripherals in the MC.
 * 1. enable the global interrupt flag.
 * 2. initialize the UART and the TWI with configurations.
 * 3. initialize the motor and the buzzer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void MC2Init(void);
/*******************************************************************************
 *                                Application                                  *
 *******************************************************************************/

int main(void){
	uint8 password[7];						/* an array to hold the transferred data*/
	uint8 passwordComResult;				/* to hold the result of the password comparison */
	uint8 misMatchCounter=0;				/* to count the number of mismatched passwords */
	uint8 operation;						/* to hold the operation wanted by the user */
	MC2Init();								/* to initialize the peripherals used in MC2*/
	RecievePassword(password);			/* receiving the password for the first time and saving it */
	ChangePassword(password);				/* to save the password for the first time */
	while(1){
		/*
		 * a do while loop to receive the password and compare it at least once and at most three times
		 * the do while loop stands while the result of the comparison is false and the number of mismatches
		 * is less than three
		 */
		do{
			RecievePassword(password);							/* receiving the password */

			passwordComResult=ComparePassword(password);		/* comparing the password */
			UART_sendByte(passwordComResult);					/* sending back the result of the comparison */
			/* checking on the number of mismatches */
			if(passwordComResult==INCORRECT){
				misMatchCounter++;								/* increasing the mismatch counter*/
				if(misMatchCounter==3){
					/*alarm();*/
					UART_sendByte(LIMIT_PASSED);				/* sending back that the limit of trial is matched */
				}
				else{
					UART_sendByte(LIMIT_NOT_PASSED);
				}
			}
			else{
				UART_sendByte(LIMIT_NOT_PASSED);
			}
		}while( (!passwordComResult ) && (misMatchCounter!=3) );
		/*
		 * the operation wanted is held in the 6th element of the password
		 */
		operation=password[6];									/* taking the wanted operation from the received password array*/
		if(misMatchCounter==3){
			operation=LIMIT_PASSED;								/* changing the operation if mismatch number is 3*/
			Alarm();											/* calling the alarm function to turn on the buzzer */
		}

		misMatchCounter=0;										/* making the mismatch counter 0 again */

		switch(operation){
		case '+':
			OpenDoor();											/* to open the door */
			break;
		case '-':
			RecievePassword(password);							/* to take the new password from the user */
			ChangePassword(password);							/* to store the password in the EEPROM */
			break;
		case LIMIT_PASSED:
			break;
		}
	}
}

/*******************************************************************************
 *                      Functions definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Function compares the received password from the other MC
 * Args:
 * IN:
 * uint8 ptr_password
 * OUT:
 * uint8 result
 */
uint8 ComparePassword(uint8* ptr_password){
	uint8 result=CORRECT;										/* a variable to hold the result of the comparison */
	uint8 comparedCharacter;									/* a variable to hold the character received from the memory*/
	uint8 i=0;													/* a counter for the for loop */

	/* a for loop to compare every character of the password */
	for(i=0;i<PASSWORD_SIZE;i++){
		EEPROM_readByte(passwordAddress+i,&comparedCharacter);	/* reading a character from the memory */
		/* comparing every character */
		if(ptr_password[i]!=comparedCharacter){
			result=INCORRECT;
		}
	}
	return result;
}

/*
 * Description :
 * Function to open the door
 * 1. initialize the timer.
 * 2. wait for the tick to change and change the state of the motor depending on the value of tick.
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
	Timer_config timerConfig;					/* a structure to hold the conigurations of the timer */
	timerConfig.timerMode=OV_MODE;
	timerConfig.initValue=6;
	timerConfig.compValue=250;
	timerConfig.prescalar=F_CPU_1024;
	TIMER0_Init(&timerConfig);

	while(tick<=1031){
		if (tick==1){
			/* start rotating the motor to open the door */
			DcMotor_Rotate(CW);
		}
		else if(tick==469){
			/* this means a 15 seconds has passed so the door needs to be hold for 3 seconds */
			DcMotor_Rotate(OFF);
		}
		else if(tick==563){
			/* this means the has been open for 3 seconds so it needs to be closed */
			DcMotor_Rotate(A_CW);
		}
		else if(tick==1031){
			/* this means the door has returned to its initial position after 33 seconds */
			/* so the motor stops and the tick timer is reset to zero and the timer is de-initalized */
			DcMotor_Rotate(OFF);
			TIMER0_DeInit();
			tick++;
		}
	}
	tick=0;
}

/*
 * Description :
 *the callback function of the timer
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
 * Function receives password from the other MC
 * Args:
 * IN:
 * uint8 *ptr_password
 * OUT:
 * NONE
 */
void ChangePassword(uint8 *ptr_password){
	uint8 i=0;
	for(i=0;i<PASSWORD_SIZE;i++){
		EEPROM_writeByte(passwordAddress+i,ptr_password[i]);
		_delay_ms(10);
	}
}
/*
 * Description :
 * Function receives password from the other MC
 * Args:
 * IN:
 * uint8 * ptr_password
 * OUT:
 * NONE
 */
void RecievePassword(uint8 *ptr_password){
	for (uint8 i=0;i<DATA_SIZE;i++){
		ptr_password[i]=UART_recieveByte();
	}
}
/*
 * Description :
 * Function makes an alarm .
 * 1. initialize the timer.
 * 2. turn on the the buzzer.
 * 3. wait for 60 seconds .
 * 4. turn off the buzzer.
 * 5. deinitialize the timer.
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */

void Alarm(void){
	TIMER0_SetCallback(CountSeconds);			/* setting the callback function of the timer0 */
	Timer_config timerConfig;					/* a structure to hold the configurations of the timer */
	timerConfig.timerMode=OV_MODE;
	timerConfig.initValue=6;
	timerConfig.compValue=250;
	timerConfig.prescalar=F_CPU_1024;
	TIMER0_Init(&timerConfig);
	BUZZER_On();
	while(tick<=1875){}
	BUZZER_Off();
	tick=0;
}

/*
 * Description :
 * initialize the used peripherals in the MC.
 * 1. enable the global interrupt flag.
 * 2. initialize the UART and the TWI with configurations.
 * 3. initialize the motor and the buzzer
 * Args:
 * IN:
 * NONE
 * OUT:
 * NONE
 */
void MC2Init(void){
	SREG|=(1<<7);
	DcMotor_Init();							/* initializing the DC motor*/
	BUZZER_Init();							/* initializing the buzzer */

	/* Configurations and initialization of the UART*/
	UART_config uartConfig;					/* a structure to hold the configurations of the UART*/
	uartConfig.baudRate=9600;
	uartConfig.dataSize=BIT_8;
	uartConfig.parityType=PARITY_OFF;
	uartConfig.stopBits=ONE_BIT;
	UART_init(&uartConfig);

	/* Configurations and initialization of TWI */
	TWI_config twiConfig;					/*  a structure to hold the configurations of the TWI */
	twiConfig.address=0x02;
	twiConfig.mode=FAST_MODE;
	TWI_init(&twiConfig);
}
