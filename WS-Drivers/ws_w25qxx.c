
#include "ws_w25qxx.h" 

extern           SD_HandleTypeDef hsd                 ;   //  SD�����ƽṹ

extern           SPI_HandleTypeDef W25Qxx_hspi        ;   //  оƬ��ʹ�õ�SPI���

WS_W25QxxTypeDef WS_W25Qxx_Chip_Type   = W25Qxx_Not   ;	  //  Ĭ�Ͽ�

WS_W25Qxx_FlashInfoTypeDef W25QxxInfo                 ;   //  W25Qxx �����Ϣ

uint8_t  W25QxxWriteBuffer[4096]                      ;   //  ����дW25QxxоƬ�������ݻ���



/********************************************************************************
	*�������ƣ� uint8_t WS_W25Qxx_Read_Write_Byte(uint8_t data)
	*���ܣ�     SPI��дһ���ֽ�
	*������     date:д�������
	*���أ�			r_date:����������
*********************************************************************************/
uint8_t WS_W25Qxx_Read_Write_Byte(uint8_t date)
{
	    uint8_t r_date , t_date = date ;
	    HAL_SPI_TransmitReceive(&W25Qxx_hspi,&t_date,&r_date,1,1);
	    return r_date ;   
}

/********************************************************************************
	*�������ƣ� uint8_t WS_W25Qxx_Read_Byte(void)
	*���ܣ�     SPI��һ���ֽ�
	*������     ��
	*���أ�			r_date:����������
*********************************************************************************/
uint8_t WS_W25Qxx_Read_Byte(void)
{
	    uint8_t r_date  ;
	    HAL_SPI_Receive(&W25Qxx_hspi,&r_date,1,1);
	    return r_date ;   
}

/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write_Byte(uint8_t date)
	*���ܣ�     SPI��һ���ֽ�
	*������     date:д�������
	*���أ�			��
*********************************************************************************/
void WS_W25Qxx_Write_Byte(uint8_t date)
{
	    uint8_t t_date = date ;
	    HAL_SPI_Transmit(&W25Qxx_hspi,&t_date,1,1);
}


/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Init(void)
	*���ܣ�     ��ʼ��W25QXX����ȡоƬ�ͺż�������Ϣ���ã����������Ϣ
	*������     ��
	*���أ�			W25Qxx ����
*********************************************************************************/
WS_W25QxxTypeDef WS_W25Qxx_Init(void)
{
	   if(WS_W25Qxx_Chip_Type == W25Qxx_Not)
		 {
			 uint16_t id =  WS_W25Qxx_Read_ID() ;   
			 id =  WS_W25Qxx_Read_ID()          ;	
			 //  �����Ϣ�ṹ��
			 memset((char *)&W25QxxInfo,0,sizeof(WS_W25Qxx_FlashInfoTypeDef))	;
			 switch(id)
			 {
				 case W25Qxx_16 :  WS_W25Qxx_Chip_Type = W25Qxx_16 ; 
													 W25QxxInfo.chipSectorNbr = 512      ;			 
													 break;
				 
				 case W25Qxx_32 :  WS_W25Qxx_Chip_Type = W25Qxx_32 ;
													 W25QxxInfo.chipSectorNbr = 1024      ;
													 break;
				 
				 case W25Qxx_64 :  WS_W25Qxx_Chip_Type = W25Qxx_64 ;
													 W25QxxInfo.chipSectorNbr = 2048      ;
													 break;
				 
				 case W25Qxx_128 : WS_W25Qxx_Chip_Type = W25Qxx_128 ; 
													 W25QxxInfo.chipSectorNbr = 4096      ;
													 break; 
			 }
			 if(WS_W25Qxx_Chip_Type != W25Qxx_Not)
			 {
				 W25QxxInfo.chipType       = WS_W25Qxx_Chip_Type      ;
				 W25QxxInfo.chipSectorSize = WS_W25Qxx_Sector_Size    ;
				 W25QxxInfo.fontStartSector= W25QxxInfo.chipSectorNbr>>1 ; //  
				 W25QxxInfo.fatfsSectorNbr = (W25QxxInfo.chipSectorNbr>>1)* (4096/WS_W25Qxx_Fatfs_Sector_Size) ; //  �ļ�ϵͳ��һ��ռ�
				 W25QxxInfo.fatfsSectorSize= WS_W25Qxx_Fatfs_Sector_Size ;
				 WS_Debug(1,"W25Qxx id : %X\r\n",id)  ;
			 }
			 else
			 {
				 WS_Debug(1,"Not W25Qxx %X\r\n",id)   ;    
			 }
		 
		 }
		 return WS_W25Qxx_Chip_Type ;
}


