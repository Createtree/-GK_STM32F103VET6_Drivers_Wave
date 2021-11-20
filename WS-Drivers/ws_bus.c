
/******************************************************************************
 * @file     				 ws_stm32_drive_borad.c
 * @brief    				 WS_BUSͨ��Э����ش���
 * @version  				 1.0.0
 * @creationTime     2020_03_18
 * @creator					 
 * @readme
 *        1. ����֡�ı���  
 *				2. ����֡����
          3. 
*******************************************************************************/

#include "ws_bus.h"

//   �ⲿ��������
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;


unsigned int short WS_Bus_CRC16_Check(unsigned char *pdat, unsigned  int len)
{
      unsigned int  i;
      unsigned int  j;
      unsigned int short  crc = 0xFFFF;
      for (i=0; i<len ; i++)
      {
        crc ^= ((*pdat++) & 0x00FF);
        for (j=0; j<8; j++)
        {
          if (crc & 0x0001)
          {
            crc >>= 1;
            crc ^= 0xA001;
          }
          else
          {
            crc >>= 1;
          }
        }
      }
     return crc ;
}


/******************************************************************************
	*�������ƣ� WS_BUS_Transmit_Command
	*���ܣ�     ����ͨ��Э�鷢����������
	*������     
              Addr �� Ŀ���ַ
              cmd  �� ����
							len  :  ���ݳ���
							date :  ���ݻ����ַ
	*����:		  �Ƿ��ͳɹ�
*******************************************************************************/
HAL_StatusTypeDef WS_BUS_Transmit_Command(uint16_t addr,uint16_t cmd , uint16_t  len ,char *date )
{  
	  static   char  dateBuffer[138]  = "W00000000S1234"    ; 
	  unsigned int short crc;
	  dateBuffer[1] = addr&0x00ff  ;  dateBuffer[2] = addr>>8  ;
	  dateBuffer[3] = cmd&0x00ff  ;   dateBuffer[4] = cmd>>8  ;
	  dateBuffer[5] = len&0x00ff  ;   dateBuffer[6] = len>>8  ;
	  dateBuffer[7] = 0;              dateBuffer[8] = 0;
	
	  memcpy(dateBuffer+10,date,len);
	  crc = WS_Bus_CRC16_Check((unsigned char *)dateBuffer , len + 10)  ;
    dateBuffer[7] = crc&0x00ff    ;   dateBuffer[8] = crc>>8  ;
	
	  return  WS_Bus_Transmit_Byte(dateBuffer,len + 10);
}


/******************************************************************************
	*�������ƣ� WS_BUS_Transmit_Command_String
	*���ܣ�     ����ͨ��Э�鷢����������,����Ϊ�ַ���
	*������     
              Addr �� Ŀ���ַ
              cmd  �� ����
							len  :  ���ݳ���
							date :  ���ݻ����ַ
	*����:		  �Ƿ��ͳɹ�
*******************************************************************************/
HAL_StatusTypeDef WS_BUS_Transmit_Command_String(uint16_t addr,uint16_t cmd , char *date )
{  
    int len = strlen(date);	
    return  WS_BUS_Transmit_Command(addr,cmd,len,date);
}


/******************************************************************************
	*�������ƣ� WS_BUS_Parser_Command
	*���ܣ�     ����ͨ��Э��������ݣ������Ƹ�ʽ��
	*������     
              hbus �� ����֡�ṹ
							date :  ԭʼ���ݻ����ַ
	*����:		  �Ƿ��ͳɹ�
*******************************************************************************/
HAL_StatusTypeDef WS_BUS_Parser_Command(WS_BusTypeDef *hbus,char *date, int size)
{  
	  unsigned int short crc = 0 ;
	  size = size;
	
	   //  ����ESP8266����ͷ
	  if(memcmp(date,"+IPD",4)  == 0)
		{
		   date +=7 ;
			 while(*date >= '0' && *date <= '9')
			 {
			    date ++;
				  
			 }
			 date ++;
		}
	
	  if(date[0] == 'W' && date[9] == 'S') //  ֡ͷ֡β�ж�
		{
		    hbus->addr = date[2]<<8  ;    hbus->addr |= date[1] ;  
				hbus->cmd  = date[4]<<8  ;    hbus->cmd  |= date[3] ;  
				hbus->len  = date[6]<<8  ;    hbus->len  |= date[5] ;  
				hbus->crc  = date[8]<<8  ;    hbus->crc  |= date[7] ;  			

			  date[7] = 0; date[8] = 0  ;		         	  
			  hbus->pBuffPtr = date + 10;
			
			  crc = WS_Bus_CRC16_Check((unsigned char *)date,size);
			
			  if(crc == hbus->crc)
				{			 
					  return  HAL_OK ;			  
				}
				else
				{
				    //WS_BUS_Transmit_Command_String(0x0,0xffff,WS_Format("MCU CRC ERR :%x",crc));
				}
		}	  
    return  HAL_ERROR;
}

/******************************************************************************
	*�������ƣ� WS_BUS_Parser_Command_String
	*���ܣ�     ����ͨ��Э��������ݣ��ַ�����ʽ��
	*������     
              hbus �� ����֡�ṹ
							date :  ԭʼ���ݻ����ַ
	*����:		  �Ƿ��ͳɹ�
*******************************************************************************/
HAL_StatusTypeDef WS_BUS_Parser_Command_String(WS_BusTypeDef *hbus,char *date, int size)
{  
	  //unsigned int short crc = 0 ;
	  char     buf[4] = {0,0,0,0};
	  size = size;
	  
	  //  ����ESP8266����ͷ
//	  if(memcmp(date,"+IPD",4)  == 0)
//		{
//		   date +=7 ;
//			 while(*date >= '0' && *date <= '9')
//			 {
//			    date ++;
//				  
//			 }
//			 date ++;
//		}
	
	  if(date[0] == 'W' && date[9] == 'S') //  ֡ͷ֡β�ж�
		{
			
			  buf[0]  = date[1] ;   buf[1]  = date[2] ; 
			  hbus->addr = atoi(buf);
			
			  buf[0]  = date[3] ;   buf[1]  = date[4] ; 
			  hbus->cmd = atoi(buf);
			
			  buf[0]  = date[5] ;   buf[1]  = date[6] ; 
			  hbus->len = atoi(buf);
			  
		    if(hbus->len == strlen(date + 10))
				{
				   hbus->pBuffPtr = date + 10;
					 return  HAL_OK            ;
				}					
		}	  
    return  HAL_ERROR;
}


/******************************************************************************
	*�������ƣ� WS_BUS_Run_Command
	*���ܣ�     ��������
	*������     
              hbus �� ����֡�ṹ
							date :  ԭʼ���ݻ����ַ
	*����:		  �Ƿ��ͳɹ�
*******************************************************************************/

void   WS_BUS_Run_Command(WS_BusTypeDef *hbus)
{
       switch(hbus->cmd)
			 {
				 case 0: //WS_Config_String_Write_System_Info(hbus->pBuffPtr);
					       break;
				 
				 case 1: //WS_Config_String_Write_hstep1(hbus->pBuffPtr); 
					       break;
				 
				 case 2:
					       break;
			 }
}




