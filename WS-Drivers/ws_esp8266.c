/******************************************************************************
 * @file     				 ws_esp8266.c
 * @brief    				 esp8266 相关操作
 * @version  				 1.0.0
 * @creationTime     2020_06_23
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/

#include "ws_esp8266.h"

static  int ESPLinkStep = 1;
extern  UART_HandleTypeDef huart1;


/******************************************************************************
	*函数名称： WS_ESP8266_Send_AT
	*功能：     ESP8266发送AT指令
	*参数：     无
	*返回:			无
*******************************************************************************/

void  WS_ESP8266_Transmit_AT(char  * str)
{
      WS_UART_Transmit_String(&huart1 ,  WS_Format("%s\r\n",str));
}



/******************************************************************************
	*函数名称： WS_OS_Task_ESP8266_Link
	*功能：     ESP8266链接网络
	*参数：     无
	*返回:			无
*******************************************************************************/

int WS_OS_Task_ESP8266_Link(void * date)
{
	   
     switch(ESPLinkStep)
		 {
			 case 1:
				        WS_ESP8266_Transmit_AT("AT+CIPSTATUS") ;
				        return 150; 
			 
			 case 2:
				        WS_ESP8266_Transmit_AT( "AT+CWMODE_DEF=3")	;
			          ESPLinkStep ++ ;
				        return 100;
			 
			 case 3:
				        WS_ESP8266_Transmit_AT( "AT+CWJAP_DEF=\"zzp\",\"19900109\"")	;
			          ESPLinkStep = 1 ;  
				        return 400;   //  4秒手继续查询网络状态
			 
			 
			 case 4:
				       WS_ESP8266_Transmit_AT( "AT+CIPSTART=\"TCP\",\"192.168.1.101\",60000");
			         ESPLinkStep = 1 ;  
				       return 300;   //  3秒手继续查询网络状态
			 
			 case 5:
				       break;
		 }
	   return 9  ;  
}


/******************************************************************************
	*函数名称： WS_ESP8266_Receive_Callback
	*功能：     ESP8266接收的数据回调函数
	*参数：     无
	*返回:			无
*******************************************************************************/

void WS_ESP8266_Receive_Callback(char *date , int len)
{
	 //  int i ;
     WS_RS485_Transmit_Byte(date,len);

     switch(ESPLinkStep)	
		 {
			 case 1: 
				        do
								{
									
									if(len < 23 || strncmp("AT+CIPSTATUS",date,12) != 0 || strncmp("STATUS",date+15,6) != 0)
									{
									   break;
									}
									
									if(date[22] == '5' ) //  网路未链接
									{
										 ESPLinkStep = 2; 						 
										 break;
									}
									
									if(date[22] == '2' ) //  网路链接,未建立TCP
									{
										 ESPLinkStep = 4; 						 
										 break;
									}
									
									
									
									if(date[22] == '3' ) //  建立链接
									{
										 ESPLinkStep = 5; 
										 break;
									}
									
									if(date[22] == '4' ) // 网络断开
									{
										 ESPLinkStep = 4; 	
										 break;
									}
									
									
								}while(0);
				        break;
								
			 case 5:
				       if( strncmp("CLOSED",date,6) == 0)
							 {
							    ESPLinkStep = 1;
							 }
               break;				 
				        
		 }
       
} 


