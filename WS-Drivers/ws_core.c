


#include "ws_core.h"
#include <stdarg.h>		  //  标准头文件
#include <stdio.h>

extern UART_HandleTypeDef huart1;


void WS_Delay_US(int z)
{
   int x;
	 for(;z>0;z--)
	   for(x=28;x>0;x--);
}

void WS_Delay_MS(int z)
{
   int x;
	 for(;z>0;z--)
	   for(x=16000;x>0;x--);
}



//   格式化可变变量字符串缓存
char  formatBuf[256];
//   格式化可变变量字符串
char *WS_Format(char *p,...)
{
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);
	  return formatBuf;
}


/********************************************************************************
	*函数名称： WS_Debug
	*功能：     
  *参数：     wait :   等待发送时间，单位MS , 数据采用DMA传输，
                       需要等待发送完成，则写入等待时间
	*						
	*返回：			无
*********************************************************************************/
char  debugBuf[256];
void WS_Debug(int wait , char *p,...)
{
	  /*  格式化数据  */
    va_list ap;
		va_start(ap,p); 
		vsprintf(debugBuf,p,ap);
		va_end(ap);	
	
//  编译控制	
#ifdef DEBUG_EN    
	  //  信息输出接口
	  WS_UART_Transmit_String(&huart1,  debugBuf);     
 //   if(wait > 0) WS_Delay_MS(wait)              ;  	
#endif	
}


//  查找字符串内的在指定数字字符串
//  p_stepString : 被查找字符串
//  index        : 需要查找的字符串下标,1~XXXX;
//  p_RxBuf      ; 缓存查找到的字符串
char WS_Find_Index_String(char *p_stepString , int index, char *p_RxBuf)
{
	   int n = 0 ;     //  记录查找字符数
	   int strnum = 0; //  记录查找字符串数
	   p_RxBuf[0]  = 0;//  清空
     while(n < STEP_STRING_MAX_LEN && *p_stepString != 0) 
		 {  
				if((*p_stepString  == STOP_CHAR) ||  n == 0)
				{
				   strnum ++;
					 if(strnum == index) //  找到匹配下标
						{
							 if(n > 0) p_stepString ++;
							 while(*p_stepString != 0 && *p_stepString != STOP_CHAR)
							 {
							   *p_RxBuf = *p_stepString;
								  p_stepString ++         ;
								  p_RxBuf ++;
							 }
							 *p_RxBuf = 0;
							 return WS_Status_True;
						}
				}
			  n ++           ;
				p_stepString ++;
		 }
	 *p_RxBuf = 0; 
	 return WS_Status_False;	 
}





