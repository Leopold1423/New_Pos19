#include "encoder.h"
#include "tim.h"
void encoder_init()
{   
    HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);
}


