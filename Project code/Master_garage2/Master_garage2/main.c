/*
 * SMART_Parking.c
 *this project is about a parking system 
 The admin will use the UART for entering the IDS of the users who has the right to enter the garage.
 then , the user will enter his personal id in the UART VT and if it is already stored by the admin , a green led will be high
 and a motor will open to open the door and the pir sensor will read 1 if the car entered and then it will close the motor of the door
 and SPI will send the id to another Micro "slave" to print on LCD the id of The user .
 if the user enterd wrong id , a red led will be high 
 if the user wants to go out from the garage , he will enter id to another door using UART VT
  and if it is right, the door will be open and so on..
  
  
 * Created: 9/9/2023 6:42:07 PM
 * Author : Haiidy Hossam
 */
#define F_CPU 8000000
#include <util/delay.h>
#include "STDTYPE.h"
#include "UTILS.h"
#include "DIO_interface.h"
#include "LCD_Interface.h"
#include "LCD_Config.h"
#include "UART_interface.h"
#include "I2C_interface.h"
#include "EEPROM_config.h"
#include "EEPROM_interface.h"
#include "SPI_interface.h"
#include "TIMER1_interface.h"

u8 time[3];         // To store current time (hours, minutes, seconds)
u8 uart_data;       // Data received via UART
u8 input_id;        // User input ID
u8 flag_enter = 0;  // Flag to indicate successful entry
u8 num_slot = 3;    // Number of available parking slots
u8 driverIsin[3];   // Array to store IDs of drivers in slots
u8 D_ID[3];         // Array to store admin-entered IDs

typedef enum
{
	EXIT,
	ENTER,
	X_E
} Status;            // Enum to represent different states

void GetData(u8 State)
{
	input_id = UART_RECEIVE();  // Receive user input ID via UART
	UART_SEND(input_id);        // Send the received ID back via UART for confirmation
	flag_enter = 0;            // Reset the entry flag

	if (State == ENTER)
	{
		// Check if the input ID matches any stored IDs and is not already inside
		for (u8 i = 0; i < 3; i++)
		{
			if ((input_id == EEPROM_READ(i)) && (input_id != driverIsin[i]))
			{
				driverIsin[i] = input_id; // Store the ID of the driver in the slot
				flag_enter = 1;          // Set the entry flag to indicate success
				break;
			}
		}
	}
	else if (State == EXIT)
	{
		// Check if the input ID matches a driver ID inside
		for (u8 i = 0; i < 3; i++)
		{
			if (input_id == driverIsin[i])
			{
				flag_enter = 1;          // Set the entry flag to indicate success
				driverIsin[i] = 0;       // Clear the ID from the slot
				break;
			}
		}
	}
	else if (State == X_E)
	{
		// Display available slots and instructions on the LCD
		LCD_Clear();
		LCD_String(" available: ");
		LCD_printNumber(num_slot);
		GOTOXY(1, 0);
		LCD_String("N->Enter.X->Exit");

		// Receive user choice (N for Enter, X for Exit)
		uart_data = UART_RECEIVE();
		UART_SEND(uart_data);
	}
}

void G_init()
{
	// Configure LCD
	DIO_SetPortDirection(Data_Port, Port_output);
	DIO_SetPinDirection(CTRL_PORT, RS_Pin, output);
	DIO_SetPinDirection(CTRL_PORT, RW_Pin, output);
	DIO_SetPinDirection(CTRL_PORT, Enable_Pin, output);
	LCD_Init();

	/* EEPROM on port C pin 0 & pin 1 */
	// Initialize I2C for EEPROM communication
	I2C_VoidInitMaster(0);

	/* Two pins for the red LED and the green LED */
	// Configure the green LED as an output (Active High)
	DIO_SetPinDirection(Port_D, Pin_6, output);
	// Configure the red LED as an output (Active High)
	DIO_SetPinDirection(Port_D, Pin_7, output);

	/* Pins for the PIR Sensor */
	// Configure PIR Sensor pins as inputs
	DIO_SetPinDirection(Port_D, Pin_2, input);
	DIO_SetPinDirection(Port_D, Pin_3, input);

	/* Two Servo motors */
	// Initialize Timer1 for Servo control (you may have custom functions for this)
	SERVO_TIMER1_mode14_Init();
	SERVO_2_TIMER1_mode14_Init();

	/* SPI Initialization */
	SPI_INIT_MASTER();

	// Configure the motor for entering the system
	DIO_SetPinDirection(Port_D, Pin_4, output);
	// Configure the motor for exiting the system
	DIO_SetPinDirection(Port_D, Pin_5, output);

	/* UART pins */
	// Configure RX pin as an input
	DIO_SetPinDirection(Port_D, Pin_0, input);
	// Configure TX pin as an output
	DIO_SetPinDirection(Port_D, Pin_1, output);

	// Initialize UART communication
	UART_INIT();


	// Display "Admin" on the LCD
	LCD_String("Admin");
	GOTOXY(1, 0);
	LCD_String("Enter IDS : ");

	// Receive and store admin-entered IDs
	for (u8 i = 0; i < 3; i++)
	{
		D_ID[i] = UART_RECEIVE();
		UART_SEND(D_ID[i]);
		LCD_Send_Data(D_ID[i]);
		EEPROM_Write(i, D_ID[i]);
	}

	// Clear the LCD
	LCD_Clear();
}