/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Init(void)
	*���ܣ�     ��ʼ��W25QXX����ȡоƬ�źţ����������Ϣ
	*������     ��
	*���أ�			оƬ�ͺţ� W25Qxx_16  =	0XEF14  
												 W25Qxx_32  =	0XEF15  
												 W25Qxx_64  =	0XEF16  
												 W25Qxx_128 =	0XEF17 	
*********************************************************************************/ 
uint16_t WS_W25Qxx_Read_ID(void)
{
	uint16_t id = 0;	  
	WS_W25Qxx_CS_RESET;
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_ID);      //���Ͷ�ȡID����	    
	WS_W25Qxx_Write_Byte(0x00); 	    
	WS_W25Qxx_Write_Byte(0x00); 	    
	WS_W25Qxx_Write_Byte(0x00); 	 			   
	id|=WS_W25Qxx_Read_Byte()<<8;  
	id|=WS_W25Qxx_Read_Byte();	 
	WS_W25Qxx_CS_SET;				    
	return id;
} 													 


/********************************************************************************
	*�������ƣ� uint8_t WS_W25Qxx_Read_SR(void) 
	*���ܣ�     ��ȡW25Qxx��״̬�Ĵ���
	*������     ��
	*���أ�			״̬�Ĵ���ֵ
	�Ĵ�������
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
	TB,BP2,BP1,BP0:FLASH����д��������
	WEL:дʹ������
	BUSY:æ���λ(1,æ;0,����)
	Ĭ��:0x00
*********************************************************************************/ 
uint8_t WS_W25Qxx_Read_SR(void)   
{  
	uint8_t byte=0;   
	WS_W25Qxx_CS_RESET;                            //ʹ������   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Read);      //���Ͷ�ȡ״̬�Ĵ�������    
	byte=WS_W25Qxx_Read_Byte();                    //��ȡһ���ֽ�  
	WS_W25Qxx_CS_SET;                              //ȡ��Ƭѡ     
	return byte;   
} 

/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write_SR(uint8_t date) 
	*���ܣ�     дW25QXX״̬�Ĵ���
	*������     ��
	*���أ�			��
  ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
*********************************************************************************/ 
void WS_W25Qxx_Write_SR(uint8_t date)   
{   
	WS_W25Qxx_CS_RESET;                                //ʹ������   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Write);    //����дȡ״̬�Ĵ�������    
	WS_W25Qxx_Write_Byte(date);               	   //д��һ���ֽ�  
	WS_W25Qxx_CS_SET;                                  //ȡ��Ƭѡ     	      
} 


/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write_Enable(void)  
	*���ܣ�     W25QxxоƬдʹ��	�� ��������ǰ��Ҫдʹ��
	*������     ��
	*���أ�		  ��
*********************************************************************************/ 
 void WS_W25Qxx_Write_Enable(void)   
{
	WS_W25Qxx_CS_RESET;                          
  WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_WriteEnable); 	
	WS_W25Qxx_CS_SET;                           	    
} 


/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write_Disable(void)    
	*���ܣ�     W25QxxоƬдʧ��	�� ������������Ҫдʧ��
	*������     ��
	*���أ�		  ��
