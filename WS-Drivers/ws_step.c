/******************************************************************************
 * @file     				 ws_step.c
 * @brief    				 �������̻�����ش���
 * @version  				 1.0.0
 * @creationTime     2020_05_1
 * @creator					 
 * @readme
 *        
*******************************************************************************/

#include "ws_step.h"



extern  WS_ConfigInfoTypeDef   systemConfigInfo;

//  ���̿��ƽṹ
WS_StepControlTypeDef    hsctr1;
WS_StepControlTypeDef    hsctr2;


//  ����1�Զ���λ��־
char   step1AutoResetFlag = 0;
//  ����2�Զ���λ��־
char   step2AutoResetFlag = 0;

int    WS_Step1_AutoResetFlag_Timer(void * date)
{
    date = date ;
	  step1AutoResetFlag = 1;
	  return 0;
}

int    WS_Step2_AutoResetFlag_Timer(void * date)
{
    date = date ;
	  step2AutoResetFlag = 1;
	  return 0;
}


/******************************************************************************
	*�������ƣ�  WS_Step_Task_Step1
	*���ܣ�      �û�����1������

*******************************************************************************/
int WS_Step_Task_Step1(void * date)
{
    static   uint16_t  stepMain = 0, stepSub = 0     ; 
	  static   WS_StepControlTypeDef  *hsctr = &hsctr1 ;
	  int      stu = WS_Step_Input_ERR ;
	  date = date  ;
  
	  hsctr->p_sysin = (WS_SysInputTypeDef *) date     ;

	  switch(stepMain)
		{
			//   ��ʼ��
			case 0:
				      switch(stepSub)
							{
								case 0:
									     switch(hsctr->p_stepConfig->TriggerMode)
											 {
												 
												 case 5:
												 case 6:
																 if(WS_Step_Input_check(hsctr) == WS_Step_Input_OK)
																 {
																		 stepSub ++; 
																 }
																 break   ;
																 
												 default: stepSub ++;  break;
													 
											 }
									     break;
								
								case 1:
									
								       WS_Audio_Open(hsctr->audioFilepath,101)  ;  //  ���������ļ�
									     if(hsctr->p_stepConfig->realy1DelayTrigger >= -1)
											 {
											    WS_Borad_Open_Relay_Time(1,1,0);
											 }
											 if(hsctr->p_stepConfig->realy2DelayTrigger >= -1)
											 {
											    WS_Borad_Open_Relay_Time(2,1,0);
											 }
								       stepSub ++ ;
								       return 500 ; 
								
								case 2:
									     if(WS_define_Audio_IS_Stop)
											 {
									        stepSub ++ ;
											 }
								       break;
								
								case 3:
									     stepMain ++  ;  stepSub = 0;
									     break;									     
							}								
			        break;
							
		  case 1:
				      
				      switch(hsctr->p_stepConfig->TriggerMode)
							{
							  case 1: stu = WS_Step_Input_Mode1(hsctr) ; break;
								case 2: stu = WS_Step_Input_Mode2(hsctr) ; break;
								case 3: stu = WS_Step_Input_Mode3(hsctr) ; break;
								case 4: stu = WS_Step_Input_Mode4(hsctr) ; break;
								case 5: stu = WS_Step_Input_Mode5(hsctr) ; break;
								case 6: stu = WS_Step_Input_Mode6(hsctr) ; break;
								case 7: stu = WS_Step_Input_Mode7(hsctr) ; break;
                case 8: stu = WS_Step_Input_Mode8(hsctr) ; break;	 	
                case 9: stu = WS_Step_Input_Mode9(hsctr) ; break;
                case 10: stu = WS_Step_Input_Mode10(hsctr) ; break;	 								
							}
				      if(stu == WS_Step_Input_OK  )
							{
							   stepMain ++ ; 
								 WS_Borad_Open_Buzzer_Time(500);							 
							}
              break;	

			case 2:
				      //  �趨�Զ���λʱ��
			        if(hsctr->p_stepConfig->resetMode > 0)
							{
							  WS_OS_Create_Timer(WS_Step1_AutoResetFlag_Timer,hsctr->p_stepConfig->resetMode*1000);
							}
							
							// ��������1
						  WS_Audio_Open(hsctr->audioFilepath,201);
			        
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
							    if(hsctr->p_stepConfig->realy1DelayTrigger > 0)
							       WS_Borad_Open_Relay_Time(2,1,hsctr->p_stepConfig->realy1DelayTrigger) ;
							    else
									   WS_Borad_Open_Relay_Time(2,0,0) ;
							    if(hsctr->p_stepConfig->realy2TriggerTime > 0)  // ��ʱ��
									{
									   WS_Borad_Open_Relay_Time(2,3,hsctr->p_stepConfig->realy2TriggerTime+hsctr->p_stepConfig->realy2DelayTrigger);  
									}
							}
							stepMain ++;
              return 250 ;		

			case  3:
				      if(WS_define_Audio_IS_Stop)
							{
							   WS_Audio_Open(hsctr->audioFilepath,202);
								 //  �ж��Ƿ����������̵���1״̬
								 if(hsctr->p_stepConfig->realy1DelayTrigger == -1)
								 {
								    WS_Borad_Open_Relay_Time(1,0,hsctr->p_stepConfig->realy1TriggerTime);
								 }
								 stepMain ++ ;
								 return 250;
							}
              break;
							
			case 4:
				     if(WS_define_Audio_IS_Stop)
							{
							   WS_Audio_Open(hsctr->audioFilepath,203);
								 //  �ж��Ƿ����������̵���2״̬
								 if(hsctr->p_stepConfig->realy2DelayTrigger == -1)
								 {
								    WS_Borad_Open_Relay_Time(2,0,hsctr->p_stepConfig->realy2TriggerTime);
								 }
								 stepMain++;
								 return 250;
							}
							
              break;
			
      //   ����λ							
			case 5:	
 				     if( hsctr->p_stepConfig->resetMode > 0 )  //  ��ʱ��λ
						 {
							  if(step1AutoResetFlag != 0)
								{
								   step1AutoResetFlag = 0 ;
									 stepMain ++ ;
								}
						    //stepMain ++ ;
                //return 	(int)hsctr->p_stepConfig->resetMode  * 1000 ;						 
						 }
             break; 

      case 6:
             if(hsctr->p_stepConfig->TriggerMode == 2  || hsctr->p_stepConfig->TriggerMode == 4 )
						 {
						     if(WS_Step_Input_Mode2(hsctr) == WS_Step_Input_ERR)
								 {
								    stepMain = 0;
								    WS_Borad_Open_Buzzer_Time(500);
								 }
						 }
						 else
						 {
								 stepMain = 0;
								 WS_Borad_Open_Buzzer_Time(500);
						 }
             break;			
				     
							
		}
	  return 9;
}

