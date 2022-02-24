/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define DAC0_Pin GPIO_PIN_0
#define DAC0_GPIO_Port GPIOA
#define DAC1_Pin GPIO_PIN_1
#define DAC1_GPIO_Port GPIOA
#define DAC2_Pin GPIO_PIN_2
#define DAC2_GPIO_Port GPIOA
#define DAC3_Pin GPIO_PIN_3
#define DAC3_GPIO_Port GPIOA
#define DAC4_Pin GPIO_PIN_4
#define DAC4_GPIO_Port GPIOA
#define DAC5_Pin GPIO_PIN_5
#define DAC5_GPIO_Port GPIOA
#define DAC6_Pin GPIO_PIN_6
#define DAC6_GPIO_Port GPIOA
#define DAC7_Pin GPIO_PIN_7
#define DAC7_GPIO_Port GPIOA
#define ENC_BTN_Pin GPIO_PIN_10
#define ENC_BTN_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
