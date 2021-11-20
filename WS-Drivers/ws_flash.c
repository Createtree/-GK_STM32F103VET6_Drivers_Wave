
/******************************************************************************
 * @file     				 ws_flash.c
 * @brief    				 ����оƬ�ڲ�flash
 * @version  				 1.0.0
 * @creationTime     2020_03_14
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#include "ws_flash.h"



/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef  WS_Config_Write_Struct(uint8_t *buffer  , int size)
	*���ܣ�     ��ϵͳ���ò����ṹ��д��flash��̬����
	*������     p_buffer����д���ݻ����ַ
	            size   �� ���ݳ��ȣ���λ���ֽ�
	*����:			��
*******************************************************************************/
HAL_StatusTypeDef WS_Config_Write_Struct(uint8_t *p_buffer  , int size )
{	
	  WS_Flash_Erase_Page(WS_Flash_Config_Page_Addr,WS_Flash_Config_Page_Addr);  
	  return  WS_Flash_Write_Page(WS_Flash_Config_Page_Addr,p_buffer,size);
}

/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef   WS_Flash_Write_Page(uint8_t *buffer  , int size)
	*���ܣ�     ����ָ��ҳ��֧�ֶ�ҳ������ָ��������ʼҳ������ҳ,
	*������     startPage �� ��������ʼҳ
	            endPage   �� ���ݳ��ȣ���λ���ֽ�
                           ���ֻ����һҳ������ʼҳ�ͽ���ҳһ��
	*����:			��
*******************************************************************************/
HAL_StatusTypeDef WS_Flash_Erase_Page(int startPage , int endPage )
{
	  
	
	  FLASH_EraseInitTypeDef   erase;
	  HAL_StatusTypeDef stu ;
	  uint32_t  pageError           ;
	  //uint32_t  writeAddr  ;
	  //writeAddr =  FLASH_BASE + WS_Flash_Page_Size * (startPage) ;
	
	  //  �������У��
	  if(endPage < startPage)  return HAL_ERROR;
	
	  endPage ++;
	 
	  HAL_FLASH_Unlock();  //  ����flash
	
	  while(startPage < endPage)
		{
	  
			erase.TypeErase =  FLASH_TYPEERASE_PAGES;  //  ҳ����
			erase.NbPages   =   1 ;                     //  ����һҳ
			erase.PageAddress = FLASH_BASE + WS_Flash_Page_Size * (startPage) ;             //  ��ʼ��ַ
			
			stu = HAL_FLASHEx_Erase(&erase,&pageError) ;
			if(  stu == HAL_OK)      //  ����һҳ
			{
				 //WS_Debug(1,"%d page erase true\r\n",startPage);
				 //WS_Delay_MS(20);
			}
			else
			{
				 WS_Debug(1,"%d~%d page erase false\r\n",startPage,endPage-1);
			   return stu ;
			}
			startPage ++;
	  }
	  
	  
	  HAL_FLASH_Lock();   //  ����  flash
		
	
		
	  return  stu;
}


/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef  WS_Flash_Erase_Write_Page(uint8_t *buffer  , int size)
	*���ܣ�     ����������д��ָ��ҳ ,�Ȳ�����д��
  *������     page:     д�����ݵ�ҳ���
              p_buffer����д���ݻ����ַ
	            size   �� ���ݳ��ȣ���λ���ֽ� �� 
                        ���ܳ�����ҳ���ȣ������Զ���������������
	*����:			��
