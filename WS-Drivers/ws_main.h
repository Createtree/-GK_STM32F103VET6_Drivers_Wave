
/******************************************************************************
 * @file     				 ws_main.h
 * @brief    				 包含系统所需的头文件
 * @version  				 1.0.0
 * @creationTime     2020_03_29
 * @creator					 
 * @readme
 *        1.STM32F030系列   
 *				2. 
          3.
*******************************************************************************/

#ifndef _WS_MAIN_H__
#define _WS_MAIN_H__


	#include "main.h"
	#include "cmsis_os.h"
	#include "stdio.h"
	#include "stdlib.h"
  #include "ws_core.h"  
	#include "kernel.h"
	#include "string.h"
	#include "fatfs.h"
	#include "sd_diskio.h"
	
	
	#include "ws_config.h"
	
	#include "ws_borad_base.h"

  #include "ws_task.h"
//	#include "ws_flash.h"
	#include "ws_bus.h"
  #include "ws_capture.h"
	#include "ws_esp8266.h"
	#include "ws_nrf24L01.h"
	#include "ws_leds.h"
//	#include "ws_oled.h"
	#include "ws_w25qxx.h" 
	#include "ws_ili9xxx.h"
 
	
	
		
	#include "ws_gpio_config.h"
	
	
#endif
