
/******************************************************************************
 * @file     				 ws_borad_base.c
 * @brief    				 ����оƬ�ڲ�����Χ��Դ����
 * @version  				 1.0.0
 * @creationTime     2020_08_15
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#include "ws_borad_base.h"
#include "usb_device.h"

//#include "usb_device.h"
//#include "stm32f0xx_hal_def.h"






//   �ⲿ��������
extern  UART_HandleTypeDef huart1;
extern  UART_HandleTypeDef huart3;
extern  UART_HandleTypeDef huart2;
extern  uint32_t SystemCoreClock ;

extern  SD_HandleTypeDef hsd;

extern  TIM_HandleTypeDef htim3;
extern  TIM_HandleTypeDef htim5;
extern  TIM_HandleTypeDef htim7;

extern  SPI_HandleTypeDef hspi2;

extern   RTC_HandleTypeDef hrtc;

extern  ADC_HandleTypeDef hadc1;
extern  ADC_HandleTypeDef hadc2;



//extern  SD_HandleTypeDef hsd;


extern  osMessageQId UART_Receiver_MsgHandle;

//     MP3����״̬��ֵ�ź�
extern osSemaphoreId WS_Audio_Idle_SemaphoreHandle;


//  ȫ�ֱ���

uint16_t  touchOriginal_X = 0;
uint16_t  touchOriginal_Y = 0;


extern uint32_t ex1_high;//�沶���ֵ
extern uint32_t ex1_low;//�沶���ֵ
extern uint32_t ex1_flag;//�жϼ�����־λ







/* USER CODE BEGIN Header_WS_Borad_Drivers_Thread */
/**
* @brief Function implementing the WS_Borad_Drivers thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_WS_Borad_Drivers_Task */
void WS_FreeThread_Borad(void const * argument)
{
	
	//MX_USB_DEVICE_Init() ;  
	
  osDelay(1000)  ;
	
	WS_Borad_Init();     
  /* Infinite loop */
  for(;;)
  {
		//WS_Borad_Update_Input_Output() ;
		//WS_OS_Task_System_Input_Check();
    osDelay(50);
  } 
}