*********************************************************************************/ 
void WS_W25Qxx_Write_Disable(void)   
{  
	WS_W25Qxx_CS_RESET;                           
  WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_WriteDisable);  
	WS_W25Qxx_CS_SET;                                 
} 		
 
/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint16_t len)     
	*���ܣ�     ����ָ����ַ�����ֽ�Ϊ��λ����ȡоƬ���ݣ�
  *������     buffer:���ݻ���
              addr  :��ȡ��ַ��
              len   :��ȡ���ݳ���
	*���أ�		  ��
*********************************************************************************/ 
void WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint32_t len)   
{								    
		WS_W25Qxx_CS_RESET;                            
    WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_ReadData);         	  //���Ͷ�ȡ����   
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16));  	//����24bit��ַ    
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8)) ;   
    WS_W25Qxx_Write_Byte((uint8_t) addr);   
    HAL_SPI_Receive(&W25Qxx_hspi,buffer,len,100);   	//ѭ������  
	  WS_W25Qxx_CS_SET;  				    	      
} 

/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)    
	*���ܣ�     ����ָ����ַ�����ֽ�Ϊ��λ��д���ݵ�оƬ�����256���� ÿ��������Ҫ�ֶ��д��
  *������     buffer:���ݻ���
              addr  :д��ַ��
              len   :д���ݳ���
	*���أ�		  ��
*********************************************************************************/ 
void WS_W25Qxx_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)
{
 
  WS_W25Qxx_Write_Enable();                  	        
	WS_W25Qxx_CS_RESET  ;                           
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_PageProgram);      	//����дҳ����   
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16)); 	//����24bit��ַ    
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8)) ;   
	WS_W25Qxx_Write_Byte((uint8_t)  addr);   
	HAL_SPI_Transmit(&W25Qxx_hspi, buffer,len, 100);//ѭ��д��
	WS_W25Qxx_CS_SET  ;                            	        
	WS_W25Qxx_Wait_Idle();					   		             //�ȴ�д�����
} 



/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint16_t len)     
	*���ܣ�     ����ָ����ַ�����ֽ�Ϊ��λ��д���ݵ�оƬ��
  *������     buffer:���ݻ���
              addr  :д���ַ��
              len   :д�����ݳ���
	*���أ�		  ��
*********************************************************************************/ 
WS_StatusTypeDef WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint32_t len)   
{								    
	uint32_t size        ; 
  uint32_t  chipSector , addrAdd , writeCount ,writeAddr ;
	uint16_t i   = 0  ,  l    ;

	//  ���������������
	if(len == 0 || buffer == NULL || addr > W25QxxInfo.chipSectorNbr*W25QxxInfo.chipSectorSize)   return  WS_Status_ERROR;
	
	//   ����д������
	while(len  >  0)
	{
		
		  l =   addr % WS_W25Qxx_Sector_Size ;
		  //  �����Ҫ����д����ֽڣ���������������д��
		  if(((WS_W25Qxx_Sector_Size) - l) <  len)
			{
			   writeCount = ((WS_W25Qxx_Sector_Size) - l);
				 len       -= ((WS_W25Qxx_Sector_Size) - l); 
			}
			else
			{
			   writeCount       = len;
				 len             -= len;
			}
			
			//   ����д���ַ���ڵ������������ַƫ��
			chipSector =addr / 4096; // оƬ������ַ  
			addrAdd    =addr % 4096; // �������ڵ�ƫ��
			
			//  ����д���ַ
			writeAddr = chipSector  *   4096 ; 
		

			 //   ��������������
			 WS_W25Qxx_Read   (W25QxxWriteBuffer, chipSector * (WS_W25Qxx_Sector_Size) 
												,WS_W25Qxx_Sector_Size) ;
			 //WS_W25Qxx_Read(W25QxxWriteBuffer,chipSector * 4096 , 4096);
			 
			 //   ����д������ݿ�����ָ��λ��
			 memcpy(W25QxxWriteBuffer + addrAdd , buffer , writeCount);
			 
			 
			 
			 //  ������Ӧ����
			 WS_W25Qxx_Erase_Sector(chipSector);  
			 
			 //  ��16ҳд��
			 for(i=0; i <  16 ; i++)
				{
					 size  = (i * 256)   ;                                     
					 WS_W25Qxx_Fatfs_Write_Page(W25QxxWriteBuffer+size,writeAddr+size,256) ;
				}
				

			WS_W25Qxx_Read(W25QxxWriteBuffer,addr,writeCount)       ;
				
			if(memcmp(W25QxxWriteBuffer ,buffer,writeCount) != 0)
			{
				WS_Debug(0,"W25Qxx write error:%d~%d\r\n",writeAddr,writeAddr+writeCount);	
			}
			else
			{
				//WS_Debug(0,"W25Qxx write OK Sector:%d~%d\r\n",writeAddr,writeAddr+writeCount);
			}
			
			addr   += writeCount;   //  ��ӵ�ַ���ˣ���ɲ���д��
			buffer += writeCount;
			
	}
  return WS_Status_OK;	     		    	      
}



