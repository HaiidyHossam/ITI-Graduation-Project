#include "UTILS.h"
#include "STDTYPE.h"
#include "I2C_interface.h"
#include "I2C_config.h"
#include "I2C_private.h"

void I2C_VoidInitMaster(u8 master_address )
{
	/* frequency = 400kbs */
	TWBR = 2 ;
CLR_BIT(TWSR,TWSR_TWPS0) ;
CLR_BIT(TWSR,TWSR_TWPS1) ;
	if(master_address > 0 )
	{
		TWAR = master_address ;
	}
	SET_BIT(TWCR,TWCR_TWEN) ;
}
void I2C_VoidInitSlave(u8 master_address )
{
	if(master_address > 0 )
	{
		TWAR = master_address ;
	}
	SET_BIT(TWCR,TWCR_TWEN) ;
}
void I2C_VoidStartCondition(void)
{
	/* set start condition */
	SET_BIT(TWCR,TWCR_TWSTA) ;
	/* start job by clearing TWINT */
	SET_BIT(TWCR,TWCR_TWINT) ;
	/* then twint set by hardware when start condition end */
	while(GET_BIT(TWCR,TWCR_TWINT) == 0 ) ;
	/* Check if correction start */

	/* clear TWINT again */
	/* SET_PIN(TWCR,TWCR_TWINT) ; */

	/* check */
}
void I2C_VoidSetSlaveAddress_Operation(u8 slave_address,u8 operation)
{
	TWDR = ((slave_address<<1) | (operation)) ;
	/* clear start condition -> postponed  */
	   CLR_BIT(TWCR,TWCR_TWSTA) ;
    /* start job by clearing TWINT */
       SET_BIT(TWCR,TWCR_TWINT) ;
    /* then twint set by hardware when start condition end */
   	   while(GET_BIT(TWCR,TWCR_TWINT) == 0 ) ;
   	/* check */
}
void I2C_VoidMasterWriteData(u8 copy_U8data)
{
	TWDR = copy_U8data ;
    /* start job by clearing TWINT */
       SET_BIT(TWCR,TWCR_TWINT) ;
    /* then twint set by hardware when start condition end */
   	   while( GET_BIT(TWCR,TWCR_TWINT) == 0 ) ;
   	   /* check */
}
void I2C_VoidMasterReadData(u8 *copy_U8destination)
{
    /* start job by clearing TWINT */
       SET_BIT(TWCR,TWCR_TWINT) ;
    /* then twint set by hardware when start condition end */
   	   while(GET_BIT(TWCR,TWCR_TWINT) == 0 ) ;
   	   *copy_U8destination = TWDR ;
}
void I2C_VoidMasterStopCondition(void)
{
	 SET_BIT(TWCR,TWCR_TWSTO) ;
    /* start job by clearing TWINT */
       SET_BIT(TWCR,TWCR_TWINT) ;
       /* NO need for  while(Read_PinValue(TWCR,TWCR_TWINT) == 0 ) ; */
}
