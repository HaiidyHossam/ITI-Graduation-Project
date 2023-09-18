/*
 * SPI_program.c
 *
 * Created: 7/13/2023 7:51:49 PM
 *  Author: Haiidy Hossam
 */ 
#include "STDTYPE.h"
#include "UTILS.h"
#include "SPI_interface.h"
#include "SPI_private.h"
#include "DIO_interface.h"

void SPI_INIT_MASTER (void){
	//define main 3 pins
	DIO_SetPinDirection(Port_B,Pin_5,output); //MOSI
	DIO_SetPinDirection(Port_B,Pin_6,input); //MISO
	DIO_SetPinDirection(Port_B,Pin_7,output); //SCK
	DIO_SetPinDirection(Port_B,Pin_4,output); //Ss output
	DIO_SetPinValue(Port_B,Pin_4,HIGH); //ss high
	//disable interrupt
	CLR_BIT(SPCR,SPCR_SPIE_PIN);
	
	//LSB --select data direction
	SET_BIT(SPCR,SPCR_DORD_PIN);
	
	//enable master 
	SET_BIT(SPCR,SPCR_MSTR_PIN);
	
	//polarity >> leading edge is rising
	CLR_BIT(SPCR,SPCR_CPOL_PIN);
	//clock phase,set leading
	//Sample at leading edge >> sample= receive
	CLR_BIT(SPCR,SPCR_CPHA_PIN);
	 
	// freq 16 -- master decides the clock
	SET_BIT(SPCR,SPCR_SPR0_PIN);
	CLR_BIT(SPCR,SPCR_SPR1_PIN);
	CLR_BIT(SPSR,SPSR_SPI2X);
	
	//SPI enable
	SET_BIT(SPCR,SPCR_SPE_PIN);
}
void SPI_INIT_SLAVE (void){
	
		DIO_SetPinDirection(Port_B,Pin_5,input); //MOSI
		DIO_SetPinDirection(Port_B,Pin_6,output); //MISO
		DIO_SetPinDirection(Port_B,Pin_7,input); //SCK
		DIO_SetPinDirection(Port_B,Pin_4,input); //Ss output
		
	//disable interrupt
	CLR_BIT(SPCR,SPCR_SPIE_PIN);
	
	//LSB --select data direction
	SET_BIT(SPCR,SPCR_DORD_PIN);
	
	//enable slave SS
	CLR_BIT(SPCR,SPCR_MSTR_PIN);
	
	//polarity >> leading edge is rising
	CLR_BIT(SPCR,SPCR_CPOL_PIN);
	
	//Sample at leading edge >> sample= receive
	CLR_BIT(SPCR,SPCR_CPHA_PIN);
	
	//SPI enable
	SET_BIT(SPCR,SPCR_SPE_PIN);
}
u8 SPI_TR_RC (u8 data){
	//send data
	SPDR=data;
	//wait till data is sent
	while(GET_BIT(SPSR,SPSR_SPIF)==0);
	//return received data
	return SPDR;
	
}