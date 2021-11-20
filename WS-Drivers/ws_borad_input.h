
/******************************************************************************
 * @file     				 ws_borad_base.c
 * @brief    				 处理芯片内部及外围资源驱动
 * @version  				 1.0.0
 * @creationTime     2020_08_15
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_BORAD_INPUT_H__
#define _WS_BORAD_INPUT_H__


#include "ws_main.h"

//   获取自带输入口下降沿
uint16_t WS_Borad_Get_Port_Down(void);
uint16_t WS_Borad_Get_Port_Up(void);

//   获得12键遥控按下的按键下降沿，每按下一次返回一次
char getWxKey12Down(void)            ;

//   得到IC卡编号，用于身份IC感应，支持20张卡片身份感应
char WS_Borad_IC_Number_Input (char index);


void  WS_OS_Task_System_Input_Check(void);





#endif
