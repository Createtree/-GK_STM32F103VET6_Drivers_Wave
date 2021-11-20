
/******************************************************************************
 * @file     				 ws_borad_base.c
 * @brief    				 ����оƬ�ڲ�����Χ��Դ����
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_STM32_DRIVE_BORAD_H__
#define _WS_STM32_DRIVE_BORAD_H__


#include "ws_main.h"


// ȫ������ 
extern uint16_t  touchOriginal_X ;
extern uint16_t  touchOriginal_Y ;

 
void WS_UART_Start_DMA_Rx(void);

//   ��ʼ������
void WS_Borad_Init(void);

//   �򿪵��ԵƵ�ʱ��
char WS_Borad_Open_DebugLed_Time(int time);
//   �����е�ʱ��
char WS_Borad_Open_RunLed_Time(int time)  ;
//   �򿪷�����
char WS_Borad_Open_Buzzer_Time(int time)  ;
//   �ٿذ��ؼ̵���
void WS_Borad_Open_Relay_Time(int n, int stu ,   float time);

//   Audioģ����ز���
void WS_Audio_Open(unsigned char dir , unsigned char file);
void WS_Audio_Open_Blocking(unsigned char dir , unsigned char file);
void WS_Audio_Set_Volume(char vol);
void WS_Audio_Start(void);
void WS_Audio_Stop(void);
void WS_Audio_Open_Number(int num);  
void WS_Audio_Wait_Idle(int xBlockTime);



//   ���ڷ����ַ�����DMA����
HAL_StatusTypeDef WS_UART_Transmit_String(UART_HandleTypeDef *huart, char *p_string);
//   ���ڷ������ݣ�DMA����
HAL_StatusTypeDef WS_UART_Transmit_Byte(UART_HandleTypeDef *huart, char *BuffAddr , int size);

//   RS485���߷�������
HAL_StatusTypeDef WS_RS485_Transmit_String(char *BuffAddr);
HAL_StatusTypeDef WS_RS485_Transmit_Byte(uint8_t * BuffAddr , int size);

//    
HAL_StatusTypeDef WS_Bus_Transmit_Byte(char *BuffAddr , int len);
  
//    �������ݸ���
void  WS_Borad_Update_Input_Output(void)  ; 



#endif

/*********************************************************************************************************
                                             END FILE           
*********************************************************************************************************/



