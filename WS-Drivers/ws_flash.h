

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


#include "ws_main.h"

//      配置数据存放在最后一页
#define WS_Flash_Page_Size 1024        // 芯片存储页长度
#define WS_Flash_Config_Page_Addr 63   // 配置信息存储的页地址

//   前16K（0~15页）给Bootloader使用，后（16~59页）44K 给APP
#define WS_Flash_APP_CODE_ADDR    FLASH_BASE + WS_Flash_Page_Size * (WS_Flash_APP_Start_Page)
#define WS_Flash_APP_Start_Page   16    
#define WS_Flash_APP_MAX_Page     59


HAL_StatusTypeDef WS_Flash_Erase_Write_Page(int page , uint8_t *p_buffer  , int size );

HAL_StatusTypeDef WS_Config_Write_Struct(uint8_t *buffer  , int size );

HAL_StatusTypeDef WS_Config_Read_Struct(uint8_t *buffer  , int size );

HAL_StatusTypeDef WS_Flash_Erase_Page(int startPage , int endPage );

HAL_StatusTypeDef WS_Flash_Write_Page(int page , uint8_t *p_buffer  , int size );

HAL_StatusTypeDef WS_Flash_Read_Page(int page , uint8_t *buffer  , int size );
