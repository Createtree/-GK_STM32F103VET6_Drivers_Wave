


#ifndef __FLASH_H
#define __FLASH_H	

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q16
//容量为2M字节,共有16个Block,512个Sector 


#include "ws_main.h" 

//       外设定义
#define  WS_Peripheral_W25Qxx        1
#define  WS_W25Qxx_Sector_Size       4096 


//    所使用的SPI编号
#define  W25Qxx_hspi hspi2

//    片选脚定义
#define WS_W25Qxx_CS_RESET 		   W25_CS_GPIO_Port->ODR &= ~(W25_CS_Pin)		
#define WS_W25Qxx_CS_SET	 		   W25_CS_GPIO_Port->ODR |=  (W25_CS_Pin)	
	
//   枚举芯片类型	
typedef enum 
{
	W25Qxx_Not = 0        ,
  W25Qxx_16  =	0XEF14  ,
  W25Qxx_32  =	0XEF15  ,
  W25Qxx_64  =	0XEF16  ,
  W25Qxx_128 =	0XEF17  ,
} WS_W25QxxTypeDef ;


//      文件存储信息
typedef struct _WS_ConfigFileTypeDef 
{
	 char *name ;
	 uint32_t   startSector ;
	 uint32_t   endSector   ;
} WS_ConfigFileTypeDef    ;


/** 
  * @brief  W25Qxx Information Structure definition
  */ 
typedef struct
{
  WS_W25QxxTypeDef chipType;          /*!< Specifies the card Type                          */
  
  uint32_t chipSectorNbr ;            /*!< Specifies the Card Capacity in sectors           */
  uint32_t chipSectorSize;            /*!< Specifies one sector size in bytes               */
  
	
  uint32_t fatfsSectorNbr  ;          /*!< Specifies the Card fatfs Capacity in sectors     */
  uint32_t fatfsSectorSize ;          /*!< Specifies fatfs sector size in bytes             */
  
	uint32_t fontStartSector ;
	uint8_t  font16Flag      ;
	uint8_t  font24Flag      ;
	uint8_t  font32Flag      ;

}WS_W25Qxx_FlashInfoTypeDef;




//     全局变量，外挂flash芯片类型
extern WS_W25QxxTypeDef WS_W25Qxx_Chip_Type  ;	//全局芯片类型	   
extern WS_W25Qxx_FlashInfoTypeDef W25QxxInfo ;  //全局


//      后半空间开始存储自定义信息
#define WS_W25Qxx_FONT16_Base_Sector  (W25QxxInfo.fontStartSector)
#define WS_W25Qxx_FONT16_Start_Sector (0  )  //  0开始 ，   70  结束  ， 用71扇区
#define WS_W25Qxx_FONT24_Start_Sector (71 )  //  71开始 ，  266 结束  ， 用156扇区
#define WS_W25Qxx_FONT32_Start_Sector (267)  //  267开始 ， 544 结束  ， 用277扇区

//      前半部分存储文件系统
//#define WS_W25Qxx_Fatfs_Sector_Number  2048 * 8
#define WS_W25Qxx_Fatfs_Sector_Size    512
#define WS_W25Qxx_Fatfs_Page_Nbr       (WS_W25Qxx_Sector_Size/WS_W25Qxx_Fatfs_Sector_Size)


 
//指令表
#define WS_W25Qxx_Reg_ID	     0x90 
#define WS_W25Qxx_Reg_Read		 0x05 
#define WS_W25Qxx_Reg_Write		 0x01 

#define WS_W25Qxx_Reg_WriteEnable		   0x06 
#define WS_W25Qxx_Reg_WriteDisable		 0x04 
#define WS_W25Qxx_Reg_ReadData			   0x03 
#define WS_W25Qxx_Reg_PageProgram		   0x02 


#define WS_W25Qxx_Reg_Erase_Sector		   0x20 
#define WS_W25Qxx_Reg_Erase_Block			   0xD8 
#define WS_W25Qxx_Reg_Erase_Chip			   0xC7 


#define WS_W25Qxx_Reg_PowerDown			   0xB9 
#define WS_W25Qxx_Reg_PowerUp	         0xAB 
#define WS_W25Qxx_Reg_DeviceID			   0xAB 


WS_W25QxxTypeDef   WS_W25Qxx_Init(void);        // 初始化
void               WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint32_t len) ;
WS_StatusTypeDef   WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint32_t len) ;
void               WS_W25Qxx_Font_Chenk_Write(void);

uint16_t           WS_W25Qxx_Read_ID(void);  	    		// 读取FLASH ID
WS_StatusTypeDef   WS_W25Qxx_Fatfs_Write_Page(uint8_t* buffer,uint32_t addr,uint16_t len)   ;

WS_StatusTypeDef   WS_W25Qxx_Fatfs_Write_Sector(uint8_t* buffer,uint32_t startFatfsSector,uint32_t fatfsSectorCount);   
WS_StatusTypeDef   WS_W25Qxx_Fatfs_Read_Sector(uint8_t* buffer,uint32_t startFatfsSector,uint32_t fatfsSectorCount) ; 
void               WS_W25Qxx_Fatfs_Erase_Sector(uint32_t sector)    ;
WS_StatusTypeDef   WS_W25Qxx_Font16_Read_Date(uint8_t * buffer  , uint8_t *string);
WS_StatusTypeDef   WS_W25Qxx_Font24_Read_Date(uint8_t * buffer  , uint8_t *string);
WS_StatusTypeDef   WS_W25Qxx_Font32_Read_Date(uint8_t * buffer  , uint8_t *string);




WS_StatusTypeDef   WS_W25Qxx_Sector_Write( uint8_t *buffer  ,uint32_t sector , uint32_t size);	
void               WS_W25Qxx_Erase_Sector(uint32_t sector)  ;	
void               WS_W25Qxx_Wait_Idle(void);  
uint8_t	           WS_W25Qxx_Read_SR(void);
void               WS_W25Qxx_Write_SR(uint8_t sr);  	

#endif
