/******************************************************************************
	*�������ƣ� WS_Borad_Close_RunLed(void)
	*���ܣ�     ��ʼ��������Դ
	*������     ��
	*����:			��
*******************************************************************************/
void WS_Borad_Init(void)
{
	
  uint8_t  *p_uartbuffer ;
	
  //	uint16_t  i;
	
  

	//  ʹ��RTC���ж�
	__HAL_RTC_ALARM_ENABLE_IT(&hrtc , RTC_IT_SEC);    
	
	//  ʹ�ܸ����ж�
	HAL_TIM_Base_Start_IT(&htim5)     ;
	HAL_TIM_Base_Start_IT(&htim7)	  ;
//	HAL_TIM_Base_Start_IT(&htim3)   ;
	
	
	
	
	//  ADC ��ʼ��
	HAL_ADCEx_Calibration_Start(&hadc1); //  У׼ADC1
	HAL_ADCEx_Calibration_Start(&hadc2); //  У׼ADC2
	
  


	
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);	
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);	
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	
  __HAL_UART_CLEAR_IDLEFLAG(&huart3);	
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
	
	
	// 1�� ������ջ��棻 2����������DMA����
	p_uartbuffer = pvPortMalloc(USART1_RECEIVE_BUFFER_SIZE);
  HAL_UART_Receive_DMA(&huart1,(uint8_t *)p_uartbuffer,USART1_RECEIVE_BUFFER_SIZE)   ;
	
	p_uartbuffer = pvPortMalloc(USART2_RECEIVE_BUFFER_SIZE);
  HAL_UART_Receive_DMA(&huart2,(uint8_t *)p_uartbuffer,USART2_RECEIVE_BUFFER_SIZE)   ;
	
	p_uartbuffer = pvPortMalloc(USART3_RECEIVE_BUFFER_SIZE);
  HAL_UART_Receive_DMA(&huart3,(uint8_t *)p_uartbuffer,USART3_RECEIVE_BUFFER_SIZE)   ; 				  

  
	//   ���ڷ��Ͳ���
	WS_UART_Transmit_String(&huart1,"USART1 start\r\n")   ;
	WS_RS485_Transmit_String("RS485 start\r\n") ;
	WS_UART_Transmit_String(&huart3,"USART3 start\r\n")   ;
	
	//WS_RS485_Transmit_String("RS485 start\r\n")         ;
  osDelay(10);
	//  ϵͳ��ʱ��
	WS_Debug(1,"HCLK = %dM\r\n",HAL_RCC_GetHCLKFreq()/1000000); 
	osDelay(10)     ;
	
	
	WS_W25Qxx_Init() ;            //  SPI FLASH init
	osDelay(50)      ;
	
	WS_System_Config_Init();      //  ������Ϣ��ʼ��
	osDelay(50)  ;
		
  WS_W25Qxx_Font_Chenk_Write(); //  �ֿ�����д��
	osDelay(50)  ;
	
	WS_TFT_Init();                //   TFT ��ʼ�� 
  osDelay(50)  ;
	
	
	//  ���� SPI FLASH
	FRESULT   retFlash;
	retFlash = f_mount(&USERFatFS, USERPath, 1);
	if(retFlash)
	{
			WS_Debug(1,"spi flash mount error\r\n") ;	
		  WS_Debug(1,"spi flash start mkfs\r\n")  ;	
		  retFlash =  f_mkfs("1:",0,512)          ;	       

		//  ��ʽ��
		  if(retFlash)
			{
					WS_Debug(1,"spi flash mkfs error\r\n")   ;	 
			} 
      else
			{ 
			    WS_Debug(1,"spi flash mkfs ok\r\n");
      		f_setlabel((const TCHAR *)"1:SPI FLASH");		// �޸��̷�����
			}		
	}
	else
	{
		  WS_Debug(1,"spi flash mount OK   \r\n")  ;	
		  FRESULT 	res ; 
	   
      char fontTest[] = "��"   ;  
		  uint8_t buffer[130]      ;
		  UINT br;
			
		  res = f_open(&USERFile,"1:/H_16.Dzk",FA_READ); 
			if(res!=FR_OK)
			{
				WS_TFT_Dis_8X16_String(0,116,"H_16.dzk NOT OPEN",WHITE ,RED);
				
			}else{
				WS_TFT_Dis_8X16_String(0,116,WS_Format("H_16.dzk Open size: %d",USERFile.fsize),WHITE ,RED); 
				f_lseek(&USERFile , ((fontTest[0]-0xa1) * 94  +  (fontTest[1]- 0xa1) )*32);
				res =  f_read(&USERFile,buffer,32,&br);
				if(res == FR_OK )
				{
				   WS_TFT_Dis_16X16_Date(50,200,buffer,WHITE,RED);
				}
				f_close(&USERFile);
			}
			
			res = f_open(&USERFile,"1:H_24.dzk",FA_READ); 
			if(res!=FR_OK)
			{
				WS_TFT_Dis_8X16_String(0,132,"H_24.dzk NOT OPEN",WHITE ,RED);
				
			}else{
				WS_TFT_Dis_8X16_String(0,132,WS_Format("H_24.dzk Open size: %d",USERFile.fsize),WHITE ,RED); 
				f_lseek(&USERFile , ((fontTest[0]-0xa1) * 94  +  (fontTest[1]- 0xa1) )*72);
				res =  f_read(&USERFile,buffer,72,&br);
				if(res == FR_OK )
				{
				   WS_TFT_Dis_24X24_Date(70,200,buffer,WHITE,RED);
				}
				f_close(&USERFile);
			}
			
			res = f_open(&USERFile,"1:/H_32.dzk",FA_READ); 
			if(res!=FR_OK)
			{
				WS_TFT_Dis_8X16_String(0,148,"H_32.dzk NOT OPEN",WHITE ,RED);
				
			}else{
				WS_TFT_Dis_8X16_String(0,148,WS_Format("H_32.dzk Open size: %d",USERFile.fsize),WHITE ,RED); 
				f_lseek(&USERFile , ((fontTest[0]-0xa1) * 94  +  (fontTest[1]- 0xa1) )*128);
				res =  f_read(&USERFile,buffer,128,&br);
				if(res == FR_OK )
				{
				   WS_TFT_Dis_32X32_Date(100,200,buffer,WHITE,RED);
				}
				f_close(&USERFile);
			}
	}




