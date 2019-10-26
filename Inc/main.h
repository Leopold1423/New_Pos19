/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct Flag
{
  int fivems;
  int fiftyms;
  int halfs;
  int ones;
  int wave;
  int readpos;
  int readcircle;
  int readangle;
  int test1;
  int test2;
  int test3;
}Flag;   
extern Flag flag;

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
#define CSNM2_Pin GPIO_PIN_13
#define CSNM2_GPIO_Port GPIOC
#define CSNM3_Pin GPIO_PIN_14
#define CSNM3_GPIO_Port GPIOC
#define CSNM1_Pin GPIO_PIN_15
#define CSNM1_GPIO_Port GPIOC
#define CLK1_Pin GPIO_PIN_5
#define CLK1_GPIO_Port GPIOA
#define MISO1_Pin GPIO_PIN_6
#define MISO1_GPIO_Port GPIOA
#define MOSI1_Pin GPIO_PIN_7
#define MOSI1_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_9
#define KEY1_GPIO_Port GPIOA
#define CLK3_Pin GPIO_PIN_10
#define CLK3_GPIO_Port GPIOC
#define MISO3_Pin GPIO_PIN_11
#define MISO3_GPIO_Port GPIOC
#define MOSI3_Pin GPIO_PIN_12
#define MOSI3_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
