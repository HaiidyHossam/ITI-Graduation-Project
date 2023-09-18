#include "EEPROM_interface.h"
#include "EEPROM_config.h"
#include "I2C_interface.h"
#include "STDTYPE.h"
#include <util/delay.h>
void EEPROM_Write(u8 address,u8 value)
{
	I2C_VoidStartCondition() ;
	I2C_VoidSetSlaveAddress_Operation(EEPROM_ADDRESS,TWI_WRITE) ;
	I2C_VoidMasterWriteData(address) ;
	I2C_VoidMasterWriteData(value) ;
	I2C_VoidMasterStopCondition() ;
}
u8  EEPROM_READ (u8 address)
{
	u8 ret = 0 ;
	I2C_VoidStartCondition() ;
	I2C_VoidSetSlaveAddress_Operation(EEPROM_ADDRESS,TWI_WRITE) ;
	I2C_VoidMasterWriteData(address) ;
	I2C_VoidStartCondition() ;
	I2C_VoidSetSlaveAddress_Operation(EEPROM_ADDRESS,TWI_READ) ;
	I2C_VoidMasterReadData(&ret) ;
	I2C_VoidMasterStopCondition() ;
	return ret ;
}
void EEPROM_WirtePage(u8 start_address,u8 *page_data)
{
	u8 page_size = 0 , i  ;
	for(page_size = 0;page_data[page_size] != '\0';page_size ++ ) ;
	for(i = 0 ; i<=page_size;i++ , start_address ++)
	{
		EEPROM_Write(start_address,page_data[i]) ;
		_delay_ms(200);
	}
}
void EEPROM_ReadPage(u8 start_address,u16 data_size,u8 *data)
{
	u8 data_rx[data_size+1] , i  ;
	for( i = 0;i<data_size;i++,start_address++)
	{
		data_rx[i] = EEPROM_READ(start_address) ;
	}
	data_rx[i] = '\0' ;
	for(i=0;data_rx[i] != '\0' ;i++)
	{
		data[i] = data_rx[i] ;
	}
	data[i] = '\0' ;
}
