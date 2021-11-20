


#include "ws_core.h"
#include <stdarg.h>		  //  ��׼ͷ�ļ�
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



//   ��ʽ���ɱ�����ַ�������
char  formatBuf[256];
//   ��ʽ���ɱ�����ַ���
char *WS_Format(char *p,...)
{
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);
	  return formatBuf;
}


/********************************************************************************
	*�������ƣ� WS_Debug
	*���ܣ�     
  *������     wait :   �ȴ�����ʱ�䣬��λMS , ���ݲ���DMA���䣬
                       ��Ҫ�ȴ�������ɣ���д��ȴ�ʱ��
	*						
	*���أ�			��
*********************************************************************************/
char  debugBuf[256];
void WS_Debug(int wait , char *p,...)
{
	  /*  ��ʽ������  */
    va_list ap;
		va_start(ap,p); 
		vsprintf(debugBuf,p,ap);
		va_end(ap);	
	
//  �������	
#ifdef DEBUG_EN    
	  //  ��Ϣ����ӿ�
	  WS_UART_Transmit_String(&huart1,  debugBuf);     
 //   if(wait > 0) WS_Delay_MS(wait)              ;  	
#endif	
}


//  �����ַ����ڵ���ָ�������ַ���
//  p_stepString : �������ַ���
//  index        : ��Ҫ���ҵ��ַ����±�,1~XXXX;
//  p_RxBuf      ; ������ҵ����ַ���
char WS_Find_Index_String(char *p_stepString , int index, char *p_RxBuf)
{
	   int n = 0 ;     //  ��¼�����ַ���
	   int strnum = 0; //  ��¼�����ַ�����
	   p_RxBuf[0]  = 0;//  ���
     while(n < STEP_STRING_MAX_LEN && *p_stepString != 0) 
		 {  
				if((*p_stepString  == STOP_CHAR) ||  n == 0)
				{
				   strnum ++;
					 if(strnum == index) //  �ҵ�ƥ���±�
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





