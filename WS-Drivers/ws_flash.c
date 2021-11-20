
/******************************************************************************
 * @file     				 ws_flash.c
 * @brief    				 操作芯片内部flash
 * @version  				 1.0.0
 * @creationTime     2020_03_14
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#include "ws_flash.h"



/******************************************************************************
	*方法名称： HAL_StatusTypeDef  WS_Config_Write_Struct(uint8_t *buffer  , int size)
	*功能：     将系统配置参数结构体写入flash静态保存
	*参数：     p_buffer：待写数据缓存地址
	            size   ： 数据长度，单位：字节
	*返回:			无
*******************************************************************************/
HAL_StatusTypeDef WS_Config_Write_Struct(uint8_t *p_buffer  , int size )
{	
	  WS_Flash_Erase_Page(WS_Flash_Config_Page_Addr,WS_Flash_Config_Page_Addr);  
	  return  WS_Flash_Write_Page(WS_Flash_Config_Page_Addr,p_buffer,size);
}

/******************************************************************************
	*方法名称： HAL_StatusTypeDef   WS_Flash_Write_Page(uint8_t *buffer  , int size)
	*功能：     擦除指定页，支持多页擦除，指定擦除起始页到结束页,
	*参数：     startPage ： 擦除的起始页
	            endPage   ： 数据长度，单位：字节
                           如果只擦除一页，则起始页和结束页一样
	*返回:			无
*******************************************************************************/
HAL_StatusTypeDef WS_Flash_Erase_Page(int startPage , int endPage )
{
	  
	
	  FLASH_EraseInitTypeDef   erase;
	  HAL_StatusTypeDef stu ;
	  uint32_t  pageError           ;
	  //uint32_t  writeAddr  ;
	  //writeAddr =  FLASH_BASE + WS_Flash_Page_Size * (startPage) ;
	
	  //  输入参数校验
	  if(endPage < startPage)  return HAL_ERROR;
	
	  endPage ++;
	 
	  HAL_FLASH_Unlock();  //  解锁flash
	
	  while(startPage < endPage)
		{
	  
			erase.TypeErase =  FLASH_TYPEERASE_PAGES;  //  页擦出
			erase.NbPages   =   1 ;                     //  擦出一页
			erase.PageAddress = FLASH_BASE + WS_Flash_Page_Size * (startPage) ;             //  开始地址
			
			stu = HAL_FLASHEx_Erase(&erase,&pageError) ;
			if(  stu == HAL_OK)      //  擦除一页
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
	  
	  
	  HAL_FLASH_Lock();   //  上锁  flash
		
	
		
	  return  stu;
}


/******************************************************************************
	*方法名称： HAL_StatusTypeDef  WS_Flash_Erase_Write_Page(uint8_t *buffer  , int size)
	*功能：     将缓存数据写入指定页 ,先擦除再写入
  *参数：     page:     写入数据的页编号
              p_buffer：待写数据缓存地址
	            size   ： 数据长度，单位：字节 ， 
                        不能超过单页长度，否则自动丢弃过长的数据
	*返回:			无
*******************************************************************************/
HAL_StatusTypeDef WS_Flash_Erase_Write_Page(int page , uint8_t *p_buffer  , int size )
{
    int i , len = size         ;
	  uint8_t *p_bufferStartAddr =  p_buffer ;
	  FLASH_EraseInitTypeDef   erase;
	  uint32_t  pageError           ;
	  uint32_t  writeAddr  ;
	  writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
	  
	 
	
	  HAL_FLASH_Unlock();  //  解锁flash
	
	  //   强行限制写入长度不能操作一页
	  if(size > WS_Flash_Page_Size) size = WS_Flash_Page_Size ; 
	  size   +=  3;    //  字节长度转为字长度，不满一个字以一个字计算
	  size   /=  4;
	  
	  erase.TypeErase =  FLASH_TYPEERASE_PAGES;  //  页擦出
	  erase.NbPages   =   1;                     //  擦出一页
	  erase.PageAddress = writeAddr;             //  开始地址  
	  if(  HAL_FLASHEx_Erase(&erase,&pageError) == HAL_OK)      //  擦除一页
		{
		   //WS_Debug(1,"%d page erase true\r\n");
		}
		else
		{
		   WS_Debug(1,"%d page erase false\r\n");
		}
	  //  写入数据 
	  for(i = 0 ; i < size ; i++ )
		{
		   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,writeAddr,*(uint32_t *)p_buffer) ; 
			 writeAddr   += 4;
			 p_buffer    += 4;
		}	
		//  校验写入的数据
		writeAddr =  FLASH_BASE + WS_Flash_Page_Size * page ;
		if( memcmp(p_bufferStartAddr,(const void *)writeAddr,len ) == 0   )
		{
		   //WS_Debug(1,"%d page write true\r\n");
		}
		else
		{
		   WS_Debug(1,"%d page write false\r\n");
		}  
	  HAL_FLASH_Lock();   //  上锁  flash	
		
		WS_OS_EN_INTERRUPT 
	  return  HAL_OK;
}


/******************************************************************************
	*方法名称： HAL_StatusTypeDef  WS_Flash_Write(uint8_t *buffer  , int size)
	*功能：     将缓存数据写入指定页 ,必须提前调用擦除函数擦除指定的页
  *参数：     page:     写入数据的页编号
              p_buffer：待写数据缓存地址
	            size   ： 数据长度，单位：字节
	*返回:			无
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
	 
	  HAL_FLASH_Unlock();  //  解锁flash
	 
	  //   强行限制写入长度不能操作一页
	  //if(size > WS_Flash_Page_Size) size = WS_Flash_Page_Size ; 
	  size   +=  3;    //  字节长度转为字长度，不满一个字以一个字计算
	  size   /=  4;
	
	 
	  for(i = 0 ; i < size ; i++ )
		{
			 memcpy(word.ch,p_buffer,4);
		   stu =   HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,writeAddr,word.num) ; 
			 writeAddr   += 4;
			 p_buffer    += 4;
		}
		
		//  校验写入的数据
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
		
	  
	  HAL_FLASH_Lock()   ;   //  上锁  flash
		
		
	  return  stu;
}


/******************************************************************************
	*方法名称： HAL_StatusTypeDef  WS_Config_Read_Struct(uint8_t *buffer  , int size)
	*功能：     将配置结构体写入flash静态保存
	*参数：     p_buffer：保存数据缓存地址
	            size   ： 数据长度，单位：字节
	*返回:			无
*******************************************************************************/

HAL_StatusTypeDef WS_Config_Read_Struct(uint8_t *buffer  , int size )
{
	return  WS_Flash_Read_Page(WS_Flash_Config_Page_Addr,buffer,size);	
}

/******************************************************************************
	*方法名称： HAL_StatusTypeDef  WS_Config_Read_Struct(uint8_t *buffer  , int size)
	*功能：     将配置结构体写入flash静态保存
	*参数：     p_buffer：保存数据缓存地址
	            size   ： 数据长度，单位：字节
	*返回:			无
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