/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Erase_Sector(uint32_t sector)      
	*���ܣ�     ����ָ����������һ������( 4096 byte),����ʱ��:40ms����
  *������     sector:������       
	*���أ�		  ��
*********************************************************************************/ 
void WS_W25Qxx_Erase_Sector(uint32_t sector)   
{  
 	  uint32_t addr  =  WS_W25Qxx_Sector_Size * sector;
    WS_W25Qxx_Write_Enable();                 
    WS_W25Qxx_Wait_Idle()  ;   
  	WS_W25Qxx_CS_RESET  ;                            
    WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Erase_Sector);      	
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16));  	
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8));   
    WS_W25Qxx_Write_Byte((uint8_t)  addr);  
	  WS_W25Qxx_CS_SET  ;                      	      
    WS_W25Qxx_Wait_Idle();   				   		   //�ȴ��������
} 

/********************************************************************************
	*�������ƣ� void WS_W25Qxx_Wait_Busy(void)      
	*���ܣ�     �ȴ�оƬ��ɲ���, ��Ҫ�����߳��ڶ�оƬ��ɲ���
  *������     ��       
	*���أ�		  ��
*********************************************************************************/ 
void WS_W25Qxx_Wait_Idle(void)   
{  
  int i = 0 ; 	 
	while((WS_W25Qxx_Read_SR()&0x01)==0x01)  // �ȴ�BUSYλ���
	{
		 //   �ڲ���ϵͳ�£� �����ͷ�CPU�� ����ִ�б������
	   WS_Sys_Delay_MS(1)      ;
		 if(++ i >= 10000)  break; 
	}  		            
} 


//   ʵ�����ҳʱ�� 36ms , д��ʱ��26ms
WS_StatusTypeDef  WS_W25Qxx_Sector_Write(uint8_t *buffer  ,uint32_t sector ,  uint32_t size)   
{
    WS_W25Qxx_Erase_Sector(sector) ;
	  WS_W25Qxx_Write(buffer, sector * WS_W25Qxx_Sector_Size , size) ; 
	  return WS_Status_OK;
}


//д��ϵͳ������Ϣ�麯��
void  WS_Config_Write_Struct_Callback(uint8_t *  date  ,  uint32_t  len )
{
      uint32_t   configSector = 0;
      if(WS_W25Qxx_Chip_Type != W25Qxx_Not) 
			{
				 //  ������Ϣ���������һ������
				 configSector = W25QxxInfo.chipSectorNbr-1;
				 WS_W25Qxx_Write(date,configSector*WS_W25Qxx_Sector_Size,len);   
			} 				
}


