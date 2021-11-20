/******************************************************************************
 * @file     				 ws_Outborad.c
 * @brief    				 75hc595扩展输出板驱动 
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.75hc595扩展输出板驱动   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_INBORAD_H__
#define _WS_INBORAD_H__

#include "ws_main.h"


uint16_t   WS_Inborad_Get_Port_Down(void);
uint16_t   WS_Inborad_Get_Port_Up(void);



uint16_t   WS_Inborad_Get_Port_Pin_Stu(uint16_t n)  ;
uint64_t   WS_Inborad_Get_Port_All_Stu(void)        ;
uint64_t   WS_Inborad_Get_Port_All_NOTStu(void)     ;


void       WS_Inborad_Set_Port_Stu(uint16_t n , uint16_t stu ) ;



#endif

