/******************************************************************************
 * @file     				 ws_borad_input.c
 * @brief    				 处理板载输入资源驱动
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#include "ws_borad_input.h"

//     MP3工作状态二值信号
extern osSemaphoreId WS_Audio_Idle_SemaphoreHandle;

//     任务句柄
extern   osThreadId WS_FreeTh_UTHandle;


//      身份IC检测标志
int    idCardCheckIfg  = 0 ;





/******************************************************************************
*函数名称：   WS_Borad_Get_Port_Up
	*功能：     得到自带端口产生下降沿的端口编号
	*参数：     无                  
	*返回:		  产生下降沿的端口编号
*******************************************************************************/
uint16_t WS_Borad_Get_Port_Down(void)
{
	  static  unsigned int swIfg = 0;
    unsigned int k, num ;  //  
	  char     rtn = 0, i   = 0 ;//  
	
	  if(idCardCheckIfg > 0)
		{
		  idCardCheckIfg --;
			k |= 0xffff;
		}
		else
		{
		  k  = WS_define_Borad_Input_Date ;
		}
	  
	
	
	  if(k != 0xffff)
		{  num =  swIfg | k;
			 if(num != 0xffff)
			 {
				for(i=0;i<16;i++)
				{
				   if((num & 1<<i) == 0) // 
				       break;
				}
			 if(i < 16)      //
			  {	 
				   rtn = i+1;
				}
				swIfg |= 1<<i;  // 
			 }
		}
		
		num = ~(swIfg & k);  // 
		swIfg &= num      ;  // 
		return rtn        ; 		
}


/******************************************************************************
*函数名称：   WS_Borad_Get_Port_Up
	*功能：     得到自带端口产生上升沿的端口编号
	*参数：     无                  
	*返回:		  产生上升沿的端口编号
*******************************************************************************/
uint16_t WS_Borad_Get_Port_Up(void)
{
	  static  unsigned int swIfg = 0;
    unsigned int k, num ;  //  
	  char     rtn = 0, i   = 0 ;//  
	  if(idCardCheckIfg > 0)
		{
		  idCardCheckIfg --;
			k |= 0xffff;
		}
		else
		{
	    k  = WS_define_Borad_Input_NOTDate ;
		}
	  if(k != 0xffff)
		{  num =  swIfg | k;
			 if(num != 0xffff)
			 {
				for(i=0;i<16;i++)
				{
				   if((num & 1<<i) == 0) // 
				       break;
				}
			 if(i < 16)      //
			  {	 
				   rtn = i+1;
				}
				swIfg |= 1<<i;  // 
			 }
		}
		num = ~(swIfg & k);  // 
		swIfg &= num      ;  // 
		return rtn        ; 		
}

/*
函数名称char WS_Borad_Get_Port_Stu_Pin(char n)
函数功能：得到指定输入端口的状态
输入参数: n:需要查询的按键编号(1~8)	
返回值  ：按键的状态 ：0：接地 ；1：断开；2：无效端口
*/
char WS_Borad_Get_Port_Pin_Stu(char n)
{
	   char f = 2;
     unsigned char in   ;  
     if(n < 1 || n > 12) return 2;	   
		 
		 in = WS_define_Borad_Input_Date ;
		 f = (in & (1<<n-1))!=0; 	 
	
	   return f;	 
}

/*
函数名称char WS_Borad_Get_Port_Stu_All(void)
函数功能：得到所有输入端口的状态
输入参数: 无	
返回值  ：所有端口状态  
*/
uint16_t   WS_Borad_Get_Port_All_Stu(void)
{
	     return WS_define_Borad_Input_Date;
}


/*
函数名称char WS_Borad_Get_Port_All_NOTStu(void)
函数功能：得到所有输入端口的状态
输入参数: 无	
返回值  ：所有端口状态  
*/
uint16_t   WS_Borad_Get_Port_All_NOTStu(void)
{
	     return WS_define_Borad_Input_NOTDate;
}





//  获得4键遥控按下的按键值， 持续返回
/*
函数名称char WS_Borad_Get_Port_Stu_Pin(char n)
函数功能：得到指定输入端口的状态
输入参数: n:需要查询的按键编号(1~8)	
返回值  ：按键的状态 ：0：接地 ；1：断开；2：无效端口
*/
char getWxKey4(void)
{
   unsigned char f;
   f = WS_define_Borad_RX_DATE ;        
	 if(f == 0x0f) return 0;
	 if(f == 0x8)  return 1;
	 if(f == 0x4)  return 2;
	 if(f == 0x2)  return 3;
	 if(f == 0x1)  return 4;               
	 return f;
}

//  获得4键遥控按下的按键下降沿，每按下一次返回一次
char getWxKey4Down(void)
{
	char w ,f = 0;
	static char wxIfg = 0;
	w = WS_define_Borad_RX_DATE;
	
	if(w != 0xf && w != 0)
	{
	  if(wxIfg == 0)
		{
			wxIfg = 1; 
			if(w == 1) f = 4;
			if(w == 2) f = 3;
			if(w == 4) f = 2;
			if(w == 8) f = 1;
	
		}
	}
	else
	{
	   wxIfg = 0;
	}
	
  return f;
}


