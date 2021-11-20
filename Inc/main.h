/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Relay1_Pin GPIO_PIN_3
#define Relay1_GPIO_Port GPIOE
#define EX1_Pin GPIO_PIN_5
#define EX1_GPIO_Port GPIOE
#define EX1_EXTI_IRQn EXTI9_5_IRQn
#define EX2_Pin GPIO_PIN_6
#define EX2_GPIO_Port GPIOE
#define EX2_EXTI_IRQn EXTI9_5_IRQn
#define RS485_TX_EN_Pin GPIO_PIN_13
#define RS485_TX_EN_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_1
#define SW2_GPIO_Port GPIOC
#define SW2_EXTI_IRQn EXTI1_IRQn
#define SW3_Pin GPIO_PIN_2
#define SW3_GPIO_Port GPIOC
#define SW3_EXTI_IRQn EXTI2_IRQn
#define SW4_Pin GPIO_PIN_3
#define SW4_GPIO_Port GPIOC
#define SW4_EXTI_IRQn EXTI3_IRQn
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA
#define ADC2_Pin GPIO_PIN_1
#define ADC2_GPIO_Port GPIOA
#define W25_CS_Pin GPIO_PIN_6
#define W25_CS_GPIO_Port GPIOA
#define SW5_Pin GPIO_PIN_4
#define SW5_GPIO_Port GPIOC
#define SW5_EXTI_IRQn EXTI4_IRQn
#define AD_temper_Pin GPIO_PIN_5
#define AD_temper_GPIO_Port GPIOC
#define LED_DEBUG_Pin GPIO_PIN_2
#define LED_DEBUG_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_12
#define Buzzer_GPIO_Port GPIOB
#define LCD_T_PENIRQ_Pin GPIO_PIN_11
#define LCD_T_PENIRQ_GPIO_Port GPIOD
#define LCD_T_PENIRQ_EXTI_IRQn EXTI15_10_IRQn
#define Relay2_Pin GPIO_PIN_13
#define Relay2_GPIO_Port GPIOD
#define LED_RUN_Pin GPIO_PIN_6
#define LED_RUN_GPIO_Port GPIOC
#define LCD_Light_Pin GPIO_PIN_3
#define LCD_Light_GPIO_Port GPIOD
#define LCD_T_CS_Pin GPIO_PIN_6
#define LCD_T_CS_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