/******************************************************************************
	*�������ƣ�  WS_Step_Task_Step2
	*���ܣ�      �û�����1������

*******************************************************************************/

int WS_Step_Task_Step2(void * date)
{
     
	  static   uint16_t  stepMain = 0, stepSub = 0; 
	  static   WS_StepControlTypeDef  *hsctr = &hsctr2 ;
	  int      stu = WS_Step_Input_ERR;
	 
    
	  hsctr->p_sysin = (WS_SysInputTypeDef *) date     ;
	  
		

	  switch(stepMain)
		{
			//   ��ʼ��
			case 0:
				      switch(stepSub)
							{
								case 0:WS_Audio_Open(hsctr->audioFilepath,101)  ;  //  ���������ļ�
									     if(hsctr->p_stepConfig->realy1DelayTrigger >= -1)
											 {
											    WS_Borad_Open_Relay_Time(1,1,0);
											 }
											 if(hsctr->p_stepConfig->realy2DelayTrigger >= -1)
											 {
											    WS_Borad_Open_Relay_Time(2,1,0);
											 }
								       stepSub ++ ;
								       return 500 ; 
								
								case 1:
									     if(WS_define_Audio_IS_Stop)
											 {
									        stepSub ++ ;
											 }
								       break;
								
								case 2:
									     stepMain ++  ;  stepSub = 0;
									     break;									     
							}								
			        break;
							
		  case 1:
				
			        switch(hsctr->p_stepConfig->TriggerMode)
							{
							  case 1: stu = WS_Step_Input_Mode1(hsctr) ; break;
								case 2: stu = WS_Step_Input_Mode2(hsctr) ; break;
								case 3: stu = WS_Step_Input_Mode3(hsctr) ; break;
								case 4: stu = WS_Step_Input_Mode4(hsctr) ; break;	      
							}
				      if(stu == WS_Step_Input_OK  )
							{
							   stepMain ++ ; 
								 WS_Borad_Open_Buzzer_Time(500);							 
							}
              break;	

			case 2:
				      //  �趨�Զ���λʱ��
			        if(hsctr->p_stepConfig->resetMode > 0)
							{
							  WS_OS_Create_Timer(WS_Step2_AutoResetFlag_Timer,hsctr->p_stepConfig->resetMode*1000);
							}
							
							// ��������1
						  WS_Audio_Open(hsctr->audioFilepath,201);
			        
							// �̵������
			        if(hsctr->p_stepConfig->realy1DelayTrigger >= 0)   //  ��ʱ��
							{
							    WS_Borad_Open_Relay_Time(1,1,hsctr->p_stepConfig->realy1DelayTrigger) ;
							    if(hsctr->p_stepConfig->realy1TriggerTime > 0)  // ��ʱ��
									{
									   WS_Borad_Open_Relay_Time(1,3,hsctr->p_stepConfig->realy1TriggerTime+hsctr->p_stepConfig->realy1DelayTrigger);  
									}
							}
							if(hsctr->p_stepConfig->realy2DelayTrigger >= 0)   //  ��ʱ��
							{
							    WS_Borad_Open_Relay_Time(2,1,hsctr->p_stepConfig->realy2DelayTrigger) ;
							    if(hsctr->p_stepConfig->realy2TriggerTime > 0)  // ��ʱ��
									{
									   WS_Borad_Open_Relay_Time(2,3,hsctr->p_stepConfig->realy2TriggerTime+hsctr->p_stepConfig->realy2DelayTrigger);  
									}
							}
							stepMain ++;
              return 250 ;		

			case  3:
				      if(WS_define_Audio_IS_Stop)
							{
							   WS_Audio_Open(hsctr->audioFilepath,202);
								 //  �ж��Ƿ����������̵���1״̬
								 if(hsctr->p_stepConfig->realy1DelayTrigger == -1)
								 {
								    WS_Borad_Open_Relay_Time(1,0,hsctr->p_stepConfig->realy1TriggerTime);
								 }
								 stepMain ++ ;
								 return 250;
							}
              break;
							
			case 4:
				     if(WS_define_Audio_IS_Stop)
							{
							   WS_Audio_Open(hsctr->audioFilepath,203);
								 //  �ж��Ƿ����������̵���2״̬
								 if(hsctr->p_stepConfig->realy2DelayTrigger == -1)
								 {
								    WS_Borad_Open_Relay_Time(2,0,hsctr->p_stepConfig->realy2TriggerTime);
								 }
								 stepMain++;
								 return 250;
							}
							
              break;
			
      //   ����λ							
			case 5:	
 				     if( hsctr->p_stepConfig->resetMode > 0 )  
						 {
							  if(step2AutoResetFlag != 0)            //  ��⸴λ
								{
								   step2AutoResetFlag = 0;
									 stepMain ++ ;
								}						    					 
						 }
             break; 

      case 6:
				     if(hsctr->p_stepConfig->TriggerMode == 2  || hsctr->p_stepConfig->TriggerMode == 4   )
						 {
						     if(WS_Step_Input_Mode2(hsctr) == WS_Step_Input_ERR)
								 {
								    stepMain = 0;
								    WS_Borad_Open_Buzzer_Time(500);
								 }
						 }
						 else
						 {
								 stepMain = 0;
								 WS_Borad_Open_Buzzer_Time(500);
						 }
             break;			
				     
							
		}
	  return       9;
}


