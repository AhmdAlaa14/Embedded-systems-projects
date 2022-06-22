/***************************************************************************
 * 		Module : DcMotor
 *		File name : motor.c
 *  	Created on: Oct 8, 2021
 *  	Author: Ahmed Alaa
 ***************************************************************************/

/*--------------------------------INCLUDES---------------------------------*/

#include "motor.h"
#include "pwm.h" /* to use the timer0 in pwm mode*/
#include "common_macros.h" /* to use macros like SET_BIT*/
#include "gpio.h"

/*----------------------------Function definition--------------------------*/

/***************************************************************************
 * 		function name : DcMotor_Init
 *		Description : 1-The Function responsible for setup the direction for
 *					  the two motor pins through the GPIO driver.
 *					  2-Stop at the DC-Motor at the beginning through the GPIO driver.
 *		Args : none
 *		return type : void
 ***************************************************************************/
void DcMotor_Init(void){
	/*Setting pins direction to be output pins */
	GPIO_setupPinDirection(MOTOR_PORT,MOTOR_FIRST_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT,MOTOR_SECOND_PIN,PIN_OUTPUT);
	/*
	 * FIRST AND SECOND BIT NEEDS TO BE ZERO TO TURN OFF THE MOTOR
	 */
	GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN , LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN , LOGIC_LOW);
}

/***************************************************************************
 * 		function name : DcMotor_Rotate
 *		Description :  1-The function responsible for rotate the DC Motor CW/ or A-CW or
 *					   stop the motor based on the state input state value.
 *					   2-Send the required duty cycle to the PWM driver based on the
 *					   required speed value.
 *		Args :
 *		In : DcMotor_State state
 *		In : uint8 speed
 *		return : void
 ***************************************************************************/
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	if(state==CW){
		/*
		 * first pin = 0 , second pin = 1 , to make the motor rotate clockwise
		 */
		GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN , LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN , LOGIC_HIGH);
	}
	else if(state==A_CW){
		/*
		 * first pin = 1 , second pin = 0 , to make the motor rotate clockwise
		 */
		GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN , LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN , LOGIC_LOW);
	}
	else if(state==OFF){
		/*
		 * both pins 0 to stop the motor
		 */
		GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN , LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN , LOGIC_LOW);
	}
	PWM_Timer0_Start(speed); /* to start the pwm mode in timer0 with the required duty cycle*/
}

