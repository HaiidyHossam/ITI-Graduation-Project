#ifndef _LCD_INTERFACE_H_
#define _LCD_INTERFACE_H_
void LCD_Send_Command (u8 command);
void LCD_Send_Data (u8 Data);
void LCD_Init(void);
void GOTOXY(u8 x,u8 y);
void LCD_Clear();
void LCD_String (char *str)	;
void CLCD_VidWriteSpecialCharacter(u8 *Copy_pu8Pattern, u8 Copy_u8PatternNumber,u8 Copy_u8XPOS,u8 Copy_u8YPOS);
void LCD_printNumber(int num);

void LCD_voidSendNumber(s32 Copy_s32Number);

#endif