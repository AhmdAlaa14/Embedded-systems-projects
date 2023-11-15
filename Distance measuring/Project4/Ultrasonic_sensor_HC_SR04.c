 /******************************************************************************
 *
 * Module: Ultrasonic sensor
 *
 * File Name: Ultrasonic_sensor_HC_SR04.c
 *
 * Description: Source file for the Ultrasonic sensor driver
 *
 * Author: Ahmed Alaa
 *
 *******************************************************************************/


/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "icu.h"							/* To use the functions of the ICU */
#include "Ultrasonic_sensor_HC_SR04.h"
#include "gpio.h"							/* To control the input and output pins*/
#include <util/delay.h>						/* To use the delay function */

/*******************************************************************************
 *                       Definitions and global variables                      *
 *******************************************************************************/
uint8 edgeCount=0;
uint16 timeHigh=0;
/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
/*
 * Description :
 * 1. Initialize the ICU as Required.
 * 2. Setup the ICU callback function.
 * 3. Setup the direction of the trigger pin as output pin.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_init(void){
	/*  config is a structure to hold the configurations of the icu
	 * Setting the clock of the icu timer to fcpu/8
	 * setting the icu to detect a rising edge
	 *  initializing the icu
	 */
	Icu_ConfigType config;
	config.clock=F_CPU_8;
	config.edge=RISING;
	Icu_init(&config);
	/* setting the callback function for the icu */
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	/* setting up the direction for the trigger pin as an output pin */
	GPIO_setupPinDirection(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,PIN_OUTPUT);
}
/*
 * Description :
 * 1. Send the trigger pulse to the ultrasonic sensor.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_Trigger(void){
	/*
	 * Triggering the sensor is by applying a 10us pulse
	 */

	/* Setting the trigger pin to 1*/
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,LOGIC_HIGH);
	/* Wait for 10us*/
	_delay_us(10);
	/* Resetting the trigger pin to 0 */
	GPIO_writePin(ULTRASONIC_TRIGGER_PORT,ULTRASONIC_TRIGGER_PIN,LOGIC_LOW);
}
/*
 * Description :
 * 1. Send the trigger pulse to the ultrasonic sensor by calling Ultrasonic_Trigger function.
 * 2. Calculate the distance using the returned value from the ICU.
 * Inputs : none.
 * Outputs: uint16 distance .
 */
uint16 Ultrasonic_readDistance(void){
	static uint16 distance=0;
	Ultrasonic_Trigger();
	if(2==edgeCount){
		edgeCount=0;
		distance=timeHigh/58.8;
	}
	return distance;
}
/*
 * Description :
 * 1. The callback function of the ICU.
 * 2. Used to calculate the high time of the returned signal.
 * Inputs : none.
 * Outputs: none.
 */
void Ultrasonic_edgeProcessing(void){
	edgeCount++;								/* a variable to count the edges */

	/*
	 * if the edge count = 1 this means the signal of the sensor has been sent
	 */
	if(1==edgeCount){
		Icu_clearTimerValue();					/* to reset the timer value */
		Icu_setEdgeDetectionType(FALLING);		/* to make the icu detect the falling edge*/
	}

	/*
	 * if the edge count = 2 this means the signal of the sensor has returned
	 */
	if(2==edgeCount){
		/* to capture the time which the signal has been high*/
		timeHigh=Icu_getInputCaptureValue();
		/* making the icu detect the rising edge for the next use of the sensor */
		Icu_setEdgeDetectionType(RISING);
	}
}

