#ifndef __read_data_simple_H
#define __read_data_simple_H
#ifdef __cplusplus
 extern "C" {
#endif

#define CS_SPI2_GPIO_Port   GPIOC
#define CS_SPI2_Pin         GPIO_PIN_13

typedef struct Angle
{
  float acceleration_g[3];
  float angular_rate_dps[3];
  float zero_acceleration_g[3];
  float zero_angular_rate_dps[3];
  float angular_rate[3];
  float delta_yawangle[3];
  float yawangle[3];
}Angle;
extern Angle angle;
extern Angle pre_angle;

void asm330lhh_device_init();
void asm330lhh_init();
void asm330lhh_run(void);
void Get_Yaw_angle();
void Get_Yaw_angle_0();
#ifdef __cplusplus
}
#endif
#endif