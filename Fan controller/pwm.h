/*
 * pwm.h
 *
 *  Created on: Oct 9, 2021
 *      Author: hp
 */

#ifndef PWM_H_
#define PWM_H_
/*--------------------------------INCLUDES---------------------------------*/
#include "std_types.h"

/*----------------------------Function declaration-------------------------*/
/***************************************************************************
 * 		function name : PWM_Timer0_start
 *		Description :   1-The function responsible for trigger the Timer0 with the PWM Mode.
 *						2-Setup the PWM mode with Non-Inverting.
 *						3-Setup the prescaler with F_CPU/8.
 *						4-Setup the compare value based on the required input duty cycle
 *						5-Setup the direction for OC0 as output pin through the GPIO driver.
 *						6-The generated PWM signal frequency will be 500Hz to control the DC
 *						  Motor speed.
 *		Args :
 *		In: uint8 duty_cycle
 *		return type : void
 ***************************************************************************/
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
