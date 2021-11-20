
/******************************************************************************
 * @file     				 ws_borad_base.c
 * @brief    				 ����оƬ�ڲ�����Χ��Դ����
 * @version  				 1.0.0
 * @creationTime     2020_08_15
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_BORAD_INPUT_H__
#define _WS_BORAD_INPUT_H__


#include "ws_main.h"

//   ��ȡ�Դ�������½���
uint16_t WS_Borad_Get_Port_Down(void);
uint16_t WS_Borad_Get_Port_Up(void);

//   ���12��ң�ذ��µİ����½��أ�ÿ����һ�η���һ��
char getWxKey12Down(void)            ;

//   �õ�IC����ţ��������IC��Ӧ��֧��20�ſ�Ƭ��ݸ�Ӧ
char WS_Borad_IC_Number_Input (char index);


void  WS_OS_Task_System_Input_Check(void);





#endif
