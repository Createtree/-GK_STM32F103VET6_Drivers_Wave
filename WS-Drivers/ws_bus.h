
/******************************************************************************
 * @file     				 ws_stm32_drive_borad.c
 * @brief    				 WS_BUSͨ��Э����ش���
 * @version  				 1.0.0
 * @creationTime     2020_03_18
 * @creator					 
 * @readme
 *        1. ����֡�ı���  
 *				2. ����֡����
          3. 
*******************************************************************************/


#ifndef _WS_BUS_H__
#define _WS_BUS_H__

#include "ws_main.h"

//  ���߿��ƽṹ��
typedef struct __WS_BusTypeDef
{
  unsigned short     int   addr; 
	unsigned short     int   cmd;
  unsigned short     int   len;
	unsigned short     int   crc;
	char                     *pBuffPtr;
} WS_BusTypeDef;


HAL_StatusTypeDef WS_BUS_Transmit_Command(uint16_t addr,uint16_t cmd , uint16_t  len ,char *date );
HAL_StatusTypeDef WS_BUS_Transmit_Command_String(uint16_t addr,uint16_t cmd , char *date );


HAL_StatusTypeDef WS_BUS_Parser_Command(WS_BusTypeDef *hbus,char *date ,int size);
HAL_StatusTypeDef WS_BUS_Parser_Command_String(WS_BusTypeDef *hbus,char *date, int size);

void   WS_BUS_Run_Command(WS_BusTypeDef *hbus);
#endif


