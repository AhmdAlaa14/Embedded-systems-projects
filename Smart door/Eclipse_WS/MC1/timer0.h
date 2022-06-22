/*
 * timer0.h
 *
 *  Created on: Oct 31, 2021
 *      Author: hp
 */

#ifndef TIMER0_H_
#define TIMER0_H_
/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "std_types.h"
/*******************************************************************************
 *                           Structures and Enums                              *
 *******************************************************************************/
/*******************************************************************************
 * enum name: Timer_Clock
 * enum Description:
 * this enum contains the values of the pre-scaler
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;
/*******************************************************************************
 * enum name: Timer_Mode
 * enum Description:
 * this enum contains the operation mode
 *******************************************************************************/
typedef enum
{
	OV_MODE , COMPARE_MODE = 2
}Timer_Mode;
/*******************************************************************************
 * structure name: Timer_config
 * structure Description:
 * this structure is responsible for holding the configuration of the timer
 *******************************************************************************/
typedef struct {
	uint8 initValue;
	uint8 compValue;
	Timer_Clock prescalar;
	Timer_Mode timerMode;
}Timer_config;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Initialize the Timer:
 * 1. Setup the timer mode
 * 2. Setup the timer Pre-scaler
 * 3. Setup the start value of the normal mode or the compare value for the compare mode
 */
void TIMER0_Init(Timer_config *config);
/*
 * Description :
 * Sets the wanted call back function to call it in the ISR
 */
void TIMER0_SetCallback(void(*a_pointer)(void));
/*
 * Description :
 * Turns off the timer and return all the counters to 0
 */
void TIMER0_DeInit(void);

#endif /* TIMER0_H_ */
