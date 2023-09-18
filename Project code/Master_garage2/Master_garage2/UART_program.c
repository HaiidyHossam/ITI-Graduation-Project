/*
 * UART_program.c
 *
 * Created: 6/27/2023 7:01:27 PM
 *  Author: HAIIDY HOSSAM
 */
#include "STDTYPE.h"
#include "UTILS.h"
#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"

void UART_INIT(void)
{

	/*data size 8 bit data this register is byte access,
	 means i cant access every bit */
	u8 UCSRC_mid = 0;
	CLR_BIT(UCSRB, UCSRB_UCSZ2);
	SET_BIT(UCSRC_mid, UCSRC_UCSZ0);
	SET_BIT(UCSRC_mid, UCSRC_UCSZ1);

	// config parity,disable -- cleared
	CLR_BIT(UCSRC_mid, UCSRC_UPM0);
	CLR_BIT(UCSRC_mid, UCSRC_UPM1);
	// config stop bit ,1 bit -- cleared
	CLR_BIT(UCSRC_mid, UCSRC_USBS);
	// config uart asynchronous -- clear
	CLR_BIT(UCSRC_mid, UCSRC_UMSEL);
	UCSRC = UCSRC_mid;
	// config Baud rate 9600 means 51 in data sheet

	UBRRH = 0x00;
	UBRRL = 51;
	// enable rx,tx
	SET_BIT(UCSRB, UCSRB_RXEN);
	SET_BIT(UCSRB, UCSRB_TXEN);

	// config for UCSRC
	/*	u8 UCSRC_mid = 0;
		SET_BIT(UCSRC_mid,UCSRC_URSEL);
		SET_BIT(UCSRC_mid,UCSRC_UCSZ0);
		SET_BIT(UCSRC_mid,UCSRC_UCSZ1);
		UCSRC=UCSRC_mid;
		//baud rate =9600
		UBRRL=51;
		//enable
		SET_BIT(UCSRB,UCSRB_RXEN);
		SET_BIT(UCSRB,UCSRB_TXEN);*/
}

void (*UART_recCallBack)(void) = NULL;
u8 *uart_data_ptr;

// normal receive character
u8 UART_RECEIVE(void)
{
	// if 1 -- receive happened
	// while (GET_BIT(UCSRA, UCSRA_RXC) == 0)
	// 	;
	while (!(UCSRA & (1 << UCSRA_RXC)))
		;

	return UDR;
}
void UART_RECEIVE_String(u8 *array)
{
	for (u8 i = 0; array[i - 1] != '\r'; i++)
	{
		array[i] = UART_RECEIVE();
	}
}

// receive char using interrupt
u8 UART_RECInterrupt(void (*func)(void), u8 *DataPtr)
{
	u8 local = 0;
	if ((func != NULL) && (DataPtr != NULL))
	{
		UART_recCallBack = func;
		uart_data_ptr = DataPtr;
	}
	else
	{
		local = 1;
	}

	// enable int
	SET_BIT(UCSRB, UCSRB_RXCIE);
	return local;
}

// void __vector_13 (void) __attribute__ ((signal));
// void __vector_13 (void){
// 	if (UART_recCallBack !=NULL){
// 		*uart_data_ptr=UDR;
// 		UART_recCallBack();
// 	}
// }

// normal send char
void UART_SEND(u8 data)
{
	// // check if empty UDREMPTY "1 if empty"
	// while (GET_BIT(UCSRA, UCSRA_UDRE) == 0)
	// 	;
	// UDR = data;
	// // check if transmit complete
	// while (GET_BIT(UCSRA, UCSRA_TXC) == 0)
	// 	;
	// // clear flag by writing one
	// SET_BIT(UCSRA, UCSRA_TXC);
	
	// Wait for data register to be empty
	while (!(UCSRA & (1 << UCSRA_UDRE)))
		;

	// Write character to UART
	UDR = data;
}

// send string
void UART_SEND_String(u8 array[])
{
	for (u8 i = 0; (array[i] != '\r') && (array[i] != '\0'); i++)
		UART_SEND(array[i]);
}