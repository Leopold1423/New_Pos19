/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "simplelib.h"
#include "flash.h"
#include "can_utils.h"

#include "calculate.h"
#include "as5047p.h"
#include "read_data_simple.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
Flag flag;
static uint32_t time_ms = 0;

void flag_init()
{
  flag.fivems=0;
  flag.fiftyms=0;
  flag.halfs=0;
  flag.ones=0;
  flag.readpos=0;
  flag.readcircle=0;
  flag.readangle=0;
  flag.wave=0; 
  flag.test1=0;
  flag.test2=0;
  flag.test3=0;
}


extern float vega;
/* USER CODE END 0 */

/**
* @brief  The application entry point.
* @retval int
*/
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */
  
  
  /* MCU Configuration--------------------------------------------------------*/
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  
  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */
  
  /* Configure the system clock */
  SystemClock_Config();
  
  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */
  
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_I2C3_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  simplelib_init(&huart2, &hcan1);  
  uprintf("hello\r\n");
  //load_prams();
  flag_init();
  asm330lhh_init();
  wheel_init(); 
  can_msg test;
  can_send_msg(0x09,&test,8);//vega³õÊ¼»¯
  /* USER CODE END 2 */
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
    simplelib_run();
    if(flag.fivems == 1)
    {    
      flag.fivems=0;
      Get_Yaw_angle();
      //Get_Yaw_angle_0();
      Get_Wheel_x();
      Get_Wheel_y();  
      //send_Angle(12);
      if(flag.test1==1)
      {
        send_wave(pre_angle.acceleration_g[0],0,pre_angle.acceleration_g[1],pre_angle.acceleration_g[2]);  
      }
      if(flag.test2==1)
      {
        send_wave(pre_angle.angular_rate_dps[0],0,pre_angle.angular_rate_dps[1],pre_angle.angular_rate_dps[2]);
      }
      if(flag.test3==1)
      {
        send_wave(angle.yawangle[0],0,angle.yawangle[1],angle_toshow(angle.yawangle[2]));       
      }         
      
      if(flag.wave==1)
      {
        send_wave(position.world_x,0,position.world_y,position.world_yaw);       
      }  
    }
    if(flag.fiftyms==1)
    {
      flag.fiftyms=0;  
    }
    if(flag.halfs==1)
    {
      flag.halfs=0;
      if(flag.test1==2)
      {
        uprintf("%f  %f  %f  %f\r",vega,angle_toshow(angle.yawangle[2]),angle_toshow(vega-angle_toshow(angle.yawangle[2])),angle.yawangle[2]);
        
      }
      if(flag.test2==2)
      {
        uprintf("%f\r",angle.yawangle[2]);
//        if(pre_angle.angular_rate_dps[2]!=0)
//          uprintf("%f %f %f\r",angle.angular_rate[2],pre_angle.angular_rate_dps[2],angle.angular_rate[2]/pre_angle.angular_rate_dps[2]);
        //          uprintf("%f      %f      %f                   %f\r",
        //                  pre_angle.angular_rate_dps[0],pre_angle.angular_rate_dps[1],pre_angle.angular_rate_dps[2],angle.angular_rate[2]); 
      }
      if(flag.test3==2)
      {
        uprintf("%f  %f %f\r",angle.yawangle[0],angle.yawangle[1],angle.yawangle[2]);
      }
    }
    if(flag.ones==1)
    {    
      flag.ones=0;
      
      if(flag.readangle==1)
      {
        uprintf("%f \r",angle_toshow(angle.yawangle[2]));
      }   
      if(flag.readpos==1)
      {
        uprintf("x=%f     y=%f     angle=%f\r",position.world_x,position.world_y,position.world_yaw);
      }
      if(flag.readcircle==1)
      {
        uprintf("x=%f     y=%f \r",wheel_x.now_circlenum,wheel_y.now_circlenum); 
      }        
      
    }
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
* @brief System Clock Configuration
* @retval None
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_IncTick(void)
{  
  time_ms++; 
  if (time_ms%5 == 0) 
  {
    flag.fivems = 1;
  }
  if (time_ms%500 == 0) 
  {
    flag.halfs = 1;
  }
  if (time_ms%50 == 0) 
  {
    flag.fiftyms = 1;
  }
  if (time_ms%1000 == 0) 
  {
    flag.ones = 1;
  }
  if(time_ms>=64000)
  {
    time_ms=0;
  }
}
/* USER CODE END 4 */

/**
* @brief  This function is executed in case of error occurrence.
* @retval None
*/
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
