#ifndef TIMER1_PRIVATE_H_
#define TIMER1_PRIVATE_H_
#define TCCR1A   *((volatile u8*)0x4F)

#define WGM10       0
#define WGM11       1
#define FOC1B       2
#define FOC1A       3
#define COM1B0      4                        /*******Bit 5:4 – COM1B1:0: Compare Output Mode for Compare unit B*******/
#define COM1B1      5                        /*******Bit 5:4 – COM1B1:0: Compare Output Mode for Compare unit B*******/
#define COM1A0      6                        /*******Bit 7:6 – COM1A1:0: Compare Output Mode for Compare unit A*******/
#define COM1A1      7                        /*******Bit 7:6 – COM1A1:0: Compare Output Mode for Compare unit A*******/


#define TCCR1B     *((volatile u8*)0x4E)

#define ICNC1        7
#define ICES1        6
/*******************bit 5 reserved**********/
#define WGM13        4
#define WGM12        3
#define CS12         2
#define CS11         1
#define CS10         0

#define TCNT1H       *((volatile u8*)0x4D)
#define TCNT1L       *((volatile u8*)0x4C)
#define TCNT1        *((volatile u16*)0x4C)

#define OCR1AH       *((volatile u8*)0x4B)
#define OCR1AL       *((volatile u8*)0x4A)
#define OCR1A        *((volatile u16*)0x4A)

#define OCR1BH       *((volatile u8*)0x49)
#define OCR1BL       *((volatile u8*)0x48)
#define OCR1B        *((volatile u16*)0x48)

#define ICR1H        *((volatile u8*)0x47)
#define ICR1L        *((volatile u8*)0x46)
#define ICR1         *((volatile u16*)0x46)



#define TIMSK        *((volatile u8*)0x59)
/******bits 7 6 1 0 are not used in this prephiral****/
#define TICIE1        5
#define OCIE1A        4
#define OCIE1B        3
#define TOIE1         2


#define TIFR         *((volatile u8*)0x58)
/******bits 7 6 1 0 are not used in this prephiral****/
#define ICF1          5
#define OCF1A         4
#define OCF1B         3
#define TOV1          2
#endif