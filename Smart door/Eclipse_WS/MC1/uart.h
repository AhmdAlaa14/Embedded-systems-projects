 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
/*******************************************************************************
 *                           Structures and Enums                              *
 *******************************************************************************/
/*******************************************************************************
 * structure name: UART_config
 * structure Description:
 * this structure is responsible for holding the configuration of the UART
 *******************************************************************************/
typedef struct {
	uint32 baudRate;						/* to hold the baud rate */
	uint8 parityType;						/* to hold the parity type */
	uint8 stopBits;							/* to hold the number of stop bits */
	uint8 dataSize;							/* to hold the data size sent using UART */
}UART_config;
/*******************************************************************************
 * enum name: characterSize
 * enum Description:
 * this enum contains the values of the data size
 *******************************************************************************/
enum characterSize{
	BIT_5 , BIT_6 , BIT_7 , BIT_8 , BIT_9 = 7
};
/*******************************************************************************
 * enum name: parityType
 * enum Description:
 * this enum contains the values of the parity bit
 *******************************************************************************/
enum parityType{
	PARITY_OFF , EVEN = 2 , ODD = 3
};
/*******************************************************************************
 * enum name: stopBits
 * enum Description:
 * this enum contains the number of the stop bits
 *******************************************************************************/
enum stopBits{
	ONE_BIT , TWO_BITS
};
/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
/*
 * Description :
 * a function to configure the UART by the wanted configurations
 * IN:
 * UART_config *ptr_config
 * OUT:
 * NONE
 */
void UART_init(UART_config *ptr_config);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
