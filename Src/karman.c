#include "karman.h"
#include "read_data_simple.h"

/*
	卡尔曼滤波函数
*/
Kal_Struct kal_ang1={1,0,0.01,37.1160,0,1};
Kal_Struct kal_ang2={1,0,0.01,37.1160,0,1};
Kal_Struct kal_ang3={1,0,0.01,37.1160,0,1};
Kal_Struct kal_ang4={1,0,0.01,37.1160,0,1};
Kal_Struct kal_ang5={1,0,0.01,37.1160,0,1};
Kal_Struct kal_ang6={1,0,0.01,37.1160,0,1};

float KalMan(Kal_Struct *kal,float x){
	
	float kalman_pre;  //卡尔曼的预测值
	float cov_pre;  //卡尔曼预测值的协方差

	
	float kg;//增益
	kalman_pre=kal->kal_out*kal->A;  //计算本次卡尔曼的预测值
	
	cov_pre=kal->cov*kal->A*kal->A+kal->Q;
	
	kg=cov_pre/(cov_pre+kal->R);   //计算本次的卡尔曼增益
	
	kal->kal_out=kalman_pre+kg*(x-kalman_pre);   //通过预测值来计算本次卡尔曼滤波后的输出
	
	kal->cov=(1-kg)*cov_pre;
	
	return kal->kal_out;
}


 
float acc_angle_x_buff[6] = {0};
float acc_angle_y_buff[6] = {0};
float acc_angle_z_buff[6] = {0};
float acc_x_buff[6] = {0};
float acc_y_buff[6] = {0};
float acc_z_buff[6] = {0};
int i = 0;
void Slide(Angle* angle){
  int count;
  float sum_acc_angle_x = 0;
  float sum_acc_angle_y = 0;
  float sum_acc_angle_z = 0;
  float sum_acc_x = 0;
  float sum_acc_y = 0;
  float sum_acc_z = 0;
  
   angle->angular_rate_dps[0] = KalMan(&kal_ang1,angle->angular_rate_dps[0]);
   angle->angular_rate_dps[1] = KalMan(&kal_ang2,angle->angular_rate_dps[1]);
   angle->angular_rate_dps[2] = KalMan(&kal_ang3,angle->angular_rate_dps[2]);
   angle->acceleration_g[0] = KalMan(&kal_ang4,angle->acceleration_g[0]);
   angle->acceleration_g[1] = KalMan(&kal_ang5,angle->acceleration_g[1]);
   angle->acceleration_g[2] = KalMan(&kal_ang6,angle->acceleration_g[2]);
    
   for(int j=0;j<5;j++)
   {
   acc_angle_x_buff[j] = acc_angle_x_buff[j+1];
   acc_angle_y_buff[j] = acc_angle_y_buff[j+1];
   acc_angle_z_buff[j] = acc_angle_z_buff[j+1];
   acc_x_buff[j] = acc_x_buff[j+1];
   acc_y_buff[j] = acc_y_buff[j+1];
   acc_z_buff[j] = acc_z_buff[j+1];
   }   
   acc_angle_x_buff[5] = angle->angular_rate_dps[0];
   acc_angle_y_buff[5] = angle->angular_rate_dps[1];
   acc_angle_z_buff[5] = angle->angular_rate_dps[2];
   acc_x_buff[5] = angle->acceleration_g[0];
   acc_y_buff[5] = angle->acceleration_g[1];
   acc_z_buff[5] = angle->acceleration_g[2];
   
   
//   acc_angle_x_buff[i++] = angle->angular_rate_dps[0];
//   acc_angle_y_buff[i++] = angle->angular_rate_dps[1];
//   acc_angle_z_buff[i++] = angle->angular_rate_dps[2];
//   acc_x_buff[i++] = angle->acceleration_g[0];
//   acc_y_buff[i++] = angle->acceleration_g[1];
//   acc_z_buff[i++] = angle->acceleration_g[2];
//   
//  if(i == 6) i = 0;
//  
  for(count = 0; count < 6; count++)
  {
    sum_acc_angle_x += acc_angle_x_buff[count];
    sum_acc_angle_y += acc_angle_y_buff[count];
    sum_acc_angle_z += acc_angle_z_buff[count];
    sum_acc_x += acc_x_buff[count];
    sum_acc_y += acc_y_buff[count];
    sum_acc_z += acc_z_buff[count];    
  }
   


  angle->angular_rate_dps[0] = (float)(sum_acc_angle_x/6);
  angle->angular_rate_dps[1] = (float)(sum_acc_angle_y/6);
  angle->angular_rate_dps[2] = (float)(sum_acc_angle_z/6);
  angle->acceleration_g[0] = (float)(sum_acc_x/6);
  angle->acceleration_g[1] = (float)(sum_acc_y/6);
  angle->acceleration_g[2] = (float)(sum_acc_z/6);
  
}

/*void kalman_all(Angle* angle)
{
    angle->acc_angle_x = KalMan(&kal_ang1,angle->acc_angle_x);
    angle->acc_angle_y = KalMan(&kal_ang2,angle->acc_angle_y);
    angle->acc_angle_z = KalMan(&kal_ang3,angle->acc_angle_z);
    angle->acc_x = KalMan(&kal_ang4,angle->acc_x);
    angle->acc_y = KalMan(&kal_ang5,angle->acc_y);
    angle->acc_z = KalMan(&kal_ang6,angle->acc_z);
}*/