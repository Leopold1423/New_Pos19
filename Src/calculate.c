#include "calculate.h"
#include "as5047p.h"
#include "read_data_simple.h"

Position position={0};
float kx_center = 83,ky_center = 325;


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
  float dx = calcul_dx(angle.yawangle[2]*PI/180,(angle.delta_yawangle[2])*PI/180,wheel_x.delta_distance,wheel_y.delta_distance);                                
  float dy = calcul_dy(angle.yawangle[2]*PI/180,(angle.delta_yawangle[2])*PI/180,wheel_x.delta_distance,wheel_y.delta_distance);
 
  position.world_x += dx;
  position.world_y += dy;  
  position.world_yaw =angle_toshow(-angle.yawangle[2]);                                                                  
}

float angle_toshow(float angle)
{
  float showangle=0;
  showangle=angle;
  while(showangle>=180||showangle<-180)
  {
    if(showangle>180)showangle-=360;
    if(showangle<=-180)showangle+=360;
  }
  return showangle;
}
