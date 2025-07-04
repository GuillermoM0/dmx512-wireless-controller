/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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
#define NRF24_CE_Pin GPIO_PIN_4
#define NRF24_CE_GPIO_Port GPIOA
#define NRF24_CSN_Pin GPIO_PIN_5
#define NRF24_CSN_GPIO_Port GPIOA
#define NRF24_IRQ_Pin GPIO_PIN_6
#define NRF24_IRQ_GPIO_Port GPIOA
#define NRF24_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define MAX485_TX_Pin GPIO_PIN_6
#define MAX485_TX_GPIO_Port GPIOC
#define MAX485_RX_Pin GPIO_PIN_7
#define MAX485_RX_GPIO_Port GPIOC
#define MAX485_RE_Pin GPIO_PIN_8
#define MAX485_RE_GPIO_Port GPIOC
#define MAX485_DE_Pin GPIO_PIN_9
#define MAX485_DE_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
