/******************************************************************************
 * @file     				 ws_user_task.c
 * @brief    				 �û����� 
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.  
 *				2. 
          3.
*******************************************************************************/

#include "ws_user_task.h"

//     MP3����״̬��ֵ�ź�
extern osSemaphoreId WS_Audio_Idle_SemaphoreHandle;

//     ������
extern   osThreadId WS_FreeTh_UTHandle;

//  ���̿��ƽṹ
extern  WS_StepControlTypeDef    hsctr1;
extern  WS_StepControlTypeDef    hsctr2;

//   ϵͳ������Ϣ����
extern  WS_ConfigInfoTypeDef   systemConfigInfo ;


/* USER CODE BEGIN Header_WS_Usar_Main_Thread */
/**
* @brief Function implementing the WS_Usar_Main thread.
* @param argument: 
* @retval None
*/
void WS_FreeThread_UserTask(void const * argument)
{
  /* USER CODE BEGIN WS_FreeThread_UserTask */
  /* Infinite loop */
	
	uint16_t  stepMain = 0                  ;

	WS_StepControlTypeDef  *hsctr = &hsctr1 ;
	
	

	osDelay(2000);
	
	//  ��ʼ�����̿��ƽṹ
	hsctr1.p_stepConfig = &systemConfigInfo.hstep1;
	hsctr1.audioFilepath= 1;
	
	hsctr->p_stepConfig->realy1DelayTrigger = 2;
	hsctr->p_stepConfig->realy1TriggerTime  = 5;
	
	hsctr->p_stepConfig->realy2DelayTrigger = 5;
	hsctr->p_stepConfig->realy2TriggerTime  = 3;
	
//	hsctr->p_stepConfig->realy2DelayTrigger = -2;
//	hsctr->p_stepConfig->resetMode = -1 ; 
	
	WS_Audio_Wait_Idle(360000);
	WS_Audio_Open_Blocking(WS_Audio_System_Filepath ,101)  ;  //  ����1������
	WS_Audio_Open_Blocking(WS_Audio_System_Filepath,hsctr1.p_stepConfig->TriggerMode);
	
	//  ��ʱ����
	osDelay(hsctr->p_stepConfig->delayRun*1000);
  for(;;)
  {
		 switch(stepMain)
		 {
			 //  ��ʼ��
			 case 0:
				       WS_Audio_Open_Blocking(hsctr->audioFilepath,101)  ;  //  ���������ļ�
							 if(hsctr->p_stepConfig->realy1DelayTrigger >= -1)
							 {
									WS_define_Borad_Close_Relay1 ; 
							 }
							 if(hsctr->p_stepConfig->realy2DelayTrigger >= -1)
							 {
									WS_define_Borad_Close_Relay2 ; 
							 }
							 stepMain ++ ;
				       //break;
			 
		   case 1:
				     
               WS_Step_Input_Mode_Blocking(hsctr) ;
			         stepMain ++ ;
							 break;
							 
			 case 2:
				       // �̵������
			        if(hsctr->p_stepConfig->realy1DelayTrigger >= 0)   //  ��ʱ��
							{
								  
								  if(hsctr->p_stepConfig->realy1DelayTrigger > 0)
							       WS_Borad_Open_Relay_Time(1,1,hsctr->p_stepConfig->realy1DelayTrigger) ;
							    else
									   WS_Borad_Open_Relay_Time(1,0,0) ;
									
								  if(hsctr->p_stepConfig->realy1TriggerTime > 0)  // ��ʱ��
									{
									   WS_Borad_Open_Relay_Time(1,3,hsctr->p_stepConfig->realy1TriggerTime+hsctr->p_stepConfig->realy1DelayTrigger);  
									}
							}
							if(hsctr->p_stepConfig->realy2DelayTrigger >= 0)   //  ��ʱ��
							{
							    if(hsctr->p_stepConfig->realy2DelayTrigger > 0)
							       WS_Borad_Open_Relay_Time(2,1,hsctr->p_stepConfig->realy2DelayTrigger) ;
							    else
									   WS_Borad_Open_Relay_Time(2,0,0) ;
							    if(hsctr->p_stepConfig->realy2TriggerTime > 0)  // ��ʱ��
									{
									   WS_Borad_Open_Relay_Time(2,3,hsctr->p_stepConfig->realy2TriggerTime+hsctr->p_stepConfig->realy2DelayTrigger);  
									}
							}
							//   ��������
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,201);
							if(hsctr->p_stepConfig->realy1DelayTrigger == -1)
							{
							   WS_Borad_Open_Relay_Time(1,0,hsctr->p_stepConfig->realy1TriggerTime);  
							}
							//   ��������
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,202);
							if(hsctr->p_stepConfig->realy2DelayTrigger == -1)
							{
							   WS_Borad_Open_Relay_Time(2,0,hsctr->p_stepConfig->realy2TriggerTime);  
							}
							//   ��������
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,203);
							
							//   ��λģʽ���
							if(hsctr->p_stepConfig->resetMode >= 0)
							{
							   osDelay(hsctr->p_stepConfig->resetMode*1000);
								 stepMain = 0;
								 WS_Borad_Open_Buzzer_Time(50);
							}
							else
							{
							   vTaskSuspend(WS_FreeTh_UTHandle);
								 stepMain = 0;
							}
              break;				 
							 
		 }
     osDelay(10);
  }
  /* USER CODE END WS_FreeThread_UserTask */
}




