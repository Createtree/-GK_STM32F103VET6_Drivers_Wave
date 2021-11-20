/******************************************************************************
 * @file     				 ws_Outborad.c
 * @brief    				 75hc595��չ��������� 
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.75hc595��չ���������   
 *				2. 
          3.
*******************************************************************************/

#include "ws_Outborad.h"


//      ���������ݽṹ�ⲿ����
extern  IOBoradBufferTypeDef    IOBoradBuffer ;



/*
�������ƣvchar WS_Outborad_Set_Port_Stu
�������ܣ����������˿ڵ����״̬
�������: n�� �˿ں� �� stu: ״̬��0��ͨ�磻1���ϵ磻2��ȡ����
����ֵ  ����  
*/
void   WS_Outborad_Set_Port_Stu(uint16_t n , uint16_t stu )
{   
	     if(n < 1 || n > 64)   return ;
	
	     n --;
	     n = 63-n;
	
	     switch(stu)
			 {
				 //   ����
				 case 0:
					       IOBoradBuffer.outputBoradTransmitBuffer[n/8]  |=  (1<<(7-n%8))  ;
					       break;  
				 
				 //   �ͷ�
				 case 1:
					       IOBoradBuffer.outputBoradTransmitBuffer[n/8]  &= ~(1<<(7-n%8))  ;
					       break;
				 
				 //  ȡ��
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



