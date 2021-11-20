/******************************************************************************
 * @file     				 step_input_mode.h
 * @brief    				 ����ģʽ����
 * @version  				 1.0.0
 * @creationTime     2020_04_29
 * @creator					 
 * @readme
 *        1.STM32F030ϵ��   
 *				2. 
          3.
*******************************************************************************/



#include "step_input_mode.h"

//  ���̿��ƽṹ
extern WS_StepControlTypeDef    hsctr1;
extern WS_StepControlTypeDef    hsctr2;

/******************************************************************************
	*�������ƣ� WS_Step1_Input_Reset_Timer
	*���ܣ�     ��λ��������1�Ķ�ʱ���ص�����
	*������     
	*����:			
*******************************************************************************/

int WS_Step1_Input_Reset_Timer(void *date)
{
    hsctr1.pwCount = 0;
	  WS_Debug(1,"step1 input rst\r\n");
	  return 0;
}

/******************************************************************************
	*�������ƣ� WS_Step2_Input_Reset_Timer
	*���ܣ�     ��λ��������2�Ķ�ʱ���ص�����
	*������     
	*����:			
*******************************************************************************/

int WS_Step2_Input_Reset_Timer(void *date)
{
    hsctr2.pwCount = 0;
	  WS_Debug(1,"step2 input rst\r\n");
	  return 0;
}



/******************************************************************************
	*�������ƣ� WS_Step_Reset_Input
	*���ܣ�     �������븴λ
	*������     hstepctl�����̿��ƽṹ�壻time:���ø�λʱ��
	*����:			��
*******************************************************************************/
void WS_Step_Input_Reset(struct __WS_StepControlTypeDef  *hstepctl , int time)
{
     if(hstepctl == &hsctr1)
		 {
		    WS_OS_Create_Timer(WS_Step1_Input_Reset_Timer,time);
		 }
		 
		 if(hstepctl == &hsctr2)
		 {
		    WS_OS_Create_Timer(WS_Step2_Input_Reset_Timer,time);
		 }
}



/******************************************************************************
	*�������ƣ� WS_strchr
	*���ܣ�     ���ַ���������ָ���ַ��Ƿ����
	*������     ��
	*����:			�ַ�����λ�ã�������
*******************************************************************************/
int   WS_strchr(const char * str, char ch)
{
      int i = 0;
      while(str[i] > 0)
			{
			   if(str[i]  == ch)
				 {
				     return i;  
				 }
				 i++;
			}	
      return -1;			
}

/******************************************************************************
	*�������ƣ� WS_Step_Input_check
	*���ܣ�     ������������Ƿ�������״̬һ��������ͬʱ���ģʽ
	*������     ��
	*����:			״̬�� WS_Status_True������ƥ�� �� WS_Status_False����ƥ��
*******************************************************************************/
int  WS_Step_Input_check(struct __WS_StepControlTypeDef  *hstepctl)
{
     int i = 0;
	   uint8_t *p_pw  ;
	   uint32_t instu ;
	   if(hstepctl->p_sysin == NULL)  //  ����������ݵĺϷ���
	   {
		    return WS_Step_Input_EMPTY ;  
	   }
	   p_pw = (uint8_t *)hstepctl->p_stepConfig->stepPassword;   
	   instu = hstepctl->p_sysin->inputPortStu     ;
	   while(p_pw[i] > 0 )
		 {
			  if( (instu & 1<<(p_pw[i]-'1')) != 0   )
				{
				   return WS_Step_Input_ERR;
				}
			  i ++;
		 }
		 return WS_Step_Input_OK ;   
}

/******************************************************************************
	*�������ƣ� WS_Step_NOTInput_check
	*���ܣ�     ������������Ƿ��뷴������״̬һ��������ͬʱ���ģʽ
	*������     ��
	*����:			״̬�� WS_Status_True������ƥ�� �� WS_Status_False����ƥ��
*******************************************************************************/
int  WS_Step_NOTInput_check(struct __WS_StepControlTypeDef  *hstepctl)
{
     int i = 0;
	   uint8_t *p_pw  ;
	   uint32_t instu ;
	   if(hstepctl->p_sysin == NULL)  //  ����������ݵĺϷ���
	   {
		    return WS_Step_Input_EMPTY ;  
	   }
	   p_pw = (uint8_t *)hstepctl->p_stepConfig->stepPassword;   
	   instu = hstepctl->p_sysin->inputPortNOTStu     ;
	   while(p_pw[i] > 0 )
		 {
			  if( (instu & 1<<(p_pw[i]-'1')) != 0   )
				{
				   return WS_Step_Input_ERR;
				}
			  i ++;
		 }
		 return WS_Step_Input_OK ;   
}


