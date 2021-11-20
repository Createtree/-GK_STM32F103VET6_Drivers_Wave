/******************************************************************************
 * @file     				 step_input_mode.h
 * @brief    				 输入模式处理
 * @version  				 1.0.0
 * @creationTime     2020_04_29
 * @creator					 
 * @readme
 *        1.STM32F030系列   
 *				2. 
          3.
*******************************************************************************/



#include "step_input_mode.h"

//  流程控制结构
extern WS_StepControlTypeDef    hsctr1;
extern WS_StepControlTypeDef    hsctr2;

/******************************************************************************
	*方法名称： WS_Step1_Input_Reset_Timer
	*功能：     复位输入流程1的定时器回调函数
	*参数：     
	*返回:			
*******************************************************************************/

int WS_Step1_Input_Reset_Timer(void *date)
{
    hsctr1.pwCount = 0;
	  WS_Debug(1,"step1 input rst\r\n");
	  return 0;
}

/******************************************************************************
	*方法名称： WS_Step2_Input_Reset_Timer
	*功能：     复位输入流程2的定时器回调函数
	*参数：     
	*返回:			
*******************************************************************************/

int WS_Step2_Input_Reset_Timer(void *date)
{
    hsctr2.pwCount = 0;
	  WS_Debug(1,"step2 input rst\r\n");
	  return 0;
}



/******************************************************************************
	*方法名称： WS_Step_Reset_Input
	*功能：     设置输入复位
	*参数：     hstepctl：流程控制结构体；time:设置复位时间
	*返回:			无
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
	*方法名称： WS_strchr
	*功能：     在字符串内搜索指定字符是否存在
	*参数：     无
	*返回:			字符所在位置（索引）
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
	*方法名称： WS_Step_Input_check
	*功能：     检测流程密码是否与输入状态一至，用于同时检测模式
	*参数：     无
	*返回:			状态， WS_Status_True：密码匹配 ， WS_Status_False：不匹配
*******************************************************************************/
int  WS_Step_Input_check(struct __WS_StepControlTypeDef  *hstepctl)
{
     int i = 0;
	   uint8_t *p_pw  ;
	   uint32_t instu ;
	   if(hstepctl->p_sysin == NULL)  //  检查输入数据的合法性
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
	*方法名称： WS_Step_NOTInput_check
	*功能：     检测流程密码是否与反向输入状态一至，用于同时检测模式
	*参数：     无
	*返回:			状态， WS_Status_True：密码匹配 ， WS_Status_False：不匹配
*******************************************************************************/
int  WS_Step_NOTInput_check(struct __WS_StepControlTypeDef  *hstepctl)
{
     int i = 0;
	   uint8_t *p_pw  ;
	   uint32_t instu ;
	   if(hstepctl->p_sysin == NULL)  //  检查输入数据的合法性
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
	*方法名称： WS_Step_Input_Mode1
	*功能：     输入模式1，计次模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode1(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
					      //  非本流程输入直接退出
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode2
	*功能：     输入模式2，同时模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode2(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 uint16_t key   = 0         ;
	    
	
	 if(hstepctl->p_sysin == NULL)  //  检查输入数据的合法性
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

           		  //  非本流程输入直接退出
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
	*方法名称： WS_Step_Input_Mode3
	*功能：     输入模式3，顺序模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode3(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
					      //  非本流程输入直接退出
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode4
	*功能：     输入模式4，同时顺序模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode4(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
					      //  非本流程输入直接退出
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode5
	*功能：     输入模式5，同时离开
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode5(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;
	    
	
	 if(hstepctl->p_sysin == NULL)  //  检查输入数据的合法性
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

           		  //  非本流程输入直接退出
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
	*方法名称： WS_Step_Input_Mode6
	*功能：     输入模式4，同时顺序模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode6(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
					      //  非本流程输入直接退出
					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
									{
										 break  ;	
									}
									
					      if(key > 0)
				        {	 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode1
	*功能：     输入模式7，身份IC 模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode7(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0 ; 
	 uint16_t key   = 0      , port = 0 , idCard = 0    ;

	 if(hstepctl->p_sysin != NULL)       //  检查输入数据的合法性
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
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == idCard) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode1
	*功能：     输入模式8，多点身份IC 模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode8(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 
	 uint16_t key   = 0      , port = 0 , idCard = 0    ;

	 if(hstepctl->p_sysin != NULL)       //  检查输入数据的合法性
	 {
		 if(hstepctl->p_sysin->inputID > 0 )  //  检测身份卡片
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
					      //  标志单个卡片是否摆放正确
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
								
								//  识别是否全部正确
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
								
								//  播放卡片语音
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
	*方法名称： WS_Step_Input_Mode9
	*功能：     输入模式9，敲门模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode9(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 uint16_t key   = 0         ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
					      //  非本流程输入直接退出
//					      if(key > 0 &&  WS_strchr((char *)hstepctl->p_stepConfig->stepPassword,key + '0') == -1)
//									{
//										 break  ;	
//									}
					      if(key > 0)
				        {	 
									 WS_Step_Input_Reset(hstepctl ,6000) ; //  输入5秒自复位
									 key += '0';	
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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




//  发报机  telegram
char   telegramInTickBuf[8] =  "00000";
char   telegramInTickCode   = 0  ,telegramInCh = 0  ;
uint16_t   telegramInTickCount = 0;

const  char  telegramCode[36][8] = 
{
  "12",	 "2111" , "2121" ,"211"	 ,"1"	 ,"1121" , "221"	 ,"1111" ,"11"	 ,"1222" ,"212"	 ,"1211","22"	 ,
  "21"   ,"222"	 , "1221" ,"2212" ,"121"	 ,"111"	 ,"2"    ,"112"	 ,"1112" ,"122"	 ,"2112" ,"2122" ,"2211" ,
  "22222","12222","11222","11122","11112","11111","21111","22111","22211","22221",
  
};

//    输入字符检测
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
			
			//  清除缓存记录
			telegramInTickCount = 0;
			
		return 0;	
}

//    输入滴答检测
uint16_t    WS_Telegram_Short_Check(int time)
{
	    telegramInTickCode = 0;
	     
			if(time < 500000)              //  短按
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
	*方法名称： WS_Step_Input_Mode10
	*功能：     输入模式10，敲门模式
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int WS_Step_Input_Mode10(struct __WS_StepControlTypeDef  *hstepctl )
{	 
	 static uint16_t  step = 0  ; 
	 static int       time = 0  ;
	 uint16_t  key = 0          ;

	 if(hstepctl->p_sysin != NULL)  //  检查输入数据的合法性
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
									
									 WS_Step_Input_Reset(hstepctl ,6000) ; //  输入5秒自复位
									 if(hstepctl->p_stepConfig->stepPassword[hstepctl->pwCount] == key) //  判断输入真确与否
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
	*方法名称： WS_Step_Input_Mode_Blocking
	*功能：     根据流程控制参数，阻塞线程，完成输入检测
	*参数：     无
	*返回:			状态
*******************************************************************************/

int WS_Step_Input_Mode_Blocking(struct __WS_StepControlTypeDef  *hstepct )
{
	  WS_SysInputTypeDef     *p_hsysin     ;
	  uint16_t stu = WS_Step_Input_ERR    ;	
	
	  for(;;)
	  {
    //  等待通知    不修改通知参数
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


















