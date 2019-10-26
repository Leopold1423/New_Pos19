#ifndef __angle_H
#define __angle_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "stm32f4xx_hal.h"
#include "main.h"
#include "math.h"
#include "usart.h"
     #include "read_data_simple.h"



void IMU_Update(Angle* angle,float ax,float ay,float az,float wx,float wy,float wz);
     
     
     
     
     
     
     
     
     
     
     
     
     
#ifdef __cplusplus
}
#endif
#endif /*__ angle_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/