//      ��ȡϵͳ������Ϣ�麯��
void  WS_Config_Read_Struct_Callback(uint8_t *  date  ,  uint32_t len)   
{
	    uint32_t   configSector = 0;
      if(WS_W25Qxx_Chip_Type != W25Qxx_Not) 
			{
			   //  ������Ϣ���������һ������
				  configSector = W25QxxInfo.chipSectorNbr-1; 
				  //WS_Debug(1,"configSector:%d\r\n",configSector);
			    WS_W25Qxx_Read(date , configSector * WS_W25Qxx_Sector_Size , len);
			}		
}


//    �ڴ濨�����ļ�����+�洢λ�÷���
const  WS_ConfigFileTypeDef   loadConfigFileInfo[] =
{
   "H_16.dzk",WS_W25Qxx_FONT16_Start_Sector,70,
	 "H_24.dzk",WS_W25Qxx_FONT24_Start_Sector,266,
	 "H_32.dzk",WS_W25Qxx_FONT32_Start_Sector,544
};



//   ����Կ��ļ��Ƿ����
//   �ֿ��ļ���Ÿ�ʽ��4byte(�ļ�����) + n byte(�ļ�) + 1 byte(1~n �������׸��ֽں�У��)
WS_StatusTypeDef WS_W25Qxx_Check_File(WS_ConfigFileTypeDef W25QxxFileInfo)
{
	  uint32_t size ;
	  uint32_t fontStartSector ;
	  uint32_t Sector , i;
	  uint8_t  check = 0 ,dat ;
	                    //  �ֿ������� + ƫ������
	  fontStartSector =  W25QxxFileInfo.startSector + WS_W25Qxx_FONT16_Base_Sector ;
    WS_W25Qxx_Read((uint8_t *)&size , WS_W25Qxx_Sector_Size*fontStartSector , 4)  ;
	
	  //  �����ļ����ȳ���100M�� ����0 �� ���ش���
	  if(size > 100 * 1024*1024 || size == 0)  
		{	
			return WS_Status_ERROR;
		}

		//   ��ȥ�ļ����ȵ����ĸ��ֽڣ�����ļ����ȣ�size-4���պ���4096������
    Sector = (size-4-1) /  WS_W25Qxx_Sector_Size + 1;	
	  for(i=fontStartSector+1;i<Sector+fontStartSector;i++)
	  {
	    WS_W25Qxx_Read(&dat,WS_W25Qxx_Sector_Size*i,1);  
		  check += dat;
	  }
		WS_W25Qxx_Read(&dat,WS_W25Qxx_Sector_Size*fontStartSector + size+4  ,1);
		if(dat == check )  return WS_Status_OK;
	  else               return WS_Status_ERROR;
}

