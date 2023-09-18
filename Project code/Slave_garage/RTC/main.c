/*
 * Here is the slave.c
 *
 * Created: 9/13/2023 4:49:18 PM
 * Author : Haiidy Hossam
 */ 

#define F_CPU 8000000
#include <util/delay.h>
#include "STDTYPE.h"
#include "UTILS.h"
#include "DIO_interface.h"
#include "LCD_Interface.h"
#include "SPI_interface.h"


int main(void)
{
	// Initialize SPI as a slave (You may want to change this to SPI_INIT_Master() if this is meant to be the slave)
	SPI_INIT_SLAVE();


	/*LCD */
	// Configure ports and pins for LCD
	DIO_SetPortDirection(Port_C, Port_output);
	DIO_SetPinDirection(Port_B, Pin_0, output);
	DIO_SetPinDirection(Port_B, Pin_1, output);
	DIO_SetPinDirection(Port_B, Pin_2, output);
	
	LCD_Init();

	u8 SPI_DATA = 0;
	

	while (1)
	{
		// Receive data from the master via SPI
		SPI_DATA = SPI_TR_RC(0);

		 if (SPI_DATA == 'n') // If received data indicates a user is entering
		{
			LCD_Clear();
			GOTOXY(0, 0);
			LCD_String(" Welcome USER: ");

			// Receive and display the user data from the master
			LCD_Send_Data(SPI_TR_RC(0xff));
			_delay_ms(100);
		}
		else if (SPI_DATA == 'x') // If received data indicates a user is exiting
		{
			LCD_Clear();
			GOTOXY(0, 0);
			LCD_String(" BYE USER: ");

			// Receive and display the user data from the master
			LCD_Send_Data(SPI_TR_RC(0xff));
			_delay_ms(100);
		}
	}
}
