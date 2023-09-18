#ifndef __I2C_INTERFACE_H__
#define __I2C_INTERFACE_H__
#include "STDTYPE.h"

#define TWI_WRITE 0
#define TWI_READ  1
void I2C_VoidInitMaster(u8 master_address ) ;
void I2C_VoidInitSlave(u8 master_address ) ;
void I2C_VoidStartCondition(void) ;
void I2C_VoidSetSlaveAddress_Operation(u8 slave_address,u8 operation) ;
void I2C_VoidMasterWriteData(u8 copy_U8data) ;
void I2C_VoidMasterReadData(u8 *copy_U8destination) ;
void I2C_VoidMasterStopCondition(void) ;

#endif//__I2C_INTERFACE_H__