//  uint8_t  bf[4096] ;
//	for(i=0;i<4096;i+=4)
//  {
//	   *(uint32_t *)&bf[i] = i;
//	}
//	
//	
//	WS_W25Qxx_Write(bf,(4096-5) * 4096 + 500 ,4096);
//	
//	memset(bf,0,4096);
//	
//	WS_W25Qxx_Read (bf,(4096-5) * 4096 + 500 ,4096);
//	
//	
//	for(i=0;i<4096;i+=4)
//  {
//	  if( *(uint32_t *)&bf[i]  !=  i)
//		{
//		   WS_TFT_Dis_8X16_String(0,100,"W25Qxx WS_W25Qxx_Write error",WHITE, RED);
//			 break;
//		}
//	}
//	if(i == 4096)
//	{
//	   WS_TFT_Dis_8X16_String(0,100,"W25Qxx WS_W25Qxx_Write OK",WHITE, RED);
//	}
	
	
	WS_Borad_Open_RunLed_Time(10);
	WS_Borad_Open_DebugLed_Time(10);
	WS_Borad_Open_Buzzer_Time(10);
	
	
}



/******************************************************************************
	*�������ƣ� WS_Borad_Close_RunLed(void)
	*���ܣ�     �رհ������е�
	*������     ��
	*����:			��
*******************************************************************************/

int  WS_Borad_Close_RunLed(void *data)                     
{
	   data = data ;	
     closeRunLed();
	   return 0;
}
/******************************************************************************
	*�������ƣ� char WS_Borad_Open_RunLed_Time(int time)
	*���ܣ�     �򿪰������еƣ�time �����ر�
	*������     time�� ��ʱ�䣬��λ MS
*����:		    0: �ɹ� ��> 0 : ʧ��
*******************************************************************************/
char WS_Borad_Open_RunLed_Time(int time)
{
      openRunLed();
      return  CreateTimer(WS_Borad_Close_RunLed,time);  
}

/******************************************************************************
	*�������ƣ� WS_Borad_Close_DebugLed(void)
	*���ܣ�     �رհ��ص��Ե�
	*������     ��
	*����:			��
*******************************************************************************/
int  WS_Borad_Close_DebugLed(void * data)                     
{
	   data = data ;	
     closeDebugLed();
	   return 0;
}
/******************************************************************************
	*�������ƣ� char openDebugLedTime(int time)
	*���ܣ�     �򿪰��ص��Եƣ�time �����ر�
*������       time: ��ʱ�䣬��λ MS
	*����:		  0: �ɹ� ��> 0 : ʧ��
*******************************************************************************/
char WS_Borad_Open_DebugLed_Time(int time)
{
      openDebugLed();
      return  CreateTimer(WS_Borad_Close_DebugLed,time);   
}



/******************************************************************************
	*�������ƣ� WS_Borad_Close_Buzzer(void)
	*���ܣ�     �رշ�����
	*������     ��
	*����:		  
*******************************************************************************/
int  WS_Borad_Close_Buzzer(void * data)                     
{
	   data = data ;	
     WS_define_Borad_Close_Buzzer 
	   return 0;
}
/******************************************************************************
	*�������ƣ� char WS_Borad_Open_Buzzer_Time(int time)
	*���ܣ�     �򿪰��ط�������time �����ر�
*������       time: ��ʱ�䣬��λ MS
	*����:		  0: �ɹ� ��> 0 : ʧ��
*******************************************************************************/
char WS_Borad_Open_Buzzer_Time(int time)
{

      WS_define_Borad_Open_Buzzer
      return  CreateTimer(WS_Borad_Close_Buzzer,time);  
}
/********************************************************************/






/******************************************************************************
	*�������ƣ� WS_UART_Transmit_String_DMA
	*���ܣ�     ���ݴ���Ĵ��ڿ��ƽṹָ�룬�����ַ��������ڣ�DMA����
	*������     huart�����ڿ��ƽṹ��ָ��
              p_string �� �ַ�����ַ
	*����:		  ��
*******************************************************************************/
HAL_StatusTypeDef WS_UART_Transmit_String(UART_HandleTypeDef *huart, char *BuffAddr)
{
   return   WS_UART_Transmit_Byte(huart,BuffAddr,strlen(BuffAddr));
}

