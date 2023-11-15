/*
 * timer0.c
 *
 *  Created on: Oct 31, 2021
 *      Author: hp
 */
/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "std_types.h"
#include "timer0.h"
#include <avr/io.h>				/* To use the registers*/
#include <avr/interrupt.h>		/* To use the ISR */
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variable to hold the address of the call back function in the application */
static void (*g_callBackPtr)(void) = NULL_PTR;
static uint8 g_initValue=0; 						/* to hold the value of the start value for the normal mode*/

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect){
	OCR0 = g_initValue;
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the timer overflowed */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect){
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare value is matched */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Initialize the Timer:
 * 1. Setup the timer mode
 * 2. Setup the timer Pre-scaler
 * 3. Setup the start value of the normal mode or the compare value for the compare mode
 */
void TIMER0_Init (Timer_config *config) {
	/************************** TCCR0 Description **************************
	 * FOC0 = 1 for non pwm mode
	 * WGM00:01 to decide the mode of operation
	 * COM00:01 = 0 to control the OC0 usage
	 * CS02:00 to choose the used prescaler
	 ***********************************************************************/
	TCCR0 = (1<<FOC0);
	TCCR0 = (TCCR0&0xB7) | (((config->timerMode)&0x02)<<5) | (((config->timerMode)&0x01)<<3);
	TCCR0 = (TCCR0&0xF8) | (config->prescalar);

	if(OV_MODE==config->timerMode){
		TCNT0 = config->initValue;
		g_initValue = config->initValue;
		TIMSK |= (1<<TOIE0);
	}
	else if (COMPARE_MODE == config->timerMode){
		OCR0 = config->compValue ;
		TIMSK |= (1<<OCIE0);
	}
}
/*
 * Description :
 * Sets the wanted call back function to call it in the ISR
 */
void TIMER0_SetCallback(void(*a_pointer)(void)){
	g_callBackPtr = a_pointer;
}
/*
 * Description :
 * Turns off the timer and return all the registers to 0
 */
void TIMER0_DeInit(void){
	TCCR0 = 0;
	TIMSK &= 0xFC ;
	TCNT0 = 0;
	OCR0 = 0;
}