//    д���ļ�
WS_StatusTypeDef WS_W25Qxx_Font_Write(WS_ConfigFileTypeDef W25QxxFileInfo)
{
      FIL file;
			FRESULT 	res ; 
	    uint32_t fontStartSector ;
			UINT br ;
			uint8_t buffer[4097]     ;
	    char    filePath[24]     ;
		  uint32_t Sector = 0       ;
      uint8_t  check  = 0      ;        //  ���У��	
	    uint8_t  n      = 0      ;        //  ������
	                                                  // ���ֿ�洢ƫ����
      fontStartSector =  W25QxxFileInfo.startSector + W25QxxInfo.fontStartSector;	
			
      sprintf(filePath,"0:/%s",W25QxxFileInfo.name);  
	
			res = f_open(&file,filePath,FA_READ); 
			if(res!=FR_OK)
			{
				WS_Debug(2,"TF Card %s NOT OPEN\r\n",W25QxxFileInfo.name);
				return WS_Status_ERROR         ;
			}else{
				WS_Debug(2,"%s Open size: %d\r\n",W25QxxFileInfo.name,file.fsize); 
				WS_Debug(3,"Start with Sector %d write %s\r\n",W25QxxFileInfo.startSector,W25QxxFileInfo.name);
			}
			
      Sector = fontStartSector  ;	                  // ����16�������Sector 0 ��ʼ�洢
			*(uint32_t *)buffer = file.fsize  ;
      res = f_read(&file, buffer+4, 4092, &br);
      br  += 4; 			
			while(res == FR_OK)
			{
				 if(br > 0 )
				 {
					  // �ӵڶ�������ʼ��һ���ֽڽ��к�У��
					  if(Sector >  fontStartSector)
						{
						    check += buffer[0] ;
  					}

						if(br == 4096)
						{
				      //WS_W25Qxx_Sector_Write(Sector++ ,buffer ,br) ;
							WS_W25Qxx_Write(buffer,Sector++*WS_W25Qxx_Sector_Size , br) ;
						}
						else
						{
						  buffer[ br ++]  =  check ;
              //WS_W25Qxx_Sector_Write(Sector++ ,buffer ,br) ;
							WS_W25Qxx_Write(buffer,Sector++*WS_W25Qxx_Sector_Size , br) ;
						}
						
					  if(++n % 20 != 0)
					     WS_Debug(0,".")    ; 
						else
							 WS_Debug(0,"\r\n."); 
				 }
				 else
				 {
					  //  �洢���ĸ��ֽ�Ϊ�ļ�����
					  if((file.fsize-4) % 4096 == 0)
						{
						   buffer[ br ++]  =  check ;
               //WS_W25Qxx_Sector_Write(Sector++ ,buffer ,br) ;
							 WS_W25Qxx_Write(buffer,Sector++*WS_W25Qxx_Sector_Size , br) ;
						}			
						if(WS_W25Qxx_Check_File(W25QxxFileInfo) == WS_Status_OK)
						{
					    WS_Sys_Delay_MS(1);
					    WS_Debug(3,"\r\nwrite %s finish\r\n",W25QxxFileInfo.name , file.fsize);
						}
						return WS_Status_OK;
					}
				  res = f_read(&file, buffer, 4096, &br);
				}
		return WS_Status_ERROR ;
}



//   ���ڴ濨�����ֿ���Ϣ��W25Qxx
void WS_W25Qxx_Font_Chenk_Write(void)
{
	  uint16_t  i ;	
//  ��ӡ�ڴ濨��Ϣ	
#if BSP_SD_CardInfo !=  0
    HAL_SD_CardInfoTypeDef cardInfo              ;
		HAL_SD_GetCardInfo(&hsd, &cardInfo)          ;
		float fCardSize = 1.0*cardInfo.BlockNbr*cardInfo.BlockSize/1024/1024;
		WS_Debug(2,"Card Size:%.2fM file count:%d\r\n", fCardSize,sizeof(loadConfigFileInfo)/sizeof(WS_ConfigFileTypeDef));
#endif	
	
    for(i=0;i<sizeof(loadConfigFileInfo)/sizeof(WS_ConfigFileTypeDef);i++)
		  {
			   if(WS_W25Qxx_Check_File(loadConfigFileInfo[i]) == WS_Status_OK)
		     {
					  WS_Debug(1,"%s exist\r\n",loadConfigFileInfo[i].name);
					  if(i == 0)  W25QxxInfo.font16Flag = 1;
					  if(i == 1)  W25QxxInfo.font24Flag = 1;
					  if(i == 2)  W25QxxInfo.font32Flag = 1;
				 }
				 else
				 {	     
						#if (FS_FAT12 != 0 || FS_FAT16 != 0  || FS_FAT32 != 0 )  
								FRESULT   retSD;
								retSD = f_mount(&SDFatFS, SDPath, 1);
								if(retSD)
								{
										WS_Debug(1,"TF card mount error\r\n",retSD);	
								}
								else
								{
										WS_W25Qxx_Font_Write(loadConfigFileInfo[i]); 		
								}	  
						#endif
				 }
				 
			}  	
}

