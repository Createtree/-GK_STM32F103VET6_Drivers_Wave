/***********************************************************
文件名称:  Saveinfo.c
功    能:  完成数据保存的具体任务，提供数据保存结构体，对每个
           配置数据以读写的方式访问，在调用写函数自动完成保存，
           读函数能通过懒加载的方式自动初始化
编    写:  zzp
日    期:  2016-11-8

************************************************************/


#include "ws_config.h"
#include "ws_step.h  "

//    懒加载标志
char  configInfiInitIfg = 0;

//   系统配置信息定义
WS_ConfigInfoTypeDef   systemConfigInfo        ;



/*
函数名称: void WS_Config_System_Save_Timer(void)
函数功能：保存配置数据结构体
输入参数: 无								
返回值：  无
*/
int WS_Config_System_Save_Timer(void * data)
{
	 data = data ;
   WS_Config_Write_Struct_Callback((uint8_t *)&systemConfigInfo,sizeof(systemConfigInfo));
   return 0    ;
}

//   保存配置数据，1秒后调用配置函数
void WS_Config_System_Save(void)
{  
   CreateTimer(WS_Config_System_Save_Timer,100);	
}


/*
函数名称: void WS_System_Config_Init(void)
函数功能：读取配置数据初始化，如果是第一次上电设置默认数据
输入参数: 无								
返回值：  无
*/
void WS_System_Config_Init(void)
{
	//  读配置信息
	configInfiInitIfg = 1;   //  初始化标志
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
		 systemConfigInfo.powerOnCount ++;  //  记录开机次数
		 WS_Config_System_Save();           //  保存信息
		 WS_Debug(1,"Open count  : %d\r\n",systemConfigInfo.powerOnCount) ;	        
  }

}


//      读取系统配置信息虚函数
__weak  void  WS_Config_Read_Struct_Callback(uint8_t *  date  ,  uint32_t len)
{
        
}

//      写入系统配置信息虚函数
__weak  void  WS_Config_Write_Struct_Callback(uint8_t *  date  ,  uint32_t len)
{
        
}



