/*****************************
编写核心代码，尽量做到不依赖硬件



*****************************/


#ifndef __CORE_H__
#define __CORE_H__

#include "ws_main.h"

#define DEBUG_EN 1


#define STOP_CHAR ','
#define STEP_STRING_MAX_LEN 1024

#define  WS_Status_True		    ( 0 )
#define  WS_Status_False	    ( 1 )




//   格式化可变变量字符串
char *WS_Format(char *p,...);

//   调试输出
void WS_Debug(int wait , char *p,...);


void WS_Delay_US(int z);
void WS_Delay_MS(int z);

//#define WS_Delay_MS HAL_Delay

//  系统延时 , 
#define WS_Sys_Delay_MS osDelay


char WS_Find_Index_String(char *p_stepString , int index, char *p_RxBuf);







#endif



