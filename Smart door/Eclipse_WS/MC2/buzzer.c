 /******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.c
 *
 * Description: source code file for the LCD driver
 *
 * Author: Ahmed Alaa
 *
 *******************************************************************************/
/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "gpio.h"
#include "buzzer.h"
#include "std_types.h"
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the buzzer:
 * 1. setup an output port for the buzzer.
 * 2. turn off the buzzer.
 */
void BUZZER_Init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	BUZZER_Off();
}
/*
 * Description :
 * Initialize the buzzer:
 * 1. turn on the buzzer.
 */
void BUZZER_On(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
/*
 * Description :
 * Initialize the buzzer:
 * 1. turn off the buzzer.
 */
void BUZZER_Off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
