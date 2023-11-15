/*
 * motor.h
 *
 *  Created on: Oct 8, 2021
 *      Author: hp
 */

#ifndef MOTOR_H_
#define MOTOR_H_
/*--------------------------------INCLUDES---------------------------------*/
#include "gpio.h"
#include "std_types.h"

/*---------------------Defintions and configrations------------------------*/
#define 	MOTOR_PORT 			PORTB_ID
#define 	MOTOR_FIRST_PIN 	PIN0_ID
#define 	MOTOR_SECOND_PIN	PIN1_ID

/* ----------------------------Enum definitions----------------------------*/
/*
 * enum name : DcMotor_State
 * enum description : this enum for the states of the DC motor
 */
typedef enum{
	OFF,CW,A_CW
}DcMotor_State;

/*----------------------------Function declaration-------------------------*/
/***************************************************************************
 * 		function name : DcMotor_Init
 *		Description : 1-The Function responsible for setup the direction for
 *					  the two motor pins through the GPIO driver.
 *					  2-Stop at the DC-Motor at the beginning through the GPIO driver.
 *		Args : none
 *		return type : void
 ***************************************************************************/
void DcMotor_Init(void);
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
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* MOTOR_H_ */
