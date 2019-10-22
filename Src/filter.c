#include "filter.h"
#include "stdlib.h"

kal_filter *k_flt;
sec_filter *sec_struct;

void filter_init()
{
  k_flt = (kal_filter *)malloc(sizeof(kal_filter));
  sec_struct =  (sec_filter *)malloc(sizeof(sec_filter));
  k_flt->Q=1.0;
  k_flt->R=1.0;
  k_flt->P=0;
  k_flt->X=0;
  k_flt->K=0;
  k_flt->P_last=0;
  k_flt->X_last=0;
}

float kalman_filter(kal_filter *k_flt,float input){

//测量值更新
k_flt->input = input;
k_flt->K = (k_flt->P_last)/(k_flt->P_last+k_flt->R);
k_flt->X = k_flt->X_last + k_flt->K*(k_flt->input-k_flt->X_last);
k_flt->P = (1-k_flt->K)*(k_flt->P_last);
//时间更新
k_flt->X_last = k_flt->X;                  //不含控制项
k_flt->P_last = k_flt->P+k_flt->Q;
return k_flt->X;
}


float second_filter(sec_filter *sec_struct,float input){
sec_struct->cur_output=(sec_struct->a)*input+(sec_struct->b)*(sec_struct->last_output)
+(sec_struct->c)*(sec_struct->pre_output);
sec_struct->pre_output=sec_struct->last_output;
sec_struct->last_output=sec_struct->cur_output;
return sec_struct->cur_output;
}


float adc_filter(uint16_t *data,uint16_t num) {
static uint16_t max=0,min=0,sum=0; 
for (int i=0; i<num; i++) {
if (max < *(data+i))
max = *(data+i);//保存最大值
if (min > *(data+i))
min = *(data+i);//保存最小值
sum += *(data+i);
}
sum = sum - max - min;
float average = (float)sum/(num-2);//取平均值
return average;
}