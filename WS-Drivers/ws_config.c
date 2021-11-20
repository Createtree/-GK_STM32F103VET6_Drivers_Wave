/***********************************************************
�ļ�����:  Saveinfo.c
��    ��:  ������ݱ���ľ��������ṩ���ݱ���ṹ�壬��ÿ��
           ���������Զ�д�ķ�ʽ���ʣ��ڵ���д�����Զ���ɱ��棬
           ��������ͨ�������صķ�ʽ�Զ���ʼ��
��    д:  zzp
��    ��:  2016-11-8

************************************************************/


#include "ws_config.h"
#include "ws_step.h  "

//    �����ر�־
char  configInfiInitIfg = 0;

//   ϵͳ������Ϣ����
WS_ConfigInfoTypeDef   systemConfigInfo        ;



/*
��������: void WS_Config_System_Save_Timer(void)
�������ܣ������������ݽṹ��
�������: ��								
����ֵ��  ��
*/
int WS_Config_System_Save_Timer(void * data)
{
	 data = data ;
   WS_Config_Write_Struct_Callback((uint8_t *)&systemConfigInfo,sizeof(systemConfigInfo));
   return 0    ;
}

//   �����������ݣ�1���������ú���
void WS_Config_System_Save(void)
{  
   CreateTimer(WS_Config_System_Save_Timer,100);	
}


/*
��������: void WS_System_Config_Init(void)
�������ܣ���ȡ�������ݳ�ʼ��������ǵ�һ���ϵ�����Ĭ������
�������: ��								
����ֵ��  ��
*/
void WS_System_Config_Init(void)
{
	//  ��������Ϣ
	configInfiInitIfg = 1;   //  ��ʼ����־
	WS_Config_Read_Struct_Callback((uint8_t *)&systemConfigInfo,sizeof(systemConfigInfo));	 
	if(systemConfigInfo.initFlag != 's')
	{
		 systemConfigInfo.initFlag   =  's';
		 systemConfigInfo.powerOnCount = 1 ;
		

		 WS_Debug(2,"First power on\r\n")         ;
     WS_Config_System_Save_Timer((void *)1)   ; 		 
  }
	else
  {
		 systemConfigInfo.powerOnCount ++;  //  ��¼��������
		 WS_Config_System_Save();           //  ������Ϣ
		 WS_Debug(1,"Open count  : %d\r\n",systemConfigInfo.powerOnCount) ;	        
  }

}


//      ��ȡϵͳ������Ϣ�麯��
__weak  void  WS_Config_Read_Struct_Callback(uint8_t *  date  ,  uint32_t len)
{
        
}

//      д��ϵͳ������Ϣ�麯��
__weak  void  WS_Config_Write_Struct_Callback(uint8_t *  date  ,  uint32_t len)
{
        
}



