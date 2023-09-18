#include "STDTYPE.h"
#include "UTILS.h"
#include "DIO_interface.h"
#include "TIMER1_interface.h"
#include "TIMER1_private.h"

void (*FUNC)(void);

void SERVO_TIMER1_mode14_Init(void)
{
	/*
 * select channel
 select OC1A
 * TO COMPARE MATCH
 fast pwm - mode 14
 * AND NON INVERTED PWM
 */
	CLR_BIT(TCCR1A, COM1A0);
	SET_BIT(TCCR1A, COM1A1);

	/*select wave generation mode 14*/
	CLR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1A, WGM11);
	SET_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1B, WGM12);

	/*PRESCALAR 8*/
	CLR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	/*COUNTS TOP*/ // ever count takes 1 micro seconds
	//>> now it calculates 20000 micro second as we need in the servo
	ICR1 = 19999;
	// now initialize the angle
	// zero at 1 milli -- 1000 micro -- means 999 in proteus its -90
	OCR1A = 999; // now we are at angle zero
}
void SERVO_Set_Angle(u8 angle)
{
	OCR1A = ((angle * 1000UL) / 180) + 999;
	/*UL means unsigned long -- it is a type of casting types
	//because if we get angle 180 the result will be so large,
	bigger than 32 bit of ocr ..so we shall cast it */
}

void SERVO_2_TIMER1_mode14_Init(void)
{
	/*
 * select channel
 select OC1A
 * TO COMPARE MATCH
 fast pwm - mode 14
 * AND NON INVERTED PWM
 */
	CLR_BIT(TCCR1A, COM1B0);
	SET_BIT(TCCR1A, COM1B1);

	/*select wave generation mode 14*/
	CLR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1A, WGM11);
	SET_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1B, WGM12);

	/*PRESCALAR 8*/
	CLR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	/*COUNTS TOP*/ // ever count takes 1 micro seconds
	//>> now it calculates 20000 micro second as we need in the servo
	ICR1 = 19999;
	// now initialize the angle
	// zero at 1 milli -- 1000 micro -- means 999 in proteus its -90
	OCR1B = 999; // now we are at angle zero
}
void SERVO_2_Set_Angle(u8 angle)
{
	OCR1B = ((angle * 1000UL) / 180) + 999;
	/*UL means unsigned long -- it is a type of casting types
	//because if we get angle 180 the result will be so large,
	bigger than 32 bit of ocr ..so we shall cast it */
}

// 16 bit
void TIMER1_Vid_Init(void)
{
	/*
	 * select channle
	 * TO COMPARE MATCH
	 * AND NON INVERTED PWM
	 */
	CLR_BIT(TCCR1A, COM1A0);
	SET_BIT(TCCR1A, COM1A1);

	/*select wave generation mode*/
	CLR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1A, WGM11);
	SET_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1B, WGM12);

	/*PRESCALAR 8*/
	CLR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	/*COUNTS TOP*/
	ICR1 = 2500;
}

void TIMER1_VidSetICR(u16 Top)
{
	ICR1 = Top;
}

void TIMER1_VidSetOCR(u16 cmp_match)
{
	OCR1A = cmp_match;
}

/*--------------------------------------------------*/

void TIMER1_Vid_ICUInit(void)
{
	/***SET PIN AS ICU AT PORTD PIN 6***/
	DIO_SetPinDirection(Port_D, Pin_6, input);

	/***SET PRESCALAR****/
	SET_BIT(TCCR1B, CS10);
	CLR_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);

	/**SELECT MODE AS NORMAL**/
	CLR_BIT(TCCR1A, WGM10);
	CLR_BIT(TCCR1A, WGM11);
	CLR_BIT(TCCR1B, WGM12);
	CLR_BIT(TCCR1B, WGM13);

	/***SET EDGE AS RISING EDGE***/
	SET_BIT(TCCR1B, ICES1);

	/***set INTERRUPT  enable icu****/
	SET_BIT(TIMSK, TICIE1);
}
void TIMER1_Vid_ICUResetTimer(void)
{
	TCNT1 = 0;
}
u16 TIMER1_U16_ICUGetValue(void)
{
	return ICR1;
}
void TIMER1_Vid_ICUSetEdge(u8 Copy_u8_set_edge)
{
	if (Copy_u8_set_edge == RISING_EDGE)
	{
		SET_BIT(TCCR1B, ICES1);
	}
	else if (Copy_u8_set_edge == FALLING_EDGE)
	{
		CLR_BIT(TCCR1B, ICES1);
	}
}
void TIMER1_Vid_DeInit(void)
{
	TCNT1 = 0;
	ICR1 = 0;
	CLR_BIT(TIMSK, TICIE1);
}
void ICU_CALL_BACK(void (*ICU_callback)(void))
{
	FUNC = ICU_callback;
}

// void __vector_6 (void)__attribute__((signal));
// void __vector_6 (void)
// {
// 	FUNC();
// }
