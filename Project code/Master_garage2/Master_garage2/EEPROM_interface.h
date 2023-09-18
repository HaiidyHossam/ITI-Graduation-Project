#ifndef   __EEPROM_INTERFACE_H__
#define   __EEPROM_INTERFACE_H__
#include "STDTYPE.h"
void EEPROM_Write(u8 address,u8 value) ;
u8   EEPROM_READ (u8 address) ;
void EEPROM_WirtePage(u8 start_address,u8 *page_data) ;
void EEPROM_ReadPage(u8 start_address,u16 data_size,u8 *data) ;

#endif // __EEPROM_INTERFACE_H__