/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode1
	*���ܣ�     ����ģʽ1���ƴ�ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode1(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputPortDown > 0)
		 {
			  key = hstepctl->p_sysin->inputPortDown;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  �Ǳ���������ֱ���˳�
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0  ;
														   return  WS_Step_Input_OK;
														}															
									 }
									 else
									 {
											hstepctl->pwCount = 0;
										 
									 }
							     key -= 0x30;
								}

                if(key > 0  ) 
								{ 									
									  WS_Borad_Open_Buzzer_Time(10)             ;
                    WS_Audio_Open(hstepctl->audioFilepath,key);	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}


/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode2
	*���ܣ�     ����ģʽ2��ͬʱģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode2(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 uint16_t key   = 0         ;
	    
	
	 if(hstepctl->p_sysin == NULL)  //  ����������ݵĺϷ���
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 else
	 {
		  key = hstepctl->p_sysin->inputPortDown;    
	 }
	 
	 switch(step)
			{
				case 0: 
					     
				        if(WS_Step_Input_check (hstepctl )  ==  WS_Step_Input_OK)
								 {
								   return  WS_Step_Input_OK  ; 
								 }

           		  //  �Ǳ���������ֱ���˳�
					      if(key > 0  &&   WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
								if(key > 0)
									{			            
											WS_Audio_Open(hstepctl->audioFilepath,key+50);	
									}

								break;
								
		   
			} 
   return WS_Step_Input_ERR;
}



/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode3
	*���ܣ�     ����ģʽ3��˳��ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode3(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputPortDown > 0)
		 {
			  key = hstepctl->p_sysin->inputPortDown;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  �Ǳ���������ֱ���˳�
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0  ;
														   return  WS_Step_Input_OK;
														}															
									 } 
									 else
									 {
										  if(hstepctl->pwCount > 0)
											{
											   if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount-1] != key)
												 {
												    hstepctl->pwCount = 0 ;
												 } 	
											}																					
									 }
							     key -= 0x30;
								}

                if(key > 0  ) 
								{ 									
									  WS_Borad_Open_Buzzer_Time(100)             ;
                    WS_Audio_Open(hstepctl->audioFilepath,key);	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}



/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode4
	*���ܣ�     ����ģʽ4��ͬʱ˳��ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode4(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputPortDown > 0)
		 {
			  key = hstepctl->p_sysin->inputPortDown;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  �Ǳ���������ֱ���˳�
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0   ;
															 if(WS_Step_Input_check(hstepctl) == WS_Step_Input_OK)
															 {	 
																	 return  WS_Step_Input_OK;
															 }
														}															
									 }
									 else
									 {
											if(hstepctl->pwCount > 0)
											{
											   if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount-1] != key)
												 {
												    hstepctl->pwCount = 0 ;
												 } 	
											}												
									 }
							     key -= 0x30;
								}

                if(key > 0  ) 
								{ 									
									  //WS_Borad_Open_Buzzer_Time(100)             ;
                    WS_Audio_Open(hstepctl->audioFilepath,key);	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}



/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode5
	*���ܣ�     ����ģʽ5��ͬʱ�뿪
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode5(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;
	    
	
	 if(hstepctl->p_sysin == NULL)  //  ����������ݵĺϷ���
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 else
	 {
		  key = hstepctl->p_sysin->inputPortUp;    
	 }
	 
	 switch(step)
			{
				case 0: 
					     
				        if(WS_Step_NOTInput_check (hstepctl )  ==  WS_Step_Input_OK)
								 {
								   return  WS_Step_Input_OK  ; 
								 }

           		  //  �Ǳ���������ֱ���˳�
					      if(key > 0  &&   WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
								if(key > 0)
									{			            
											WS_Audio_Open(hstepctl->audioFilepath,key);	
									}

								break;
								
		   
			} 
   return WS_Step_Input_ERR;
}


/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode6
	*���ܣ�     ����ģʽ4��ͬʱ˳��ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode6(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputPortUp > 0)
		 {
			  key = hstepctl->p_sysin->inputPortUp;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  �Ǳ���������ֱ���˳�
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0   ;
															 if(WS_Step_NOTInput_check(hstepctl) == WS_Step_Input_OK)
															 {	 
																	 return  WS_Step_Input_OK;
															 }
														}															
									 }
									 else
									 {
											if(hstepctl->pwCount > 0)
											{
											   if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount-1] != key)
												 {
												    hstepctl->pwCount = 0 ;
												 } 	
											}												
									 }
							     key -= 0x30;
								}

                if(key > 0  ) 
								{ 									
									  //WS_Borad_Open_Buzzer_Time(100)             ;
                    WS_Audio_Open(hstepctl->audioFilepath,key);	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}



