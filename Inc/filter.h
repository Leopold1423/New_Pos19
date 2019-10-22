#include "stm32f4xx_hal.h"
#include "usart.h"
   typedef struct {
     float input;
     float X;
     float K;
     float P;
     float R;
     float Q;
     float P_last;
     float X_last;     
   }kal_filter;
   extern kal_filter *k_flt;
   
   typedef struct {
     float a;
     float b;
     float c;
     float pre_output;
     float last_output;
     float cur_output;    
   }sec_filter;
   extern sec_filter *sec_struct;
   
 float kalman_filter(kal_filter *k_flt,float input);
 float second_filter(sec_filter *sec_struct,float input);
 float adc_filter(uint16_t *data,uint16_t num);
 void filter_init();

#ifdef __cplusplus
}
#endif
