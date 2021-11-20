
/******************************************************************************
 * @file     				 ws_stm32_drive_borad.c
 * @brief    				 WS_BUS通信协议相关处理
 * @version  				 1.0.0
 * @creationTime     2020_03_18
 * @creator					 
 * @readme
 *        1. 数据帧的编排  
 *				2. 数据帧解析
          3. 
*******************************************************************************/


#ifndef _WS_BUS_H__
#define _WS_BUS_H__

#include "ws_main.h"

//  总线控制结构体
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