*******************************************************************************/
HAL_StatusTypeDef WS_Flash_Erase_Write_Page(int page , uint8_t *p_buffer  , int size )
{
    int i , len = size         ;
	  uint8_t *p_bufferStartAddr =  p_buffer ;
	  FLASH_EraseInitTypeDef   erase;
	  uint32_t  pageError           ;
	  uint32_t  writeAddr  ;
	  writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
	  
	 
	
	  HAL_FLASH_Unlock();  //  ����flash
	
	  //   ǿ������д�볤�Ȳ��ܲ���һҳ
	  if(size > WS_Flash_Page_Size) size = WS_Flash_Page_Size ; 
	  size   +=  3;    //  �ֽڳ���תΪ�ֳ��ȣ�����һ������һ���ּ���
	  size   /=  4;
	  
	  erase.TypeErase =  FLASH_TYPEERASE_PAGES;  //  ҳ����
	  erase.NbPages   =   1;                     //  ����һҳ
	  erase.PageAddress = writeAddr;             //  ��ʼ��ַ  
	  if(  HAL_FLASHEx_Erase(&erase,&pageError) == HAL_OK)      //  ����һҳ
		{
		   //WS_Debug(1,"%d page erase true\r\n");
		}
		else
		{
		   WS_Debug(1,"%d page erase false\r\n");
		}
	  //  д������ 
	  for(i = 0 ; i < size ; i++ )
		{
		   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,writeAddr,*(uint32_t *)p_buffer) ; 
			 writeAddr   += 4;
			 p_buffer    += 4;
		}	
		//  У��д�������
		writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
		if( memcmp(p_bufferStartAddr,(const void *)writeAddr,len ) == 0   )
		{
		   //WS_Debug(1,"%d page write true\r\n");
		}
		else
		{
		   WS_Debug(1,"%d page write false\r\n");
		}  
	  HAL_FLASH_Lock();   //  ����  flash	
		
		WS_OS_EN_INTERRUPT 
	  return  HAL_OK;
}


/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef  WS_Flash_Write(uint8_t *buffer  , int size)
	*���ܣ�     ����������д��ָ��ҳ ,������ǰ���ò�����������ָ����ҳ
  *������     page:     д�����ݵ�ҳ���
              p_buffer����д���ݻ����ַ
	            size   �� ���ݳ��ȣ���λ���ֽ�
	*����:			��
*******************************************************************************/
HAL_StatusTypeDef WS_Flash_Write_Page(int page , uint8_t *p_buffer  , int size )
{
    int i , len = size         ;
	  union
		{
		  uint8_t ch[4]   ;
			uint32_t num    ;
		} word;
	  
	  uint8_t *p_bufferStartAddr =  p_buffer ;
	  HAL_StatusTypeDef stu;
	 
	  uint32_t  writeAddr  ;
	  writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
	 
	  HAL_FLASH_Unlock();  //  ����flash
	 
	  //   ǿ������д�볤�Ȳ��ܲ���һҳ
	  //if(size > WS_Flash_Page_Size) size = WS_Flash_Page_Size ; 
	  size   +=  3;    //  �ֽڳ���תΪ�ֳ��ȣ�����һ������һ���ּ���
	  size   /=  4;
	
	 
	  for(i = 0 ; i < size ; i++ )
		{
			 memcpy(word.ch,p_buffer,4);
		   stu =   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,writeAddr,word.num) ; 
			 writeAddr   += 4;
			 p_buffer    += 4;
		}
		
		//  У��д�������
		writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
		if( memcmp(p_bufferStartAddr,(const void *)writeAddr,len ) == 0   )
		{
		 //  WS_Debug(1,"%d page write true\r\n",page-WS_Flash_APP_Start_Page);
		 //  WS_Delay_MS(10);
		}
		else
		{
		   WS_Debug(1,"%d page write false\r\n",page);
		}
		
	  
	  HAL_FLASH_Lock()   ;   //  ����  flash
		
		
	  return  stu;
}


/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef  WS_Config_Read_Struct(uint8_t *buffer  , int size)
	*���ܣ�     �����ýṹ��д��flash��̬����
	*������     p_buffer���������ݻ����ַ
	            size   �� ���ݳ��ȣ���λ���ֽ�
	*����:			��
*******************************************************************************/

HAL_StatusTypeDef WS_Config_Read_Struct(uint8_t *buffer  , int size )
{
	return  WS_Flash_Read_Page(WS_Flash_Config_Page_Addr,buffer,size);	
}

/******************************************************************************
	*�������ƣ� HAL_StatusTypeDef  WS_Config_Read_Struct(uint8_t *buffer  , int size)
	*���ܣ�     �����ýṹ��д��flash��̬����
	*������     p_buffer���������ݻ����ַ
	            size   �� ���ݳ��ȣ���λ���ֽ�
	*����:			��
*******************************************************************************/

HAL_StatusTypeDef WS_Flash_Read_Page(int page , uint8_t *buffer  , int size )
{
	uint32_t endAddr   =  FLASH_BASE + WS_Flash_Page_Size * page +  size;
	uint32_t startAddr = FLASH_BASE + WS_Flash_Page_Size * page ;  ;
	int i=0 ;
	
	while((startAddr < endAddr))
	{
		buffer[i++]=(*(uint8_t*) startAddr);
		startAddr ++;	
	}	
	return HAL_OK;	
}



