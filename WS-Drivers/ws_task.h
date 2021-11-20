/******************************************************************************
 * @file     				 ws_task.c
 * @brief    				 ws程序框架下的任务管理
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_TASK_H__
#define _WS_TASK_H__

#include "ws_main.h"
#include "wareform.h"
//   任务ID定义
#define WS_OS_ID_Runled_define 1
#define WS_OS_ID_Test_define   2
#define WS_OS_ID_Keyborad_define    3
#define WS_OS_ID_Touch_define       4

//  触发后的任务
#define triggerTaskId 10

//typedef struct node_Struct_Typedef{
//	uint16_t ID;
//	uint16_t data;
//	
//	struct node_Struct_Typedef *next;
//	
//}NODE;
void is_rectangle(uint16_t i);

void WS_OS_Task_Create(void) ;
void WS_Timer_10MS_Callback(void);
//void Wave_Analysis(void);
//void Wave_Find_Vm(void);
//void WS_TIM3_IT_Callback(void);
#endif

