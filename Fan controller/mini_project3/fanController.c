/*
 ============================================================================
 Name        : fanController.c
 Author      : Ahmed Alaa
 Description : system to control a fan
 Date        : 8/10/2021
 ============================================================================
 */

/*--------------------------------INCLUDES---------------------------------*/

#include "lcd.h"          								/* to display on the lcd */
#include "motor.h"		  								/* to control the motor */
#include "adc.h"		  								/* to initialize the ADC module */
#include "lm35_sensor.h"  								/* to get the temperature */

int main(){
	uint8 temp;											/* to hold the temperature value */

	ADC_ConfigType s_configType;						/* to hold the configurations of the adc */
	s_configType.prescaler=F_cpu_8;						/* setting the prescaler of the adc to Fcpu/8 */
	s_configType.ref_volt=Vref_internal;				/* setting the vref of the adc to the internal 2.56 v vref */
	ADC_init(&s_configType);							/* to initialize the ADC*/

	LCD_init();											/* to initialize the LCD */
	LCD_displayStringRowColumn(0,3,"Fan is ");			/* Display this string "Fan is " only once on LCD at the first row */
	LCD_displayStringRowColumn(1,3,"Temp =     C");		/* Display this string "Temp =     C" only once on LCD at the second row */
	DcMotor_Init();

	while(1)
	{
		temp=LM35_getTemperature(&s_configType);		/* getting the temperature value from the lm35 sensor */
		LCD_moveCursor(1,10);							/* moving the cursor to display the temperature */
		/* Displaying the temperature on the LCD */
		if(temp>=100){
			LCD_intgerToString(temp);
		}
		else{
			LCD_intgerToString(temp);
			LCD_displayCharacter(' ');
		}
		if(temp>=120)
		{
			DcMotor_Rotate(CW,100);						/* Rotating the motor with the required speed */
			LCD_displayStringRowColumn(0,11,"ON ");		/* Displaying the state of the fan */
		}
		else if(temp>=90)
		{
			DcMotor_Rotate(CW,75);						/* Rotating the motor with the required speed */
			LCD_displayStringRowColumn(0,11,"ON ");		/* Displaying the state of the fan */
		}
		else if(temp>=60)
		{
			DcMotor_Rotate(CW,50);						/* Rotating the motor with the required speed */
			LCD_displayStringRowColumn(0,11,"ON ");		/* Displaying the state of the fan */
		}
		else if(temp>=30)
		{
			DcMotor_Rotate(CW,25);						/* Rotating the motor with the required speed */
			LCD_displayStringRowColumn(0,11,"ON ");		/* Displaying the state of the fan */
		}
		else
		{
			DcMotor_Rotate(OFF,0);						/* Turning off the motor */
			LCD_displayStringRowColumn(0,11,"OFF");		/* Displaying the state of the fan */
		}
	}

}