/******************************************************************************
	*�������ƣ�  WS_Creat_UserStep
	*���ܣ�      �����û���������
	*������     
              ��          
	*����:		  ��
*******************************************************************************/

void WS_Step_Creat(void)
{
	   //  ��ʼ�����̿��ƽṹ
	   hsctr1.p_stepConfig = &systemConfigInfo.hstep1;
	   hsctr1.audioFilepath= 1;
	   //hsctr1.p_stepConfig->TriggerMode = 2;
	
	   //   �жϸ������Ƿ�ʹ��
	   if(hsctr1.p_stepConfig->delayRun>0)
		 {
	      WS_Audio_Open_Blocking(WS_Audio_System_Filepath ,101)  ;  //  ����1������
	      WS_Audio_Open_Blocking(WS_Audio_System_Filepath,hsctr1.p_stepConfig->TriggerMode);
			   //  �����û�����1
			  WS_OS_Create_Task(WS_OS_ID_User_Step_1,WS_Step_Task_Step1,(uint32_t)hsctr1.p_stepConfig->delayRun * 1000);
		 }
	   
	  
	   //  ��ʼ�����̿��ƽṹ
	   hsctr2.p_stepConfig = &systemConfigInfo.hstep2;
	   hsctr2.audioFilepath= 2;
		 //hsctr2.p_stepConfig->TriggerMode = 2;
	  
	   if(hsctr2.p_stepConfig->delayRun > 0)
		 {
			 
			  WS_Audio_Open_Blocking(WS_Audio_System_Filepath ,102)  ;  //  ����1������
	      WS_Audio_Open_Blocking(WS_Audio_System_Filepath,hsctr2.p_stepConfig->TriggerMode);
			  //  �����û�����2
        WS_OS_Create_Task(WS_OS_ID_User_Step_2,WS_Step_Task_Step2,(uint32_t)hsctr2.p_stepConfig->delayRun * 1000);
		 }
}




