

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


#include "ws_main.h"

//      �������ݴ�������һҳ
#define WS_Flash_Page_Size 1024        // оƬ�洢ҳ����
#define WS_Flash_Config_Page_Addr 63   // ������Ϣ�洢��ҳ��ַ

//   ǰ16K��0~15ҳ����Bootloaderʹ�ã���16~59ҳ��44K ��APP
#define WS_Flash_APP_CODE_ADDR    FLASH_BASE + WS_Flash_Page_Size * (WS_Flash_APP_Start_Page)
#define WS_Flash_APP_Start_Page   16    
#define WS_Flash_APP_MAX_Page     59


HAL_StatusTypeDef WS_Flash_Erase_Write_Page(int page , uint8_t *p_buffer  , int size );

HAL_StatusTypeDef WS_Config_Write_Struct(uint8_t *buffer  , int size );

HAL_StatusTypeDef WS_Config_Read_Struct(uint8_t *buffer  , int size );

HAL_StatusTypeDef WS_Flash_Erase_Page(int startPage , int endPage );

HAL_StatusTypeDef WS_Flash_Write_Page(int page , uint8_t *p_buffer  , int size );

HAL_StatusTypeDef WS_Flash_Read_Page(int page , uint8_t *buffer  , int size );
