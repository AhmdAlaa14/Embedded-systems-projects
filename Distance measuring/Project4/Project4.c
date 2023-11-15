 /******************************************************************************
 *
 * Module: Fourth project
 *
 * File Name: Project4.c
 *
 * Description: Application source file of the fourth project
 *
 * Author: Ahmed Alaa
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/
#include "lcd.h"							/* To use the LCD */
#include "Ultrasonic_sensor_HC_SR04.h"		/* To read the distance from the sensor */
#include <avr/io.h>							/* To enable the global interrupt flag */


int main(void){
	SREG|=(1<<7);									/* Enable the global interrupt flag */
	uint16 distance=0;								/* A variable to hold the distance*/
	LCD_init();										/* Initializing the LCD */
	Ultrasonic_init();								/* Initializing the Ultrasonic sensor*/
	/* Displaying the string "Distance =    cm" on the LCD */
	LCD_displayString("Distance =    cm");
	while(1){
		LCD_moveCursor(0,11);						/* Moving the cursor to print the distance*/
		distance=Ultrasonic_readDistance();			/* To read the distance from the sensor*/
		LCD_intgerToString(distance);				/* To print the distance on the screen*/
		/*
		 * Overcoming the issue of the excess numbers on the screen from the previous value
		 */
		if(distance<100){
			LCD_moveCursor(0,13);
			LCD_displayCharacter(' ');
		}
		if(distance<10){
			LCD_moveCursor(0,12);
			LCD_displayCharacter(' ');
		}
	}
}