/******************************************************************************
	*�������ƣ� WS_UART_Transmit_Byte_DMA
	*���ܣ�     ���ݴ���Ĵ��ڿ��ƽṹָ�룬�������ݵ����ڣ�DMA����
	*������     huart�����ڿ��ƽṹ��ָ��
              BuffAddr �� ���ݵ�ַ
              size     :  �������ݳ���
	*����:		  ��
*******************************************************************************/
HAL_StatusTypeDef WS_UART_Transmit_Byte(UART_HandleTypeDef *huart, char *BuffAddr , int size)
{
   return   HAL_UART_Transmit(huart,(uint8_t *)BuffAddr,size,size/10 + 1); // 
}


/******************************************************************************
	*�������ƣ� WS_RS485_Transmit_Byte
	*���ܣ�     ͨ��S485�ӿڴ�������
	*������     
              BuffAddr �� ���ݵ�ַ
              size     :  �������ݳ���
	*����:		  ��
******************************************************************************/
HAL_StatusTypeDef WS_RS485_Transmit_Byte( uint8_t  *BuffAddr , int size)
{
	 HAL_StatusTypeDef rst ; 
	 WS_define_RS485_TX_EN ;
	 rst =  HAL_UART_Transmit_DMA(&WS_define_RS485_huart,(uint8_t *)BuffAddr,size);   // 
  //rst =  HAL_UART_Transmit(&WS_define_RS485_huart,(uint8_t *)BuffAddr,size,size/10+1); 
	//WS_define_RS485_TX_DI ;
	 return   rst ; 
}

/******************************************************************************
	*�������ƣ� WS_RS485_Transmit_String
	*���ܣ�     ͨ��RS485�ӿڴ����ַ���
	*������     
              BuffAddr �� ���ݵ�ַ
              size     :  �������ݳ���
	*����:		  ��
*******************************************************************************/
HAL_StatusTypeDef WS_RS485_Transmit_String( char *string)
{
	 
	 return   WS_RS485_Transmit_Byte((uint8_t * )string , strlen(string)) ;
}











/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
  UNUSED(huart);

  //HAL_UART_Transmit_DMA(huart,(uint8_t *)receiveBuf, huart->RxXferSize) ;
	xQueueSendFromISR(UART_Receiver_MsgHandle , &huart,&pxHigherPriorityTaskWoken);

	if(pxHigherPriorityTaskWoken == pdTRUE)
	{
	  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken) ;
	}
}



/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
	if(huart == &WS_define_RS485_huart)  //  RS485���ݷ�����Ϻ�ʹ�ܽ���
	{
	    WS_define_RS485_TX_DI ; 
	}
}



/**
  * @brief  Function implementing the WS_UART_Receiver_Name thread.
  * @param  argument: Not used 
  * @retval None
  */
void WS_FreeThread_UART_Receiver(void const * argument)
{
  UART_HandleTypeDef * huart = &huart1;
	int len = 0;
  /* Infinite loop */
  for(;;)
  {
    if(xQueueReceive(UART_Receiver_MsgHandle,&huart,1000000)  )
		{
			
			  len = huart->RxXferSize  - __HAL_DMA_GET_COUNTER(huart->hdmarx);   
			  if(len < huart->RxXferSize)  huart->pRxBuffPtr[len] = 0;
			
			  if(huart == &WS_define_RS485_huart)
				{
				   WS_RS485_Transmit_Byte((uint8_t *)huart->pRxBuffPtr, len);
				}
				else
				{
		      HAL_UART_Transmit_DMA(huart,(uint8_t *)huart->pRxBuffPtr, len) ;
				}
			  
 
			  HAL_UART_Receive_DMA (huart,(uint8_t *)huart->pRxBuffPtr, huart->RxXferSize) ;
		
		}
		//osDelay(10); 
  }
  
}




