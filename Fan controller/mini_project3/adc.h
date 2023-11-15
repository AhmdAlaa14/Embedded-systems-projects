/***************************************************************************
 * 		Module : ADC
 *		File name : ADC.h
 *  	Created on: Oct 8, 2021
 *  	Author: Ahmed Alaa
 ***************************************************************************/

#ifndef ADC_H_
#define ADC_H_
/*--------------------------------INCLUDES---------------------------------*/
#include "std_types.h"

/*---------------------Defintions and configrations------------------------*/
#define ADC_REF_VOLT_VALUE VREF[(Config_Ptr->ref_volt)]
#define ADC_MAXIMUM_VALUE    1023
#define AREF 5
/* ---------------------Enum and structures definitions--------------------*/

/*
 * enum name : ADC_ReferenceVoltage
 * enum description : this enum for the used Vref
 */

typedef enum{
	Vref_AREF , Vref_AVCC , Vref_internal = 3
}ADC_ReferenceVolatge;

/*
 * enum name : ADC_Prescaler
 * enum description : this enum for the used prescaler
 */

typedef enum{
	F_cpu_2 = 1 , F_cpu_4 , F_cpu_8 , F_cpu_16 , F_cpu_32 , \
	F_cpu_64 , F_cpu_128
}ADC_Prescaler;

/*
 * Structure name : ADC_ConfigType
 * Structure description : this struct is used to hold the used configurations
 */

typedef struct{
	ADC_ReferenceVolatge ref_volt;
	ADC_Prescaler prescaler;
}ADC_ConfigType;
/*----------------------------Function declaration-------------------------*/
/***************************************************************************
 * 		function name : ADC_init
 *		Description :
 *		Args :
 *		In : ADC_ConfigType * Config_Ptr
 *		return type : void
 ***************************************************************************/
void ADC_init(const ADC_ConfigType * Config_Ptr);
/***************************************************************************
 * 		function name : ADC_readChannel
 *		Description : Function responsible for read analog data from a certain ADC channel
 * 					  and convert it to digital using the ADC driver.
 *		Args :
 *		In : uint8 ch_num
 *		Out: uint16 ADC
 *		return type : uint16
 ***************************************************************************/
uint16 ADC_readChannel(uint8 ch_num);

#endif /* ADC_H_ */
