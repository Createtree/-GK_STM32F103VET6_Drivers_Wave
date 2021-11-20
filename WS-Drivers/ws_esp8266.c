/******************************************************************************
 * @file     				 ws_esp8266.c
 * @brief    				 esp8266 ��ز���
 * @version  				 1.0.0
 * @creationTime     2020_06_23
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/

#include "ws_esp8266.h"

static  int ESPLinkStep = 1;
extern  UART_HandleTypeDef huart1;


/******************************************************************************
	*�������ƣ� WS_ESP8266_Send_AT
	*���ܣ�     ESP8266����ATָ��
	*������     ��
	*����:			��
*******************************************************************************/

void  WS_ESP8266_Transmit_AT(char  * str)
{
      WS_UART_Transmit_String(&huart1 ,  WS_Format("%s\r\n",str));
}



/******************************************************************************
	*�������ƣ� WS_OS_Task_ESP8266_Link
	*���ܣ�     ESP8266��������
	*������     ��
	*����:			��
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
				        return 400;   //  4���ּ�����ѯ����״̬
			 
			 
			 case 4:
				       WS_ESP8266_Transmit_AT( "AT+CIPSTART=\"TCP\",\"192.168.1.101\",60000");
			         ESPLinkStep = 1 ;  
				       return 300;   //  3���ּ�����ѯ����״̬
			 
			 case 5:
				       break;
		 }
	   return 9  ;  
}


/******************************************************************************
	*�������ƣ� WS_ESP8266_Receive_Callback
	*���ܣ�     ESP8266���յ����ݻص�����
	*������     ��
	*����:			��
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
									
									if(date[22] == '5' ) //  ��·δ����
									{
										 ESPLinkStep = 2; 						 
										 break;
									}
									
									if(date[22] == '2' ) //  ��·����,δ����TCP
									{
										 ESPLinkStep = 4; 						 
										 break;
									}
									
									
									
									if(date[22] == '3' ) //  ��������
									{
										 ESPLinkStep = 5; 
										 break;
									}
									
									if(date[22] == '4' ) // ����Ͽ�
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


