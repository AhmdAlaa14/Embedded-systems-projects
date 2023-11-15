 /******************************************************************************
 *
 * Module: buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Ahmed Alaa
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID PORTD_ID
#define BUZZER_PIN_ID  PIN4_ID
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the buzzer:
 * 1. setup an output port for the buzzer.
 * 2. turn off the buzzer.
 */
void BUZZER_Init(void);
/*
 * Description :
 * Initialize the buzzer:
 * 1. turn on the buzzer.
 */
void BUZZER_On(void);
/*
 * Description :
 * Initialize the buzzer:
 * 1. turn off the buzzer.
 */
void BUZZER_Off(void);

#endif /* BUZZER_H_ */
