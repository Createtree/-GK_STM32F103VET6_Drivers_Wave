
#include "ws_w25qxx.h" 

extern           SD_HandleTypeDef hsd                 ;   //  SD卡控制结构

extern           SPI_HandleTypeDef W25Qxx_hspi        ;   //  芯片的使用的SPI编号

WS_W25QxxTypeDef WS_W25Qxx_Chip_Type   = W25Qxx_Not   ;	  //  默认空

WS_W25Qxx_FlashInfoTypeDef W25QxxInfo                 ;   //  W25Qxx 相关信息

uint8_t  W25QxxWriteBuffer[4096]                      ;   //  用于写W25Qxx芯片扇区数据缓存



/********************************************************************************
	*方法名称： uint8_t WS_W25Qxx_Read_Write_Byte(uint8_t data)
	*功能：     SPI读写一个字节
	*参数：     date:写入的数据
	*返回：			r_date:读到的数据
*********************************************************************************/
uint8_t WS_W25Qxx_Read_Write_Byte(uint8_t date)
{
	    uint8_t r_date , t_date = date ;
	    HAL_SPI_TransmitReceive(&W25Qxx_hspi,&t_date,&r_date,1,1);
	    return r_date ;   
}

/********************************************************************************
	*方法名称： uint8_t WS_W25Qxx_Read_Byte(void)
	*功能：     SPI读一个字节
	*参数：     无
	*返回：			r_date:读到的数据
*********************************************************************************/
uint8_t WS_W25Qxx_Read_Byte(void)
{
	    uint8_t r_date  ;
	    HAL_SPI_Receive(&W25Qxx_hspi,&r_date,1,1);
	    return r_date ;   
}

/********************************************************************************
	*方法名称： void WS_W25Qxx_Write_Byte(uint8_t date)
	*功能：     SPI读一个字节
	*参数：     date:写入的数据
	*返回：			无
*********************************************************************************/
void WS_W25Qxx_Write_Byte(uint8_t date)
{
	    uint8_t t_date = date ;
	    HAL_SPI_Transmit(&W25Qxx_hspi,&t_date,1,1);
}


/********************************************************************************
	*方法名称： void WS_W25Qxx_Init(void)
	*功能：     初始化W25QXX，获取芯片型号及配置信息设置，输出调试信息
	*参数：     无
	*返回：			W25Qxx 类型
*********************************************************************************/
WS_W25QxxTypeDef WS_W25Qxx_Init(void)
{
	   if(WS_W25Qxx_Chip_Type == W25Qxx_Not)
		 {
			 uint16_t id =  WS_W25Qxx_Read_ID() ;   
			 id =  WS_W25Qxx_Read_ID()          ;	
			 //  清除信息结构体
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
				 W25QxxInfo.fatfsSectorNbr = (W25QxxInfo.chipSectorNbr>>1)* (4096/WS_W25Qxx_Fatfs_Sector_Size) ; //  文件系统用一半空间
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
	*方法名称： void WS_W25Qxx_Init(void)
	*功能：     初始化W25QXX，获取芯片信号，输出调试信息
	*参数：     无
	*返回：			芯片型号： W25Qxx_16  =	0XEF14  
												 W25Qxx_32  =	0XEF15  
												 W25Qxx_64  =	0XEF16  
												 W25Qxx_128 =	0XEF17 	
*********************************************************************************/ 
uint16_t WS_W25Qxx_Read_ID(void)
{
	uint16_t id = 0;	  
	WS_W25Qxx_CS_RESET;
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_ID);      //发送读取ID命令	    
	WS_W25Qxx_Write_Byte(0x00); 	    
	WS_W25Qxx_Write_Byte(0x00); 	    
	WS_W25Qxx_Write_Byte(0x00); 	 			   
	id|=WS_W25Qxx_Read_Byte()<<8;  
	id|=WS_W25Qxx_Read_Byte();	 
	WS_W25Qxx_CS_SET;				    
	return id;
} 													 


/********************************************************************************
	*方法名称： uint8_t WS_W25Qxx_Read_SR(void) 
	*功能：     读取W25Qxx的状态寄存器
	*参数：     无
	*返回：			状态寄存器值
	寄存器定义
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
	SPR:默认0,状态寄存器保护位,配合WP使用
	TB,BP2,BP1,BP0:FLASH区域写保护设置
	WEL:写使能锁定
	BUSY:忙标记位(1,忙;0,空闲)
	默认:0x00
*********************************************************************************/ 
uint8_t WS_W25Qxx_Read_SR(void)   
{  
	uint8_t byte=0;   
	WS_W25Qxx_CS_RESET;                            //使能器件   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Read);      //发送读取状态寄存器命令    
	byte=WS_W25Qxx_Read_Byte();                    //读取一个字节  
	WS_W25Qxx_CS_SET;                              //取消片选     
	return byte;   
} 

/********************************************************************************
	*方法名称： void WS_W25Qxx_Write_SR(uint8_t date) 
	*功能：     写W25QXX状态寄存器
	*参数：     无
	*返回：			无
  只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
*********************************************************************************/ 
void WS_W25Qxx_Write_SR(uint8_t date)   
{   
	WS_W25Qxx_CS_RESET;                                //使能器件   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_Write);    //发送写取状态寄存器命令    
	WS_W25Qxx_Write_Byte(date);               	   //写入一个字节  
	WS_W25Qxx_CS_SET;                                  //取消片选     	      
} 


/********************************************************************************
	*方法名称： void WS_W25Qxx_Write_Enable(void)  
	*功能：     W25Qxx芯片写使能	， 编程与擦除前需要写使能
	*参数：     无
	*返回：		  无
*********************************************************************************/ 
 void WS_W25Qxx_Write_Enable(void)   
{
	WS_W25Qxx_CS_RESET;                          
  WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_WriteEnable); 	
	WS_W25Qxx_CS_SET;                           	    
} 


