#ifndef __read_data_simple_H
#define __read_data_simple_H
#ifdef __cplusplus
 extern "C" {
#endif

#define CS_SPI2_GPIO_Port   GPIOC
#define CS_SPI2_Pin         GPIO_PIN_13


extern float acceleration_mg[3];
extern float angular_rate_mdps[3];
extern float zero_angular_rate[3];
extern float zero_acceleration[3];
extern float yaw_angle;

void asm330lhh_init();
void asm330lhh_run(void);
void Get_Yaw_angle();
void Show_anglerate();
void Show_accel();
#ifdef __cplusplus
}
#endif
#endif