//   ���ֿ���Ϣ 16* 16
//   ���ݻ����ַ
//   string: ��������
WS_StatusTypeDef WS_W25Qxx_Font16_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font16Flag != 0)
		{
			 //WS_Debug(0,"MSB:%d,LSB:%d\r\n",string[0],string[1]);
			         //  �ֿ������� + ƫ������
			 addr  = (WS_W25Qxx_FONT16_Start_Sector+WS_W25Qxx_FONT16_Base_Sector) * WS_W25Qxx_Sector_Size + 4;
			 addr += ((string[0]-0xa1)  * 94  +  (string[1]- 0xa1) )*32 ;
		   WS_W25Qxx_Read(buffer, addr , 32 );
		   return WS_Status_OK ;
		}
    else
    {
		   return WS_Status_EMPTY ;
		}			
}	


//   ���ֿ���Ϣ 24* 24
//   ���ݻ����ַ
//   string: ��������
WS_StatusTypeDef WS_W25Qxx_Font24_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font24Flag != 0)
		{          //  �ֿ������� + ƫ������
			 addr  = (WS_W25Qxx_FONT24_Start_Sector+WS_W25Qxx_FONT16_Base_Sector) * WS_W25Qxx_Sector_Size + 4;
			 addr += ((string[0]-0xa1)  * 94  +  (string[1]- 0xa1) )*72 ;
		   WS_W25Qxx_Read(buffer, addr , 72 );
		   return WS_Status_OK ;
		}
    else
    {
		   return WS_Status_EMPTY ;
		}			
}

//   ���ֿ���Ϣ 32* 32
//   ���ݻ����ַ
//   string: ��������
WS_StatusTypeDef WS_W25Qxx_Font32_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font32Flag != 0)
		{          //  �ֿ������� + ƫ������
			 addr  = (WS_W25Qxx_FONT32_Start_Sector+WS_W25Qxx_FONT16_Base_Sector) * WS_W25Qxx_Sector_Size + 4;
			 addr += ((string[0]-0xa1)  * 94  +  (string[1]- 0xa1) )*128 ;
		   WS_W25Qxx_Read(buffer, addr , 128 );
		   return WS_Status_OK ;
		}
    else
    {
		   return WS_Status_EMPTY ;
		}			
}





//�����ļ�ϵͳ��д�ȴ�����,�ɵ�
void WS_W25Qxx_Fatfs_Wait_Idle(void)   
{  	 
	while((WS_W25Qxx_Read_SR()&0x01) == 0x01) ; // �ȴ�BUSYλ���  		            
}

//    �����ļ�ϵͳ�Ķ�W25Qxx���ݺ���
WS_StatusTypeDef  WS_W25Qxx_Fatfs_Read_Sector(uint8_t* buffer,uint32_t startFatfsSector,uint32_t fatfsSectorCount)   
{ 	  	
	  WS_W25Qxx_Read(buffer,startFatfsSector * WS_W25Qxx_Fatfs_Sector_Size , fatfsSectorCount * WS_W25Qxx_Fatfs_Sector_Size) ;
    return WS_Status_OK ;	
}


void WS_W25Qxx_Fatfs_Erase_Sector(uint32_t sector)   
{  
 	  uint32_t  addr  =  WS_W25Qxx_Sector_Size  *  sector;
	   
    WS_W25Qxx_Write_Enable();              
    WS_W25Qxx_Fatfs_Wait_Idle();   
  	WS_W25Qxx_CS_RESET  ;                            
    WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Erase_Sector);      	
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16))     ;  	
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8))      ;   
    WS_W25Qxx_Write_Byte((uint8_t)  addr);   
	  WS_W25Qxx_CS_SET  ;                      	      
    WS_W25Qxx_Fatfs_Wait_Idle();   				   		   //�ȴ��������
} 