/********************************************************************************
	*方法名称： void WS_W25Qxx_Write_Disable(void)    
	*功能：     W25Qxx芯片写失能	， 编程与擦除后需要写失能
	*参数：     无
	*返回：		  无
*********************************************************************************/ 
void WS_W25Qxx_Write_Disable(void)   
{  
	WS_W25Qxx_CS_RESET;                           
  WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_WriteDisable);  
	WS_W25Qxx_CS_SET;                                 
} 		
 
/********************************************************************************
	*方法名称： void WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint16_t len)     
	*功能：     按照指定地址（以字节为单位）读取芯片数据，
  *参数：     buffer:数据缓存
              addr  :读取地址；
              len   :读取数据长度
	*返回：		  无
*********************************************************************************/ 
void WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint32_t len)   
{								    
		WS_W25Qxx_CS_RESET;                            
    WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_ReadData);         	  //发送读取命令   
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16));  	//发送24bit地址    
    WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8)) ;   
    WS_W25Qxx_Write_Byte((uint8_t) addr);   
    HAL_SPI_Receive(&W25Qxx_hspi,buffer,len,100);   	//循环读数  
	  WS_W25Qxx_CS_SET;  				    	      
} 

/********************************************************************************
	*方法名称： void WS_W25Qxx_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)    
	*功能：     按照指定地址（以字节为单位）写数据到芯片，最大256个， 每个扇区需要分多次写入
  *参数：     buffer:数据缓存
              addr  :写地址；
              len   :写数据长度
	*返回：		  无
*********************************************************************************/ 
void WS_W25Qxx_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)
{
 
  WS_W25Qxx_Write_Enable();                  	        
	WS_W25Qxx_CS_RESET  ;                           
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_PageProgram);      	//发送写页命令   
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16)); 	//发送24bit地址    
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>8)) ;   
	WS_W25Qxx_Write_Byte((uint8_t)  addr);   
	HAL_SPI_Transmit(&W25Qxx_hspi, buffer,len, 100);//循环写数
	WS_W25Qxx_CS_SET  ;                            	        
	WS_W25Qxx_Wait_Idle();					   		             //等待写入结束
} 



/********************************************************************************
	*方法名称： void WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint16_t len)     
	*功能：     按照指定地址（以字节为单位）写数据到芯片；
  *参数：     buffer:数据缓存
              addr  :写入地址；
              len   :写入数据长度
	*返回：		  无
*********************************************************************************/ 
WS_StatusTypeDef WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint32_t len)   
{								    
	uint32_t size        ; 
  uint32_t  chipSector , addrAdd , writeCount ,writeAddr ;
	uint16_t i   = 0  ,  l    ;

	//  初步严重输入参数
	if(len == 0 || buffer == NULL || addr > W25QxxInfo.chipSectorNbr*W25QxxInfo.chipSectorSize)   return  WS_Status_ERROR;
	
	//   连续写入数据
	while(len  >  0)
	{
		
		  l =   addr % WS_W25Qxx_Sector_Size ;
		  //  拆分需要连续写入的字节，分物理扇区依次写入
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
			
			//   计算写入地址所在的物理扇区与地址偏移
			chipSector =addr / 4096; // 芯片扇区地址  
			addrAdd    =addr % 4096; // 在扇区内的偏移
			
			//  计算写入地址
			writeAddr = chipSector  *   4096 ; 
		

			 //   读扇区所有数据
			 WS_W25Qxx_Read   (W25QxxWriteBuffer, chipSector * (WS_W25Qxx_Sector_Size) 
												,WS_W25Qxx_Sector_Size) ;
			 //WS_W25Qxx_Read(W25QxxWriteBuffer,chipSector * 4096 , 4096);
			 
			 //   将带写入的数据拷贝到指定位置
			 memcpy(W25QxxWriteBuffer + addrAdd , buffer , writeCount);
			 
			 
			 
			 //  擦除对应扇区
			 WS_W25Qxx_Erase_Sector(chipSector);  
			 
			 //  分16页写入
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
			
			addr   += writeCount;   //  添加地址便宜，完成部分写入
			buffer += writeCount;
			
	}
  return WS_Status_OK;	     		    	      
}



