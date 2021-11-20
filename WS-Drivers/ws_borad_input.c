/******************************************************************************
 * @file     				 ws_borad_input.c
 * @brief    				 �������������Դ����
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#include "ws_borad_input.h"

//     MP3����״̬��ֵ�ź�
extern osSemaphoreId WS_Audio_Idle_SemaphoreHandle;

//     ������
extern   osThreadId WS_FreeTh_UTHandle;


//      ���IC����־
int    idCardCheckIfg  = 0 ;





/******************************************************************************
*�������ƣ�   WS_Borad_Get_Port_Up
	*���ܣ�     �õ��Դ��˿ڲ����½��صĶ˿ڱ��
	*������     ��                  
	*����:		  �����½��صĶ˿ڱ��
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
*�������ƣ�   WS_Borad_Get_Port_Up
	*���ܣ�     �õ��Դ��˿ڲ��������صĶ˿ڱ��
	*������     ��                  
	*����:		  ���������صĶ˿ڱ��
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
�������ƣvchar WS_Borad_Get_Port_Stu_Pin(char n)
�������ܣ��õ�ָ������˿ڵ�״̬
�������: n:��Ҫ��ѯ�İ������(1~8)	
����ֵ  ��������״̬ ��0���ӵ� ��1���Ͽ���2����Ч�˿�
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
�������ƣvchar WS_Borad_Get_Port_Stu_All(void)
�������ܣ��õ���������˿ڵ�״̬
�������: ��	
����ֵ  �����ж˿�״̬  
*/
uint16_t   WS_Borad_Get_Port_All_Stu(void)
{
	     return WS_define_Borad_Input_Date;
}


/*
�������ƣvchar WS_Borad_Get_Port_All_NOTStu(void)
�������ܣ��õ���������˿ڵ�״̬
�������: ��	
����ֵ  �����ж˿�״̬  
*/
uint16_t   WS_Borad_Get_Port_All_NOTStu(void)
{
	     return WS_define_Borad_Input_NOTDate;
}





//  ���4��ң�ذ��µİ���ֵ�� ��������
/*
�������ƣvchar WS_Borad_Get_Port_Stu_Pin(char n)
�������ܣ��õ�ָ������˿ڵ�״̬
�������: n:��Ҫ��ѯ�İ������(1~8)	
����ֵ  ��������״̬ ��0���ӵ� ��1���Ͽ���2����Ч�˿�
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

//  ���4��ң�ذ��µİ����½��أ�ÿ����һ�η���һ��
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


//  ���12��ң�ذ��µİ���ֵ�� ��������
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

//  ���12��ң�ذ��µİ����½��أ�ÿ����һ�η���һ��
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
	*�������ƣ� WS_OS_Task_System_Input_Check
	*���ܣ�     ����ϵͳ���룬ͨ������֪ͨ���͸�ָ���̴߳���
	*������     
              ��
	*����:		  ��
*******************************************************************************/
void  WS_OS_Task_System_Input_Check(void)
{
	 uint16_t    up  = 0 , down   = 0  , i = 0    , idCard  ; 
	 static  uint16_t      timeOut   =  0  ;
	 static WS_SysInputTypeDef  hsysin     ;
	
	 do 
	 {
	     //  �˿�����
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
			  
			 
			 if(++ timeOut == 100)  //  ÿ�����һ������״̬
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
	
	    //  ��ȡ����
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







