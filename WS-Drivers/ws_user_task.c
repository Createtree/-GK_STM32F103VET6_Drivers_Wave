/******************************************************************************
 * @file     				 ws_user_task.c
 * @brief    				 用户任务 
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.  
 *				2. 
          3.
*******************************************************************************/

#include "ws_user_task.h"

//     MP3工作状态二值信号
extern osSemaphoreId WS_Audio_Idle_SemaphoreHandle;

//     任务句柄
extern   osThreadId WS_FreeTh_UTHandle;

//  流程控制结构
extern  WS_StepControlTypeDef    hsctr1;
extern  WS_StepControlTypeDef    hsctr2;

//   系统配置信息定义
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
	
	//  初始化流程控制结构
	hsctr1.p_stepConfig = &systemConfigInfo.hstep1;
	hsctr1.audioFilepath= 1;
	
	hsctr->p_stepConfig->realy1DelayTrigger = 2;
	hsctr->p_stepConfig->realy1TriggerTime  = 5;
	
	hsctr->p_stepConfig->realy2DelayTrigger = 5;
	hsctr->p_stepConfig->realy2TriggerTime  = 3;
	
//	hsctr->p_stepConfig->realy2DelayTrigger = -2;
//	hsctr->p_stepConfig->resetMode = -1 ; 
	
	WS_Audio_Wait_Idle(360000);
	WS_Audio_Open_Blocking(WS_Audio_System_Filepath ,101)  ;  //  流程1工作于
	WS_Audio_Open_Blocking(WS_Audio_System_Filepath,hsctr1.p_stepConfig->TriggerMode);
	
	//  延时启动
	osDelay(hsctr->p_stepConfig->delayRun*1000);
  for(;;)
  {
		 switch(stepMain)
		 {
			 //  初始化
			 case 0:
				       WS_Audio_Open_Blocking(hsctr->audioFilepath,101)  ;  //  流程启动文件
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
				       // 继电器输出
			        if(hsctr->p_stepConfig->realy1DelayTrigger >= 0)   //  延时打开
							{
								  
								  if(hsctr->p_stepConfig->realy1DelayTrigger > 0)
							       WS_Borad_Open_Relay_Time(1,1,hsctr->p_stepConfig->realy1DelayTrigger) ;
							    else
									   WS_Borad_Open_Relay_Time(1,0,0) ;
									
								  if(hsctr->p_stepConfig->realy1TriggerTime > 0)  // 打开时间
									{
									   WS_Borad_Open_Relay_Time(1,3,hsctr->p_stepConfig->realy1TriggerTime+hsctr->p_stepConfig->realy1DelayTrigger);  
									}
							}
							if(hsctr->p_stepConfig->realy2DelayTrigger >= 0)   //  延时打开
							{
							    if(hsctr->p_stepConfig->realy2DelayTrigger > 0)
							       WS_Borad_Open_Relay_Time(2,1,hsctr->p_stepConfig->realy2DelayTrigger) ;
							    else
									   WS_Borad_Open_Relay_Time(2,0,0) ;
							    if(hsctr->p_stepConfig->realy2TriggerTime > 0)  // 打开时间
									{
									   WS_Borad_Open_Relay_Time(2,3,hsctr->p_stepConfig->realy2TriggerTime+hsctr->p_stepConfig->realy2DelayTrigger);  
									}
							}
							//   阻塞播放
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,201);
							if(hsctr->p_stepConfig->realy1DelayTrigger == -1)
							{
							   WS_Borad_Open_Relay_Time(1,0,hsctr->p_stepConfig->realy1TriggerTime);  
							}
							//   阻塞播放
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,202);
							if(hsctr->p_stepConfig->realy2DelayTrigger == -1)
							{
							   WS_Borad_Open_Relay_Time(2,0,hsctr->p_stepConfig->realy2TriggerTime);  
							}
							//   阻塞播放
				      WS_Audio_Open_Blocking(hsctr->audioFilepath,203);
							
							//   复位模式检测
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