//    �����ļ�ϵͳ��дW25Qxxҳ�����256�����ݺ���
WS_StatusTypeDef  WS_W25Qxx_Fatfs_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)       
{
  
  WS_W25Qxx_Write_Enable();                   	         
	WS_W25Qxx_CS_RESET  ;                            	  //ʹ������   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_PageProgram);      	      //����дҳ����   
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16)); 	//����24bit��ַ    
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8)) ;   
	WS_W25Qxx_Write_Byte((uint8_t)addr);   
	HAL_SPI_Transmit(&W25Qxx_hspi, buffer,len , 100);
	WS_W25Qxx_CS_SET  ;                            	          
	WS_W25Qxx_Fatfs_Wait_Idle();	
  return WS_Status_OK;	
} 


WS_StatusTypeDef WS_W25Qxx_Fatfs_Write_Sector(uint8_t* buffer,uint32_t startFatfsSector,uint32_t sectorCount)   
{ 			 		 
  uint32_t addr = 0    ;
	uint32_t size        ; 
  uint32_t  chipSector , addrAdd , fatfsSectorCount ;
	uint16_t i   = 0  ,  l    ;

	//   ����д������
	while(sectorCount >  0)
	{
		
		  l =   startFatfsSector % WS_W25Qxx_Fatfs_Page_Nbr ;
		  //  �����Ҫд�����������
		  if(((WS_W25Qxx_Fatfs_Page_Nbr) - l) <  sectorCount)
			{
			   fatfsSectorCount = ((WS_W25Qxx_Fatfs_Page_Nbr) - l);
				 sectorCount     -= ((WS_W25Qxx_Fatfs_Page_Nbr) - l); 
			}
			else
			{
			   fatfsSectorCount = sectorCount;
				 sectorCount     -= sectorCount;
			}
	
			addr   = startFatfsSector * WS_W25Qxx_Fatfs_Sector_Size   ;
			size   = fatfsSectorCount * WS_W25Qxx_Fatfs_Sector_Size   ;
			chipSector =addr / 4096; // оƬ������ַ  
			addrAdd    =addr % 4096; // �������ڵ�ƫ��
		

			 //   ��������������
			 WS_W25Qxx_Fatfs_Read_Sector (W25QxxWriteBuffer, chipSector * (WS_W25Qxx_Fatfs_Page_Nbr) 
																	 ,WS_W25Qxx_Fatfs_Page_Nbr) ;
			 //WS_W25Qxx_Read(W25QxxWriteBuffer,chipSector * 4096 , 4096);
			 
			 //   ����д������ݿ�����ָ��λ��
			 memcpy(W25QxxWriteBuffer + addrAdd , buffer , size);
			 //  ����д���ַ
			 uint32_t writeAddr = chipSector  *   4096 ;
			 
			 //  ������Ӧ����
			 WS_W25Qxx_Fatfs_Erase_Sector(chipSector);  
			 
			 //  ��16ҳд��
			 for(i=0; i <  16 ; i++)
				{
					 size  = (i * 256)   ;                                     
					 WS_W25Qxx_Fatfs_Write_Page(W25QxxWriteBuffer+size,writeAddr+size,256) ;
				}
				

			WS_W25Qxx_Fatfs_Read_Sector(W25QxxWriteBuffer,startFatfsSector,fatfsSectorCount)       ;
			if(memcmp(W25QxxWriteBuffer ,buffer,fatfsSectorCount * WS_W25Qxx_Fatfs_Sector_Size) != 0)
			{
				WS_Debug(0,"Fatfs write error:%d~%d\r\n",startFatfsSector,startFatfsSector+fatfsSectorCount);	
			}
			else
			{
				//WS_Debug(0,"Fatfs write OK Sector:%d~%d\r\n",startFatfsSector,startFatfsSector+fatfsSectorCount) ;
			}
			
			startFatfsSector += fatfsSectorCount;
			buffer           += fatfsSectorCount*WS_W25Qxx_Fatfs_Sector_Size;
			
	}
  return WS_Status_OK;	  
} 
