/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode1
	*���ܣ�     ����ģʽ7�����IC ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode7(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0      , port = 0 , idCard = 0    ;

	 if(hstepctl->p_sysin != NULL)       //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputID > 0 )  
		 {
			  key = hstepctl->p_sysin->inputID;
			  port = (key >> 8) - '0'         ;
				idCard  = (key & 0x00ff) - '0'  ;
		 }
		 else
		 {
		    return   WS_Step_Input_EMPTY    ;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					     
					      if(idCard > 0)
				        {	 
									 idCard  += '0' ;
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == idCard) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0   ;
														   return  WS_Step_Input_OK;
														}															
									 }
									 else
									 {
											hstepctl->pwCount = 0;											
									 }
									 idCard  -= '0' ;							    
								}
								
								
								
                if(key > 0  ) 
								{ 	
									  WS_Borad_Open_Buzzer_Time(100) ;
                    WS_Audio_Open(port+8,idCard)   ;	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}



static uint32_t  idIn   = 0xffffffff   ;
/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode1
	*���ܣ�     ����ģʽ8��������IC ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode8(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 
	 uint16_t key   = 0      , port = 0 , idCard = 0    ;

	 if(hstepctl->p_sysin != NULL)       //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputID > 0 )  //  �����ݿ�Ƭ
		 {
			  key = hstepctl->p_sysin->inputID;
			  port = (key >> 8) - '0'         ;
				idCard  = (key & 0x00ff) - '0'  ;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  ��־������Ƭ�Ƿ�ڷ���ȷ
					      if(idCard > 0  && port > 0)
				        {	 
									  if(hstepctl->p_stepConfig->stepPassword[port-1] == (idCard + '0'))
										{
										    idIn &= ~(1<<(port-1) ) ;
											  								  
										}
                    else
										{
										    idIn |= (1<<(port-1) )  ;
										}											
								}
								
								//  ʶ���Ƿ�ȫ����ȷ
								hstepctl->p_sysin->inputPortStu >>= 8 ;
						    if(WS_Step_Input_check(hstepctl) == WS_Step_Input_OK  )
								{
										hstepctl->p_sysin->inputPortStu = idIn;
										if(WS_Step_Input_check(hstepctl) == WS_Step_Input_OK  )
										{
												idIn  = 0xffffffff       ;    
												return WS_Step_Input_OK  ;
										}			
								}
								
								//  ���ſ�Ƭ����
                if(idCard > 0  && port > 0) 
								{ 	
									  WS_Borad_Open_Buzzer_Time(100) ;
                    WS_Audio_Open(port+8,idCard)   ;	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}



/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode9
	*���ܣ�     ����ģʽ9������ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode9(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputKnockDoor > 0)
		 {
			  key = hstepctl->p_sysin->inputKnockDoor;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      //  �Ǳ���������ֱ���˳�
//					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
//									{
//										 break  ;	
//									}
					      if(key > 0)
				        {	 
									 WS_Step_Input_Reset(hstepctl ,6000) ; //  ����5���Ը�λ
									 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0  ;
														   return  WS_Step_Input_OK;
														}															
									 }
									 else
									 {
											hstepctl->pwCount = 0;											
									 }
							     key -= 0x30;
								}

                if(key > 0  ) 
								{ 									
									  WS_Borad_Open_Buzzer_Time(100)             ;
                    //WS_Audio_Open(hstepctl->audioFilepath,key);	  
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}




//  ������  telegram
char   telegramInTickBuf[8] =  "00000";
char   telegramInTickCode   = 0  ,telegramInCh = 0  ;
uint16_t   telegramInTickCount = 0;

const  char  telegramCode[36][8] = 
{
  "12",	 "2111" , "2121" ,"211"	 ,"1"	 ,"1121" , "221"	 ,"1111" ,"11"	 ,"1222" ,"212"	 ,"1211","22"	 ,
  "21"   ,"222"	 , "1221" ,"2212" ,"121"	 ,"111"	 ,"2"    ,"112"	 ,"1112" ,"122"	 ,"2112" ,"2122" ,"2211" ,
  "22222","12222","11222","11122","11112","11111","21111","22111","22211","22221",
  
};

//    �����ַ����
int   WS_Telegram_Long_Timer(void * date)
{
	    char     i;
      date = date ;
	    telegramInCh = 0;
	
	
	    for(i = 0; i< 26 ; i++)
	    {
		    if(strcmp (telegramCode[i] ,(char *)telegramInTickBuf ) == 0)
				{
				   telegramInCh = (char)(i + 'A');
					 break;
				}
		  }
			
			
			if(telegramInCh == 0)
			{
				for(i = 0; i< 10 ; i++)
				{
					if(strcmp (telegramCode[i+26] ,(char *)telegramInTickBuf ) == 0)
					{
						 telegramInCh = (char)(i + '0');
						 break;
					}
				}
			}
			
			if(telegramInCh != 0)
			{
			   WS_Debug(1,"telegramInCh %c\r\n",telegramInCh) ;
			}
			
			//  ��������¼
			telegramInTickCount = 0;
			
		return 0;	
}

//    ����δ���
uint16_t    WS_Telegram_Short_Check(int time)
{
	    telegramInTickCode = 0;
	     
			if(time < 500000)              //  �̰�
			{
					telegramInTickCode = '1' ;
			}
			else
			{
					telegramInTickCode = '2' ;
			}
			if(telegramInTickCode != 0)
			{
					telegramInTickBuf[telegramInTickCount ++]  =  telegramInTickCode    ;
					if(telegramInTickCount < sizeof(telegramInTickBuf) ) telegramInTickBuf[telegramInTickCount ]    = 0;
					if(telegramInTickCount >= sizeof(telegramInTickBuf))  telegramInTickCount = 0;			  
					WS_OS_Create_Timer(WS_Telegram_Long_Timer,2000);
			}		
	  return telegramInTickCode;
}




/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode10
	*���ܣ�     ����ģʽ10������ģʽ
	*������     ��
	*����:			ִ��ʱ����
*******************************************************************************/
int WS_Step_Input_Mode10(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 static int       time = 0  ;
	 uint16_t  key = 0          ;

	 if(hstepctl->p_sysin != NULL)  //  ����������ݵĺϷ���
	 {
		 if(hstepctl->p_sysin->inputPortUp > 0)
		 { 
			  WS_define_Borad_Close_Buzzer;
			  
			  time = WS_CCP_Measure_Time(0,2);
			  if(time > 40000)
				{ 
					 key =  WS_Telegram_Short_Check(time);
				}
		 }
		 if(hstepctl->p_sysin->inputPortDown > 0)
		 {  
			  WS_define_Borad_Open_Buzzer;
			  WS_CCP_Measure_Time(0,0)   ; 
			  return WS_Step_Input_ERR   ;
		 }
	 }
	 else
	 {
	     return WS_Step_Input_EMPTY ;   
	 }
	 
	 switch(step)
			{
				case 0: 
					      if(key > 0)
				        {	
									
									 WS_Step_Input_Reset(hstepctl ,6000) ; //  ����5���Ը�λ
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  �ж�������ȷ���
									 {  
														hstepctl->pwCount ++; 
														if( hstepctl->pwCount >= strlen((char *)hstepctl->p_stepConfig->stepPassword)) 
														{
															 hstepctl->pwCount = 0  ;
														   return  WS_Step_Input_OK;
														}															
									 }
									 else
									 {
											hstepctl->pwCount = 0;											
									 } 
								}  								
								break;
								
		   
			} 
      return WS_Step_Input_ERR;
}


/******************************************************************************
	*�������ƣ� WS_Step_Input_Mode_Blocking
	*���ܣ�     �������̿��Ʋ����������̣߳����������
	*������     ��
	*����:			״̬
*******************************************************************************/

int WS_Step_Input_Mode_Blocking(struct __WS_StepControlTypeDef  *hstepct )
{
	  WS_SysInputTypeDef     *p_hsysin     ;
	  uint16_t stu = WS_Step_Input_ERR    ;	
	
	  for(;;)
	  {
    //  �ȴ�֪ͨ    ���޸�֪ͨ����
		  xTaskNotifyWait( 0, 0,(uint32_t *)&p_hsysin, portMAX_DELAY );
			hstepct->p_sysin  = p_hsysin ;
			switch(hstepct->p_stepConfig->TriggerMode)
			{
				case 1: stu =  WS_Step_Input_Mode1(hstepct) ; break;
				case 2: stu =  WS_Step_Input_Mode2(hstepct) ; break;
				case 3: stu =  WS_Step_Input_Mode3(hstepct) ; break;
				case 4: stu =  WS_Step_Input_Mode4(hstepct) ; break;	      
			}
			if(stu == WS_Step_Input_OK)
			{
				 WS_Borad_Open_Buzzer_Time(100);
			   break;
			}
		}
	  return 	WS_Step_Input_OK ;					 
}


















