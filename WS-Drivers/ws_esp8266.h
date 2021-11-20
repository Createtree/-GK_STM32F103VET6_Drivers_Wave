/******************************************************************************
 * @file     				 ws_esp8266.c
 * @brief    				 esp8266 相关操作
 * @version  				 1.0.0
 * @creationTime     2020_06_23
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/

#ifndef _WS_ESP8266_H__
#define _WS_ESP8266_H__


#include "ws_main.h"



int WS_OS_Task_ESP8266_Link(void * date);

void WS_ESP8266_Receive_Callback(char *date , int len);


#endif


