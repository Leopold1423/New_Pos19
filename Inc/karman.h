#ifndef __karman_H
#define __karman_H

#include "i2c.h"
#include "stm32f4xx_hal.h"
#include "angle.h"


typedef struct _kal_struct{
	float A;   //һ��Ϊ1
	float B;   //һ��Ϊ0
	float Q;//ϵͳ����������Э����
	float R;//����������Э����	
	float kal_out; //��һ�ο����������	
	float cov; //��һ�ο������������Э����	
}Kal_Struct;

float KalMan(Kal_Struct *kal,float x);
void Slide(Angle* angle);
void kalman_all(Angle* angle);



#endif