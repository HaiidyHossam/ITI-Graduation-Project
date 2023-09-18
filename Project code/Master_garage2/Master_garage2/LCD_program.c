#define F_CPU 8000000

#include "STDTYPE.h"
#include "DIO_interface.h"
#include "LCD_Config.h"
#include "avr/delay.h"

void LCD_Send_Command(u8 command)
{
	/* set RS as LOW */
	DIO_SetPinValue(CTRL_PORT, RS_Pin, LOW);
	/* set RW as LOW to Write */
	DIO_SetPinValue(CTRL_PORT, RW_Pin, LOW);

	// send command to data port
	DIO_SetPortValue(Data_Port, command);

	// set enable pulse
	DIO_SetPinValue(CTRL_PORT, Enable_Pin, HIGH);
	_delay_ms(3);
	DIO_SetPinValue(CTRL_PORT, Enable_Pin, LOW);
}
void LCD_Send_Data(u8 Data)
{
	/* set RS as HIGH */
	DIO_SetPinValue(CTRL_PORT, RS_Pin, HIGH);
	/* set RW as LOW to Write */
	DIO_SetPinValue(CTRL_PORT, RW_Pin, LOW);

	// send data to data port
	DIO_SetPortValue(Data_Port, Data);
	// set enable pulse
	DIO_SetPinValue(CTRL_PORT, Enable_Pin, HIGH);
	_delay_ms(3);
	DIO_SetPinValue(CTRL_PORT, Enable_Pin, LOW);
}
void LCD_Init(void)
{

	_delay_ms(40);

	// function set
	LCD_Send_Command(0b00111111);

	_delay_us(40);

	// On-off
	LCD_Send_Command(0b00001100);
	_delay_us(40);

	// clear
	LCD_Send_Command(0b00000001);
	_delay_ms(2);

	// entry set mode
	LCD_Send_Command(0b00000110);
	_delay_ms(40);
}
void GOTOXY(u8 x, u8 y)
{ //(1,4)
	u8 address = 0;
	if (x == 0)
	{
		address = y;
	}
	else if (x == 1)
	{
		address = 0x40 + y; // 44=0b01000100
	}
	// DDram address = 0b10000000 //80
	LCD_Send_Command(0b10000000 + address);
}

void LCD_Clear()
{
	LCD_Send_Command(0b00000001);
	_delay_ms(2);
	LCD_Send_Command(0b10000000);
}

void LCD_String(char *str) /* Send string to LCD function */
{
	int i;
	for (i = 0; str[i] != 0; i++) /* Send each char of string till the NULL */
	{
		LCD_Send_Data(str[i]);
	}
}

// using clcd custom character generator
void CLCD_VidWriteSpecialCharacter(u8 *Copy_pu8Pattern, u8 Copy_u8PatternNumber, u8 Copy_u8XPOS, u8 Copy_u8YPOS)
{
	u8 Local_u8Address = 0;
	// Copy_u8PatternNumber maybe 0,1,2 -- 7
	// CGram consists of 8 locations so if i wanna get the address:
	Local_u8Address = 8 * Copy_u8PatternNumber;
	// go to this address by the key of CGram "its address 64"
	LCD_Send_Command(64 + Local_u8Address);
	// write on this address
	for (u8 i = 0; i < 8; i++)
	{
		// byte by byte for each location of the 8 ones
		// send data to cgram it won't appear on lcd
		LCD_Send_Data(Copy_pu8Pattern[i]);
	}
	// 3awz a5rog mn CGRAm w aro7 ll DDRam 34an a write on LCD
	GOTOXY(Copy_u8XPOS, Copy_u8YPOS);
	// show data of each pattern on the lcd
	LCD_Send_Data(Copy_u8PatternNumber);
}

void LCD_printNumber(int num)
{
	u8 str[10];
	itoa(num, str, 10); // here 10 means decimal
	LCD_String(str);
}
void LCD_voidSendNumber(s32 Copy_s32Number)
{
	u32 Local_u32Counter = 1;
	u8 Local_u32Digit = 0;

	/* print (-) if the number is negative */
	if (Copy_s32Number < 0)
	{
		LCD_Send_Data('-');
		/* return the number to positive */
		Copy_s32Number = -Copy_s32Number;
	}
	if (Copy_s32Number == 0)
		LCD_Send_Data('0');
	else
	{
		while (Copy_s32Number / Local_u32Counter != 0)
		{
			Local_u32Counter *= 10;
		}
		Local_u32Counter /= 10;

		while (Local_u32Counter != 0)
		{
			Local_u32Digit = Copy_s32Number / Local_u32Counter;

			LCD_Send_Data(Local_u32Digit + '0');

			Copy_s32Number = Copy_s32Number % Local_u32Counter;

			Local_u32Counter /= 10;
		}
	}
}