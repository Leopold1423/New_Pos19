#ifndef __as5047p_H
#define __as5047p_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "stdint.h" 

#define AS5047PMAXPOS 16384      //0x3fff
   
typedef struct Wheel
{
  float zero_position;   //�ϵ�λ��
  float now_position;    //��ǰλ��
  float last_position;   //�ϴ�λ��
  float now_speed;  
  float now_angle;
  float last_angle;
  float circlesum;        //��Ȧ��¼
  int now_circlenum;
  int last_circlenum;
  float delta_distance;       //wheel�ƶ�����
  float full_distance;       //wheel�ƶ�����
}Wheel;  

uint16_t as5047p_Get_Position_x();
uint16_t as5047p_Get_Position_y();
void Get_Basic_x();
void Get_Basic_y();

//�ⲿ����
void wheel_init();
void Get_Wheel_x();
void Get_Wheel_y();
void Show_Wheel_x();
void Show_Wheel_y();
//�ⲿ����
extern Wheel wheel_x;
extern Wheel wheel_y;
extern float diameter_x;
extern float diameter_y;
   
#ifdef __cplusplus
}
#endif
#endif /*__ as5047p_H */