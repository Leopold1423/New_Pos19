#ifndef __calculate_H
#define __calculate_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include "math.h"


#define PI 3.1415926535
typedef struct  Position
{
  float world_x;
  float world_y;
  float world_yaw;
}Position;  

extern Position position;
extern float kx_center,ky_center;
extern float X_diameter,y_diameter;   

float calcul_dx(float thta,float d_thta,float X1,float X2);
float calcul_dy(float thta,float d_thta,float X1,float X2);
float angle_toshow(float angle);
void calcul_XY();


#ifdef __cplusplus
}
#endif
#endif /*__ calculate_H */
