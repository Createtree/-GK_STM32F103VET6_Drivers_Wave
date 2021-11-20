
#ifndef __W25Q32_DRV_H__
#define __W25Q32_DRV_H__


#include "ws_main.h"



#define W25Q32_SECTOR_SIZE	(4096)

 uint32_t W25Q32_CMD_Read_JEDEC_ID(void);

 void W25Q32_Data_Clear(uint32_t a);
 void W25Q32_Data_Read(void *pbuf,uint32_t l_byte,uint32_t a);
 void W25Q32_Data_Write(void *pbuf,uint32_t l_byte,uint32_t a);

 void W25Q32_CMD_EraseBlock64KB(uint32_t a);

 void W25Q32_Test(void);
 
 void W25Q32_TestID(void);



#endif //__W25Q32_DRV_H__



