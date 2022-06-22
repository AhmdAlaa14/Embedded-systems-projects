/***************************************************************************
 * 		Module : ADC
 *		File name : ADC.c
 *  	Created on: Oct 8, 2021
 *  	Author: Ahmed Alaa
 ***************************************************************************/
/*--------------------------------INCLUDES---------------------------------*/
#include "adc.h"
#include "common_macros.h" /* to use macros like SET_BIT*/
#include <avr/io.h> /* to use avr registers*/

/*----------------------------Function definition--------------------------*/

void ADC_init(const ADC_ConfigType * Config_Ptr)
{
	ADMUX=0;
	ADMUX=((ADMUX&0X3F)|((Config_Ptr->ref_volt)<<6)); /* To insert the used voltage reference level */
	ADCSRA=((ADCSRA&0XF8)|(Config_Ptr->prescaler));   /* To insert the used prescaler */
	SET_BIT(ADCSRA,ADEN); 							  /* To enable the ADC */
	CLEAR_BIT(ADCSRA,ADIE);							  /* To disable interrupt */
}

uint16 ADC_readChannel(uint8 channel_num){
	ADMUX=((ADMUX&0XE0)|channel_num);  /* to decide which channel is used*/
	SET_BIT(ADCSRA,ADSC);			   /* to start the conversion */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)){} /* wait until conversion is done */
	SET_BIT(ADCSRA,ADIF);			   /* reset the flag back to zero */
	return ADC;						   /* return the digital value */
}