//       ������������
uint16_t WS_Touch_ReadDate(uint8_t cmd)
{
     uint8_t spi_r , spi_t;
	   uint16_t num ; 
	
	   HAL_GPIO_WritePin(LCD_T_CS_GPIO_Port,LCD_T_CS_Pin,GPIO_PIN_RESET); 
		
     spi_t = cmd ; 	
		 HAL_SPI_TransmitReceive(&hspi2,&spi_t,&spi_r,1,1);  
		
		  //  ��ʱ�ȴ�ת�����
		 num = 200;
     while(num --);
		
     spi_t = 0x00 ; 	 		
     HAL_SPI_TransmitReceive(&hspi2,&spi_t,&spi_r,1,1);	
     num = 	spi_r;
     num <<= 8;
		
		
		 spi_t = 0x00 ; 	
     HAL_SPI_TransmitReceive(&hspi2,&spi_t,&spi_r,1,1);	 		
		 num |= spi_r;
		 
		 num &= ~(1<<16); // �����м�12λ
		 num >>= 3      ;
		  
		 HAL_GPIO_WritePin(LCD_T_CS_GPIO_Port,LCD_T_CS_Pin,GPIO_PIN_SET); 
		 
		 return num ;
		 
}

//   �麯��   ��������ص�����
__weak  void  WS_Touch_ReadDate_Callback(uint16_t x  ,  uint16_t y)
{
         
}

//   ����������ʱ������
int  WS_Touch_ReadTask_Task(void *date)
{
	 uint16_t x , y ; 
   date = date  ;
	 
	 if(HAL_GPIO_ReadPin(LCD_T_PENIRQ_GPIO_Port,LCD_T_PENIRQ_Pin) == GPIO_PIN_RESET)
	 { 		
		 x =  WS_Touch_ReadDate(0xd0) ; 
		 y =  WS_Touch_ReadDate(0x90) ; 
		 
		 
		 WS_Debug(1,"touch x = %d\r\n",x); 
		 WS_Debug(1,"touch y = %d\r\n",y);
		 WS_TFT_Dis_8X16_String(0,88,"��������: ",WHITE, BLACK) ;
	   WS_TFT_Dis_8X16_String(80,88,WS_Format("X:%4d  Y%4d",x,y),WHITE, BLACK) ;
		 
		 WS_Touch_ReadDate_Callback(x,y);
	 }
	 else //  ��������, ��������
	 {
	   return 0;
	 }
	 
	 return 4;  //   50MS����һ�δ�������
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  WS_StatusTypeDef  rtn = WS_Status_ERROR ; 
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	if(GPIO_Pin == GPIO_PIN_11)  //  �������ж�
	{
	   //   
		 //WS_OS_Create_Timer(WS_Touch_ReadTask_Timer , 3); //   30 ms��ͨ����ʱ����ȡ����
     rtn =  WS_OS_Create_Task(WS_OS_ID_Touch_define,WS_Touch_ReadTask_Task,3);
		 if(rtn != WS_Status_OK)
		 {
		    WS_OS_Start_Task(WS_OS_ID_Touch_define,3);
		 }
	}
	
	if(GPIO_Pin == EX1_Pin)
	{
		
	}
	if(GPIO_Pin == EX2_Pin)
	{
			if(HAL_GPIO_ReadPin(EX2_GPIO_Port,EX2_Pin)==GPIO_PIN_SET)//�����ߵ�ƽʱ��
			{
				ex1_low = WS_CCP_Measure_Time(0,2);				//��ʼ��ʱ�ߵ�ƽ		
			}
			else
			{
			    ex1_high = WS_CCP_Measure_Time(0,2);	//�õ��ߵ�ƽʱ��,��ʼ�´μ�ʱ 
				ex1_flag = 1;
				
			}	
	}
}



//  RTC���жϣ� �ص�
void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hrtc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_RTCEx_RTCEventCallback could be implemented in the user file
   */
	
	HAL_RTCEx_BKUPWrite(hrtc,RTC_BKP_DR2,hrtc->DateToUpdate.Year);
	HAL_RTCEx_BKUPWrite(hrtc,RTC_BKP_DR3,hrtc->DateToUpdate.Month);
	HAL_RTCEx_BKUPWrite(hrtc,RTC_BKP_DR4,hrtc->DateToUpdate.Date);
	HAL_RTCEx_BKUPWrite(hrtc,RTC_BKP_DR5,hrtc->DateToUpdate.WeekDay);
}



/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */








