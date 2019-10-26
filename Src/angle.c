#include "angle.h"
#include "read_data_simple.h"
#define GY521 0
#define Semig 1e-6
#define INTEGRAL_CONSTANT 0.005		//积分时间 5ms
#define HALF_T 0.0025

/*
	四元数函数
*/

float q0=1;
float q1=0;
float q2=0;
float q3=0;

float IMU_P=2;
float IMU_I=0.005;


void IMU_Update(Angle* angle,float ax,float ay,float az,float wx,float wy,float wz)
{
    float norm;
    float gbx,gby,gbz;
    float q0q0 = q0 * q0;                                                        
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q1q1 = q1 * q1;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    float q1q2=q1*q2;
    float q0q3=q0*q3;
    float ex,ey,ez;
    float exInt = 0 ,eyInt = 0 ,ezInt = 0;
    norm=sqrt(ax*ax+ay*ay+az*az);
    if(norm<Semig)
        return ;
    
    //测量正常化
    ax/=norm;
    ay/=norm;
    az/=norm;
    
    //计算重力加速度旋转到机体坐标系后的值
    gbx= 2*(q1q3 - q0q2);
    gby= 2*(q0q1 + q2q3);
    gbz= q0q0 - q1q1 - q2q2 + q3q3;
    
    //与实际加速度计测得的ax,ay,az做叉积，取误差
    
    
    ex = (ay*gbz - az*gby);                                                                
    ey = (az*gbx - ax*gbz);
    ez = (ax*gby - ay*gbx);	
    
    //数据融合
    exInt += ex*IMU_I*INTEGRAL_CONSTANT;
    eyInt += ey*IMU_I*INTEGRAL_CONSTANT;
    ezInt += ez*IMU_I*INTEGRAL_CONSTANT;
    
    //补偿误差
    wx+=ex*IMU_P+exInt;
    wy+=ey*IMU_P+eyInt;
    wz+=ez*IMU_P+ezInt;
    
    //更新四元数
    q0=  q0 + (-q1*wx - q2*wy - q3*wz)*HALF_T;
    q1 = q1 + (q0*wx + q2*wz - q3*wy)*HALF_T;
    q2 = q2 + (q0*wy - q1*wz + q3*wx)*HALF_T;
    q3 = q3 + (q0*wz + q1*wy - q2*wx)*HALF_T; 
    
    //计算欧拉角
    norm=sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
    if(norm<Semig)
        return ;
    q0/=norm;
    q1/=norm;
    q2/=norm;
    q3/=norm;
    
    //欧拉角转换
    angle->yawangle[0]= -atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2+1)* 57.3; 
    angle->yawangle[1]= -asin(-2*q1*q3 + 2*q0*q2)* 57.3;
    //angle->yawangle[2]=atan2(2*q1q2 + 2*q0q3, -2*q2q2 - 2*q3q3+1)* 57.3; 
    
//    if(GY521){
//        if(angle->yawangle[2]>0){
//            angle->yawangle[2]-=180;
//        }else{
//            angle->yawangle[2]+=180;
//        }
//    }
}