//  获得12键遥控按下的按键值， 持续返回
char getWxKey12(void)
{
	char w , f = 0;
	w = WS_define_Borad_RX_DATE;	
	if(w == 1) f = 1;
	if(w == 2) f = 2;
	if(w == 3) f = 3;
	if(w == 4) f = 4;
	if(w == 5) f = 5;
	if(w == 6) f = 6;
	if(w == 7) f = 7;
	if(w == 8) f = 8;
	if(w == 9) f = 9;
	if(w == 10) f = 10;
	if(w == 11) f = 11;
	if(w == 12) f = 12;		
  return f;
}

//  获得12键遥控按下的按键下降沿，每按下一次返回一次
char getWxKey12Down(void)
{
	char w ,f = 0;
	static char wxIfg = 0;
	w = WS_define_Borad_RX_DATE;
	
	if(w != 0xf && w != 0)
	{
	  if(wxIfg == 0)
		{
			wxIfg = 1; 
			if(w == 1) f =  1;
			if(w == 2) f = 2;
			if(w == 3) f = 3;
			if(w == 4) f = 4;
			if(w == 5) f = 5;
			if(w == 6) f = 6;
			if(w == 7) f = 7;
			if(w == 8) f = 8;
			if(w == 9) f = 9;
			if(w == 10) f = 10;
			if(w == 11) f = 11;
			if(w == 12) f = 12;	
		}
	}
	else
	{
	   wxIfg = 0;
	}
  return f;
}







/******************************************************************************
	*方法名称： WS_OS_Task_System_Input_Check
	*功能：     处理系统输入，通过任务通知发送给指定线程处理
	*参数：     
              无
	*返回:		  无
*******************************************************************************/
void  WS_OS_Task_System_Input_Check(void)
{
	 uint16_t    up  = 0 , down   = 0  , i = 0    , idCard  ; 
	 static  uint16_t      timeOut   =  0  ;
	 static WS_SysInputTypeDef  hsysin     ;
	
	 do 
	 {
	     //  端口输入
			 down = (uint16_t)WS_Borad_Get_Port_Down() ; 
			 up   = (uint16_t)WS_Borad_Get_Port_Up()   ;
		   if(down > 0 || up > 0 )
			 {
				 hsysin.inputPortDown = (uint16_t)down;
				 hsysin.inputPortUp   = (uint16_t)up  ;
				 hsysin.inputPortStu  = WS_Borad_Get_Port_All_Stu();
				 hsysin.inputPortNOTStu = WS_Borad_Get_Port_All_NOTStu();
				 hsysin.inputID         =  0 ;
				 hsysin.inputKnockDoor = 0   ;  
				 WS_Borad_Open_DebugLed_Time(10);
				 timeOut =0                  ;
				 xTaskNotify(WS_FreeTh_UTHandle,(uint32_t)&hsysin,eSetValueWithOverwrite) ;
				 break      ;
			 }
		   
			 
			 for(i=1;i<=8;i++)
			 {
					idCard = WS_Borad_IC_Number_Input(i) ;
					if(idCard > 0)
					{
							 timeOut = 0;
							 hsysin.inputPortDown = (uint16_t)0;
							 hsysin.inputPortUp   = (uint16_t)0;
							 hsysin.inputPortStu  = WS_Borad_Get_Port_All_Stu();
							 hsysin.inputPortNOTStu = WS_Borad_Get_Port_All_NOTStu();
							 hsysin.inputID         =  ((i+'0')<<8) + idCard + '0'  ; 
							 hsysin.inputKnockDoor = 0; 	
               xTaskNotify(WS_FreeTh_UTHandle,(uint32_t)&hsysin,eSetValueWithOverwrite) ;
							 break;
					}
			 }
			  
			 
			 if(++ timeOut == 100)  //  每秒跟新一次输入状态
			 {
					 timeOut = 0;
					 hsysin.inputPortDown   = (uint16_t)0;
					 hsysin.inputPortUp     = (uint16_t)0;
					 hsysin.inputPortStu    = WS_Borad_Get_Port_All_Stu();
					 hsysin.inputPortNOTStu = WS_Borad_Get_Port_All_NOTStu();
					 hsysin.inputID         =  0 ;
					 hsysin.inputKnockDoor  =  0 ; 
				 	 xTaskNotify(WS_FreeTh_UTHandle,(uint32_t)&hsysin,eSetValueWithOverwrite) ;		 
					 break                       ;
			 }
			 
	    
	 }while(0);
	
	    //  获取敲门
//	 i = WS_Knock_Door_Get_Count(); 
//	 if(i > 0)
//	 {
//	   hsysin.inputPortDown = (uint16_t)0;
//		 hsysin.inputPortUp   = (uint16_t)0 ;
//		 hsysin.inputPortStu  = WS_Borad_Get_Port_All_Stu();
//		 hsysin.inputPortNOTStu = WS_Borad_Get_Port_All_NOTStu();
//		 hsysin.inputID         =  0 ;
//		 hsysin.inputKnockDoor  =   i; 
//	   WS_OS_Write_Task_Parameter(WS_OS_ID_User_Step_1,(void *)&hsysin,10);
//		 WS_OS_Write_Task_Parameter(WS_OS_ID_User_Step_2,(void *)&hsysin,10);
//	   WS_Borad_Open_DebugLed_Time(100);
//		 timeOut =0 ;
//		 return 9   ;
//	 }
	
	 
	 
//	 return 9 ;
	  
}







