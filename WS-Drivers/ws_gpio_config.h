
/******************************************************************************
 * @file     				 ws_gpio_config.h
 * @brief    				 GPIO及资源分配
 * @version  				 1.0.0
 * @creationTime     2020_03_29
 * @creator					 
 * @readme
 *        1.STM32F030系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_GPIO_CONFIG_H__
#define _WS_GPIO_CONFIG_H__


#include "main.h"



#define  USART1_RECEIVE_BUFFER_SIZE 128
#define  USART2_RECEIVE_BUFFER_SIZE 128
#define  USART3_RECEIVE_BUFFER_SIZE 128
 


#define openRunLed()       LED_RUN_GPIO_Port->ODR &= ~LED_RUN_Pin;
#define closeRunLed()      LED_RUN_GPIO_Port->ODR |=  LED_RUN_Pin;
#define triggerRunLed()    LED_RUN_GPIO_Port->ODR ^=  LED_RUN_Pin;

#define openDebugLed()       LED_DEBUG_GPIO_Port->ODR &= ~LED_DEBUG_Pin;
#define closeDebugLed()      LED_DEBUG_GPIO_Port->ODR |=  LED_DEBUG_Pin;
#define triggerDebugLed()    LED_DEBUG_GPIO_Port->ODR ^=  LED_DEBUG_Pin;


#define WS_define_Borad_Open_Buzzer       Buzzer_GPIO_Port->ODR &= ~Buzzer_Pin;
#define WS_define_Borad_Close_Buzzer      Buzzer_GPIO_Port->ODR |= Buzzer_Pin;

#define WS_define_Borad_Open_Relay1       Relay1_GPIO_Port->ODR &= ~Relay1_Pin;
#define WS_define_Borad_Close_Relay1      Relay1_GPIO_Port->ODR |= Relay1_Pin;
#define WS_define_Borad_Trigger_Relay1    Relay1_GPIO_Port->ODR ^= Relay1_Pin;

#define WS_define_Borad_Open_Relay2       Relay2_GPIO_Port->ODR &= ~Relay2_Pin;
#define WS_define_Borad_Close_Relay2      Relay2_GPIO_Port->ODR |= Relay2_Pin;
#define WS_define_Borad_Trigger_Relay2    Relay2_GPIO_Port->ODR ^= Relay2_Pin;


//  MP3模块空闲标志
#define WS_define_Audio_IS_Ready  (HAL_GPIO_ReadPin(MP3_BUSY_GPIO_Port, MP3_BUSY_Pin) != GPIO_PIN_RESET) 
#define WS_define_Audio_IS_Stop   (WS_define_Audio_IS_Ready)
#define WS_define_Audio_IS_Busy   (HAL_GPIO_ReadPin(MP3_BUSY_GPIO_Port, MP3_BUSY_Pin) == GPIO_PIN_RESET) 



#define WS_define_RS485_TX_EN  RS485_TX_EN_GPIO_Port->ODR |= RS485_TX_EN_Pin;
#define WS_define_RS485_TX_DI  RS485_TX_EN_GPIO_Port->ODR &= ~RS485_TX_EN_Pin;

#define WS_define_RS485_UART    USART2
#define WS_define_RS485_huart   huart2

#define WS_define_ESP8266_UART    USART1
#define WS_define_ESP8266_huart   huart1



#define IN_Borad_CS_RESET IN_Borad_CS_GPIO_Port->ODR &= ~IN_Borad_CS_Pin;
#define IN_Borad_CS_SET   IN_Borad_CS_GPIO_Port->ODR |=  IN_Borad_CS_Pin;


#define OUT_Borad_CS_RESET OUT_Borad_CS_GPIO_Port->ODR &= ~OUT_Borad_CS_Pin;
#define OUT_Borad_CS_SET   OUT_Borad_CS_GPIO_Port->ODR |=  OUT_Borad_CS_Pin;







#endif

/*********************************************************************************************************
                                             END FILE           
*********************************************************************************************************/



