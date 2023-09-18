#ifndef TIMER1_INTERFACE_H_
#define TIMER1_INTERFACE_H_
#define RISING_EDGE  1
#define FALLING_EDGE 0

void SERVO_TIMER1_mode14_Init(void);
void SERVO_Set_Angle(u8 angle);
void TIMER1_Vid_Init(void);
void TIMER1_VidSetICR(u16 Top);
void TIMER1_VidSetOCR(u16 cmp_match);
void TIMER1_Vid_ICUInit(void);
void TIMER1_Vid_ICUResetTimer(void);
u16 TIMER1_U16_ICUGetValue(void);
void TIMER1_Vid_ICUSetEdge(u8 Copy_u8_set_edge);
void TIMER1_Vid_DeInit(void);
void SERVO_2_Set_Angle(u8 angle);
void SERVO_2_TIMER1_mode14_Init(void);
#endif