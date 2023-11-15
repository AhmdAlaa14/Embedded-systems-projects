 /******************************************************************************
 *
 * Module: Temperature Sensor
 *
 * File Name: lm35_sensor.h
 *
 * Description: source file for the LM35 Temperature Sensor driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "lm35_sensor.h"

#include "adc.h"

float VREF[4]={AREF,5,0,2.56};/* an array to hold the values of Vref*/
/*
 * Description :
 * Function responsible for calculate the temperature from the ADC digital value.
 * Args :
 *  In :  ADC_ConfigType * Config_Ptr
 */
uint8 LM35_getTemperature(const ADC_ConfigType * Config_Ptr)
{
	uint8 temp_value = 0;

	uint16 adc_value = 0;

	/* Read ADC channel where the temperature sensor is connected */
	adc_value = ADC_readChannel(SENSOR_CHANNEL_ID);

	/* Calculate the temperature from the ADC value*/
	temp_value = (uint8)(((uint32)adc_value*SENSOR_MAX_TEMPERATURE*VREF[(Config_Ptr->ref_volt)])\
	/(ADC_MAXIMUM_VALUE*SENSOR_MAX_VOLT_VALUE));

	return temp_value;
}

