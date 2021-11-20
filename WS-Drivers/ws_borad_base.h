
/******************************************************************************
 * @file     				 ws_borad_base.c
 * @brief    				 处理芯片内部及外围资源驱动
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_STM32_DRIVE_BORAD_H__
#define _WS_STM32_DRIVE_BORAD_H__


#include "ws_main.h"


// 全局声明 
extern uint16_t  touchOriginal_X ;
extern uint16_t  touchOriginal_Y ;

 
void WS_UART_Start_DMA_Rx(void);

//   初始化板子
void WS_Borad_Init(void);

//   打开调试灯灯时间
char WS_Borad_Open_DebugLed_Time(int time);
//   打开运行灯时间
char WS_Borad_Open_RunLed_Time(int time)  ;
//   打开蜂鸣器
char WS_Borad_Open_Buzzer_Time(int time)  ;
//   操控板载继电器
void WS_Borad_Open_Relay_Time(int n, int stu ,   float time);

//   Audio模块相关操作
void WS_Audio_Open(unsigned char dir , unsigned char file);
void WS_Audio_Open_Blocking(unsigned char dir , unsigned char file);
void WS_Audio_Set_Volume(char vol);
void WS_Audio_Start(void);
void WS_Audio_Stop(void);
void WS_Audio_Open_Number(int num);  
void WS_Audio_Wait_Idle(int xBlockTime);



//   串口发送字符串，DMA传输
HAL_StatusTypeDef WS_UART_Transmit_String(UART_HandleTypeDef *huart, char *p_string);
//   串口发送数据，DMA传输
HAL_StatusTypeDef WS_UART_Transmit_Byte(UART_HandleTypeDef *huart, char *BuffAddr , int size);

//   RS485总线发送数据
HAL_StatusTypeDef WS_RS485_Transmit_String(char *BuffAddr);
HAL_StatusTypeDef WS_RS485_Transmit_Byte(uint8_t * BuffAddr , int size);

//    
HAL_StatusTypeDef WS_Bus_Transmit_Byte(char *BuffAddr , int len);
  
//    串行数据更新
void  WS_Borad_Update_Input_Output(void)  ; 



#endif

/*********************************************************************************************************
                                             END FILE           
*********************************************************************************************************/



