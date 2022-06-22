 /******************************************************************************
 *
 * Module: Ultrasonic sensor
 *
 * File Name: Ultrasonic_sensor_HC_SR04.h
 *
 * Description: Header file for the Ultrasonic sensor driver
 *
 * Author: Ahmed Alaa
 *
 *******************************************************************************/

#ifndef ULTRASONIC_SENSOR_HC_SR04_H_
#define ULTRASONIC_SENSOR_HC_SR04_H_

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ULTRASONIC_TRIGGER_PORT   PORTB_ID
#define ULTRASONIC_TRIGGER_PIN    PIN5_ID

#define ULTRASONIC_ECHO_PORT   PORTD_ID
#define ULTRASONIC_ECHO_PIN    PIN6_ID

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "std_types.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * 1. Initialize the ICU as Required.
 * 2. Setup the ICU callback function.
 * 3. Setup the direction of the trigger pin as output pin.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_init(void);
/*
 * Description :
 * 1. Send the trigger pulse to the ultrasonic sensor.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_Trigger(void);
/*
 * Description :
 * 1. Send the trigger pulse to the ultrasonic sensor by calling Ultrasonic_Trigger function.
 * 2. Calculate the distance using the returned value from the ICU.
 * Inputs : none.
 * Outputs: uint16 distance .
 */
uint16 Ultrasonic_readDistance(void);
/*
 * Description :
 * 1. The callback function of the ICU.
 * 2. Used to calculate the high time of the returned signal.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_SENSOR_HC_SR04_H_ */
