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

#include "ws_Outborad.h"


//      输出输出数据结构外部声明
extern  IOBoradBufferTypeDef    IOBoradBuffer ;



/*
函数名称char WS_Outborad_Set_Port_Stu
函数功能：设置输出板端口的输出状态
输入参数: n： 端口号 ； stu: 状态（0：通电；1：断电；2：取反）
返回值  ：无  
*/
void   WS_Outborad_Set_Port_Stu(uint16_t n , uint16_t stu )
{   
	     if(n < 1 || n > 64)   return ;
	
	     n --;
	     n = 63-n;
	
	     switch(stu)
			 {
				 //   吸合
				 case 0:
					       IOBoradBuffer.outputBoradTransmitBuffer[n/8]  |=  (1<<(7-n%8))  ;
					       break;  
				 
				 //   释放
				 case 1:
					       IOBoradBuffer.outputBoradTransmitBuffer[n/8]  &= ~(1<<(7-n%8))  ;
					       break;
				 
				 //  取反
				 case 2:
					       IOBoradBuffer.outputBoradTransmitBuffer[n/8]  ^=  (1<<(7-n%8))  ;
					       break;
			 }				 
}



/* USER CODE BEGIN Header_WS_Boread_IN_OUT_Thread */
/**
* @brief Function implementing the WS_IN_OUT_Name thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WS_Boread_IN_OUT_Thread */
void WS_Boread_IN_OUT_Thread(void const * argument)
{
  /* USER CODE BEGIN WS_Boread_IN_OUT_Thread */
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END WS_Boread_IN_OUT_Thread */
}



