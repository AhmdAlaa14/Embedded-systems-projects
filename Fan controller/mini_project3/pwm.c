/***************************************************************************
 * 		Module : timer0 PWM
 *		File name : pwm.c
 *  	Created on: Oct 8, 2021
 *  	Author: Ahmed Alaa
 ***************************************************************************/

/*--------------------------------INCLUDES---------------------------------*/
#include "pwm.h"
#include "common_macros.h" /* to use macros like SET_BIT*/
#include "gpio.h"
#include <avr/io.h> /* to use avr registers */

/*----------------------------Function definition--------------------------*/
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
void PWM_Timer0_Start(uint8 duty_cycle)
{

	/* FOC0=0 to make the timer in PWM mode
	 * COM01 = 1 , COM00 = 0 ,to make the timer on pwm mode non-inverting
	 * for fcpu/8
	 * CS02 = 0 , CS01 = 1 , CS00 = 0
	 * For fast pwm mode
	 * WGM01 = WGM00 =1
	 */

	TCCR0=(1<<COM01)|(1<<CS01)|(1<<WGM01)|(1<<WGM00);
	TCNT0=0; /* To reset the timer starting counter */

	/*
	 * the passed value is the duty cycle percentage.
	 * it needs to be modified to be suitable for the PWM mode
	 * duty cycle = (duty cycle percentage * 255) / 100
	 */

	uint8 compare_value=(uint8)(((uint32)(duty_cycle*255))/100);

	OCR0=compare_value;

	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
}