void WrongID()
{
	// Display "INVALIDE ID" on the LCD
	LCD_Clear();
	LCD_String("INVALIDE ID.");

	// Turn on the Red LED (Indicating an invalid ID)
	DIO_SetPinValue(Port_D, Pin_7, 1);

	GOTOXY(1, 0);
	LCD_String("Available: ");
	LCD_printNumber(num_slot);

	// Delay for a short time to show the message
	_delay_ms(100);

	// Turn off the Red LED
	DIO_SetPinValue(Port_D, Pin_7, 0);
}

void RightID(u8 State)
{
	
	// Display "Right ID @ HH:MM:SS" on the LCD
	LCD_Clear();
	LCD_String("Right ID @");

	// Turn on the Green LED (Indicating a valid ID)
	DIO_SetPinValue(Port_D, Pin_6, 1);

	if (State == EXIT)
	{
		num_slot++;

		// Open the exit door (you may have custom functions for this)
		SERVO_2_Set_Angle(90);
		_delay_ms(100);

		// Wait for the PIR sensor to detect the car leaving
		while (DIO_GetPinValue(Port_D, Pin_3) == 0);

		// Turn off the Green LED
		DIO_SetPinValue(Port_D, Pin_6, 0);

		// Close the exit door (you may have custom functions for this)
		SERVO_2_Set_Angle(0);

		// Send the exit signal to the slave via SPI
		SPI_TR_RC('x');
		// Send the user's ID to the slave via SPI
		SPI_TR_RC(input_id);
	}
	else if (State == ENTER)
	{
		num_slot--;

		// Open the entrance door (you may have custom functions for this)
		SERVO_Set_Angle(90);
		_delay_ms(100);

		// Wait for the PIR sensor to detect the car entering
		while (DIO_GetPinValue(Port_D, Pin_2) == 0);

		// Turn off the Green LED
		DIO_SetPinValue(Port_D, Pin_6, 0);

		// Close the entrance door (you may have custom functions for this)
		SERVO_Set_Angle(0);

		// Send the enter signal to the slave via SPI
		SPI_TR_RC('n');
		// Send the user's ID to the slave via SPI
		SPI_TR_RC(input_id);
	}

	GOTOXY(1, 0);
	LCD_String("Available: ");
	LCD_printNumber(num_slot);

	// Delay for a short time to show the message
	_delay_ms(100);


	
}

int main(void)
{
	G_init();  // Initialize the system

	while (1)
	{

		GetData(X_E);  // Receive user input and state (Enter/Exit) via UART

		switch (uart_data)
		{
			case 'n':
			case 'N':
			if (num_slot == 0)
			{
				// Display "FULL, SORRY" if no available slots
				LCD_Clear();
				LCD_String(" FULL, SORRY");
				_delay_ms(200);
				continue;
			}
			GetData(ENTER);
			if (flag_enter == 1)
			{
				RightID(ENTER);
				if (num_slot > 0)
				continue;
			}
			else if (flag_enter == 0)
			{
				WrongID();
				if (num_slot > 0)
				continue;
			}
			break;
			case 'x':
			case 'X':
			GetData(EXIT);
			if (flag_enter == 1)
			{
				RightID(EXIT);
				if (num_slot > 0)
				continue;
			}
			else if (flag_enter == 0)
			{
				WrongID();
				if (num_slot > 0)
				continue;
			}
			break;
		}
	}
}
