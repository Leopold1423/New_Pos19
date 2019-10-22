#ifndef __encoder_H
#define __encoder_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include "main.h"     
 

typedef struct Encoder
{
  int encoderX_dir;
  int encoderY_dir;
  float X;
  float Y;
}Encoder;  
extern Encoder encoder;

void encoder_init(void);      
     

        
#ifdef __cplusplus
}
#endif
#endif /*__ encoder_H */