/********************************************************************************
	*方法名称： void WS_W25Qxx_Erase_Sector(uint32_t sector)      
	*功能：     按照指定扇区擦除一个扇区( 4096 byte),擦除时间:40ms左右
  *参数：     sector:扇区号       
	*返回：		  无
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
    WS_W25Qxx_Wait_Idle();   				   		   //等待擦除完成
} 

/********************************************************************************
	*方法名称： void WS_W25Qxx_Wait_Busy(void)      
	*功能：     等待芯片完成操作, 主要用于线程内对芯片完成操作
  *参数：     无       
	*返回：		  无
*********************************************************************************/ 
void WS_W25Qxx_Wait_Idle(void)   
{  
  int i = 0 ; 	 
	while((WS_W25Qxx_Read_SR()&0x01)==0x01)  // 等待BUSY位清空
	{
		 //   在操作系统下， 短暂释放CPU， 可以执行别的任务
	   WS_Sys_Delay_MS(1)      ;
		 if(++ i >= 10000)  break; 
	}  		            
} 


//   实测擦除页时间 36ms , 写入时间26ms
WS_StatusTypeDef  WS_W25Qxx_Sector_Write(uint8_t *buffer  ,uint32_t sector ,  uint32_t size)   
{
    WS_W25Qxx_Erase_Sector(sector) ;
	  WS_W25Qxx_Write(buffer, sector * WS_W25Qxx_Sector_Size , size) ; 
	  return WS_Status_OK;
}


//写入系统配置信息虚函数
void  WS_Config_Write_Struct_Callback(uint8_t *  date  ,  uint32_t  len )
{
      uint32_t   configSector = 0;
      if(WS_W25Qxx_Chip_Type != W25Qxx_Not) 
			{
				 //  配置信息保存于最后一个扇区
				 configSector = W25QxxInfo.chipSectorNbr-1;
				 WS_W25Qxx_Write(date,configSector*WS_W25Qxx_Sector_Size,len);   
			} 				
}


//      读取系统配置信息虚函数
void  WS_Config_Read_Struct_Callback(uint8_t *  date  ,  uint32_t len)   
{
	    uint32_t   configSector = 0;
      if(WS_W25Qxx_Chip_Type != W25Qxx_Not) 
			{
			   //  配置信息保存于最后一个扇区
				  configSector = W25QxxInfo.chipSectorNbr-1; 
				  //WS_Debug(1,"configSector:%d\r\n",configSector);
			    WS_W25Qxx_Read(date , configSector * WS_W25Qxx_Sector_Size , len);
			}		
}


//    内存卡配置文件名称+存储位置分配
const  WS_ConfigFileTypeDef   loadConfigFileInfo[] =
{
   "H_16.dzk",WS_W25Qxx_FONT16_Start_Sector,70,
	 "H_24.dzk",WS_W25Qxx_FONT24_Start_Sector,266,
	 "H_32.dzk",WS_W25Qxx_FONT32_Start_Sector,544
};



