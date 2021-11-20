


#ifndef __FLASH_H
#define __FLASH_H	

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q16
//����Ϊ2M�ֽ�,����16��Block,512��Sector 


#include "ws_main.h" 

//       ���趨��
#define  WS_Peripheral_W25Qxx        1
#define  WS_W25Qxx_Sector_Size       4096 


//    ��ʹ�õ�SPI���
#define  W25Qxx_hspi hspi2

//    Ƭѡ�Ŷ���
#define WS_W25Qxx_CS_RESET 		   W25_CS_GPIO_Port->ODR &= ~(W25_CS_Pin)		
#define WS_W25Qxx_CS_SET	 		   W25_CS_GPIO_Port->ODR |=  (W25_CS_Pin)	
	
//   ö��оƬ����	
typedef enum 
{
	W25Qxx_Not = 0        ,
  W25Qxx_16  =	0XEF14  ,
  W25Qxx_32  =	0XEF15  ,
  W25Qxx_64  =	0XEF16  ,
  W25Qxx_128 =	0XEF17  ,
} WS_W25QxxTypeDef ;


//      �ļ��洢��Ϣ
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




//     ȫ�ֱ��������flashоƬ����
extern WS_W25QxxTypeDef WS_W25Qxx_Chip_Type  ;	//ȫ��оƬ����	   
extern WS_W25Qxx_FlashInfoTypeDef W25QxxInfo ;  //ȫ��


//      ���ռ俪ʼ�洢�Զ�����Ϣ
#define WS_W25Qxx_FONT16_Base_Sector  (W25QxxInfo.fontStartSector)
#define WS_W25Qxx_FONT16_Start_Sector (0  )  //  0��ʼ ��   70  ����  �� ��71����
#define WS_W25Qxx_FONT24_Start_Sector (71 )  //  71��ʼ ��  266 ����  �� ��156����
#define WS_W25Qxx_FONT32_Start_Sector (267)  //  267��ʼ �� 544 ����  �� ��277����

//      ǰ�벿�ִ洢�ļ�ϵͳ
//#define WS_W25Qxx_Fatfs_Sector_Number  2048 * 8
#define WS_W25Qxx_Fatfs_Sector_Size    512
#define WS_W25Qxx_Fatfs_Page_Nbr       (WS_W25Qxx_Sector_Size/WS_W25Qxx_Fatfs_Sector_Size)


 
//ָ���
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


WS_W25QxxTypeDef   WS_W25Qxx_Init(void);        // ��ʼ��
void               WS_W25Qxx_Read(uint8_t* buffer,uint32_t addr,uint32_t len) ;
WS_StatusTypeDef   WS_W25Qxx_Write(uint8_t* buffer,uint32_t addr,uint32_t len) ;
void               WS_W25Qxx_Font_Chenk_Write(void);

uint16_t           WS_W25Qxx_Read_ID(void);  	    		// ��ȡFLASH ID
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















