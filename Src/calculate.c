#include "calculate.h"
#include "encoder.h"
#include "main.h"
#include "as5047p.h"

typedef struct  Position
{
  int cnt_x;
  int cnt_y;
  float angle_x;
  float angle_y;
}Position;  
















/****************************parameter table***************************/

//LX,LY:ȫ�������ľ�ο��㴹ֱ����

//kx_center,ky_center:LX��LY������

//X_diameter,y_diameter:ȫ����ֱ����������

//X,Y,thta,dthta:XYΪ�������ƶ����룬thta��ǰƫ����

//dx,dy:��������ϵ��λ��

/***********************************************************************/
float kx_center = 83,ky_center = 325;
float X_diameter = 1.056604,y_diameter = 1.037778;
float asm_offset_cnt = 0;


float calcul_dx(float thta,float d_thta,float X,float Y)
{
  
  float Ly=ky_center;
  float Lx=kx_center;
  float s,c;
  s=sin(thta);
  c=cos(thta);    
  float dx =  -Y*s + X*c + Ly*s*d_thta - Lx*c*d_thta;
  return dx;
}

float calcul_dy(float thta,float d_thta,float X,float Y)
{
  
  float Ly=ky_center;
  float Lx=kx_center;
  float s,c;
  s=sin(thta);
  c=cos(thta);    
  float dy =  Y*c + X*s - Ly*c*d_thta - Lx*s*d_thta;      
  return dy;
}


void calcul_XY()
{

  //***********************��������������λ��*****************************************//
  if(TIM4->CNT >= 30000)
    encoder.X = (float)TIM4->CNT-65536;
  else
    encoder.X = (float)TIM4->CNT;
  
  if(TIM3->CNT >= 30000)
    encoder.Y = (float)TIM3->CNT-65536;
  else
    encoder.Y = (float)TIM3->CNT;
  
  TIM3->CNT=0;
  TIM4->CNT=0;
  
  float x = encoder.encoderX_dir*encoder.X*50.5*PI/2048;                //2048�ߣ��ܳ�50.5*PI
  float y = encoder.encoderY_dir*encoder.Y*50.5*PI/2048;
  
  x *= X_diameter;                                //��ֱ��������
  y *= y_diameter; 

  //***********************����x y th *****************************************//
  //float dth = calcul_dth();
  float dx = calcul_dx(triangle.angle*PI/180,(triangle.angle-triangle.lastangle)*PI/180,x,y);                                    //  dth��
  float dy = calcul_dy(triangle.angle*PI/180,(triangle.angle-triangle.lastangle)*PI/180,x,y);

  
  triangle.x += dx;
  triangle.y += dy;  
  //triangle.angle += dth;                                                                            //todoΪ��pcb��������


  triangle.lastangle=triangle.angle;                    //���ڼ���dthta                              //!��ɫ��עΪ��pcbɾ������
  triangle.angle=-inputangle;                           //�ɵ������� ˳ʱ��Ϊ����ȡ��ʱ��Ϊ��           //!
  

  
  triangle.showangle=angle_toshow(-inputangle);                             //����Ƕȷ�ΰ��-180.180��              //!
}

float angle_toshow(float angle)
{
  float showangle=0;
  showangle=angle;
  while(showangle>180||showangle<-180)
  {
    if(showangle>180)showangle-=360;
    if(showangle<=-180)showangle+=360;
  }
  return showangle;
}