//   检测自可文件是否存在
//   字库文件存放格式：4byte(文件长度) + n byte(文件) + 1 byte(1~n 扇区的首个字节和校验)
WS_StatusTypeDef WS_W25Qxx_Check_File(WS_ConfigFileTypeDef W25QxxFileInfo)
{
	  uint32_t size ;
	  uint32_t fontStartSector ;
	  uint32_t Sector , i;
	  uint8_t  check = 0 ,dat ;
	                    //  字库首扇区 + 偏移扇区
	  fontStartSector =  W25QxxFileInfo.startSector + WS_W25Qxx_FONT16_Base_Sector ;
    WS_W25Qxx_Read((uint8_t *)&size , WS_W25Qxx_Sector_Size*fontStartSector , 4)  ;
	
	  //  读到文件长度超过100M， 或者0 ， 返回错误
	  if(size > 100 * 1024*1024 || size == 0)  
		{	
			return WS_Status_ERROR;
		}

		//   减去文件长度的首四个字节，如果文件长度（size-4）刚好是4096整数倍
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

//    写入文件
WS_StatusTypeDef WS_W25Qxx_Font_Write(WS_ConfigFileTypeDef W25QxxFileInfo)
{
      FIL file;
			FRESULT 	res ; 
	    uint32_t fontStartSector ;
			UINT br ;
			uint8_t buffer[4097]     ;
	    char    filePath[24]     ;
		  uint32_t Sector = 0       ;
      uint8_t  check  = 0      ;        //  求和校验	
	    uint8_t  n      = 0      ;        //  打点计数
	                                                  // 加字库存储偏移量
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
			
      Sector = fontStartSector  ;	                  // 竖排16号字体从Sector 0 开始存储
			*(uint32_t *)buffer = file.fsize  ;
      res = f_read(&file, buffer+4, 4092, &br);
      br  += 4; 			
			while(res == FR_OK)
			{
				 if(br > 0 )
				 {
					  // 从第二扇区开始第一个字节进行和校验
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
					  //  存储首四个字节为文件长度
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



//   从内存卡加载字库信息到W25Qxx
void WS_W25Qxx_Font_Chenk_Write(void)
{
	  uint16_t  i ;	
//  打印内存卡信息	
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

//   读字库信息 16* 16
//   数据缓存地址
//   string: 汉字内码
WS_StatusTypeDef WS_W25Qxx_Font16_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font16Flag != 0)
		{
			 //WS_Debug(0,"MSB:%d,LSB:%d\r\n",string[0],string[1]);
			         //  字库首扇区 + 偏移扇区
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


//   读字库信息 24* 24
//   数据缓存地址
//   string: 汉字内码
WS_StatusTypeDef WS_W25Qxx_Font24_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font24Flag != 0)
		{          //  字库首扇区 + 偏移扇区
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

//   读字库信息 32* 32
//   数据缓存地址
//   string: 汉字内码
WS_StatusTypeDef WS_W25Qxx_Font32_Read_Date(uint8_t * buffer  , uint8_t *string)
{
	  uint32_t addr ;
    if(W25QxxInfo.font32Flag != 0)
		{          //  字库首扇区 + 偏移扇区
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





//用于文件系统读写等待空闲,干等
void WS_W25Qxx_Fatfs_Wait_Idle(void)   
{  	 
	while((WS_W25Qxx_Read_SR()&0x01) == 0x01) ; // 等待BUSY位清空  		            
}

//    用于文件系统的读W25Qxx数据函数
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
    WS_W25Qxx_Fatfs_Wait_Idle();   				   		   //等待擦除完成
} 

//    用于文件系统的写W25Qxx页（最大256）数据函数
WS_StatusTypeDef  WS_W25Qxx_Fatfs_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)       
{
  
  WS_W25Qxx_Write_Enable();                   	         
	WS_W25Qxx_CS_RESET  ;                            	  //使能器件   
	WS_W25Qxx_Write_Byte(WS_W25Qxx_Reg_PageProgram);      	      //发送写页命令   
	WS_W25Qxx_Write_Byte((uint8_t)((addr)>>16)); 	//发送24bit地址    
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

	//   连续写入扇区
	while(sectorCount >  0)
	{
		
		  l =   startFatfsSector % WS_W25Qxx_Fatfs_Page_Nbr ;
		  //  拆分需要写入得连续扇区
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
			chipSector =addr / 4096; // 芯片扇区地址  
			addrAdd    =addr % 4096; // 在扇区内的偏移
		

			 //   读扇区所有数据
			 WS_W25Qxx_Fatfs_Read_Sector (W25QxxWriteBuffer, chipSector * (WS_W25Qxx_Fatfs_Page_Nbr) 
																	 ,WS_W25Qxx_Fatfs_Page_Nbr) ;
			 //WS_W25Qxx_Read(W25QxxWriteBuffer,chipSector * 4096 , 4096);
			 
			 //   将带写入的数据拷贝到指定位置
			 memcpy(W25QxxWriteBuffer + addrAdd , buffer , size);
			 //  计算写入地址
			 uint32_t writeAddr = chipSector  *   4096 ;
			 
			 //  擦除对应扇区
			 WS_W25Qxx_Fatfs_Erase_Sector(chipSector);  
			 
			 //  分16页写入
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
















