#include <string.h>
#include "asm330lhh_reg.h"
#include "stm32f4xx_hal.h"
#include "read_data_simple.h"
#include "calculate.h"
#include "spi.h"
#include "cmd.h"
#include "tim.h"
#include "angle.h"
#include "karman.h"
#include "math.h"
#include "easy_angle.h"


asm330lhh_ctx_t dev_ctx;
Angle angle;
//基础spi通信
static int32_t platform_write(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
  if (handle == &hspi2)
  {
    HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, &Reg, 1, 1000);
    HAL_SPI_Transmit(handle, Bufp, len, 1000);
    HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, GPIO_PIN_SET);
  }
  return 0;
}
static int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
   if (handle == &hspi2)
  {
    Reg |=0x80;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_SPI_Transmit(handle, (uint8_t*)&Reg, 1, 1000);
    HAL_SPI_Receive(handle, Bufp, len, 1000);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  }
  return 0;
}
//陀螺仪硬件初始化 
void asm330lhh_device_init(void)
{
  uint8_t whoamI, rst;
   /*
   *  Initialize mems driver interface.
   */
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = &hspi2;

  /*
   * Init test platform.
   */

  asm330lhh_reg_t hh;
  asm330lhh_status_reg_get(&dev_ctx, &hh.status_reg);
  /*
   *  Check device ID.
   */
  asm330lhh_device_id_get(&dev_ctx, &whoamI);
  if (whoamI != ASM330LHH_ID)
    while(1)
    {
      asm330lhh_device_id_get(&dev_ctx, &whoamI);
    };

  /*
   *  Restore default configuration.
   */
  asm330lhh_reset_set(&dev_ctx, PROPERTY_ENABLE);
  do {
    asm330lhh_reset_get(&dev_ctx, &rst);
  } while (rst);


  /*
   *  Enable Block Data Update.
   */
  asm330lhh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

  /*
   * Set Output Data Rate.
   */
//  asm330lhh_xl_data_rate_set(&dev_ctx, ASM330LHH_XL_ODR_12Hz5);
//  asm330lhh_gy_data_rate_set(&dev_ctx, ASM330LHH_GY_ODR_12Hz5);
  asm330lhh_xl_data_rate_set(&dev_ctx, ASM330LHH_XL_ODR_208Hz);
  asm330lhh_gy_data_rate_set(&dev_ctx, ASM330LHH_GY_ODR_208Hz);
  
  /*
   * Set full scale.
   */
  asm330lhh_xl_full_scale_set(&dev_ctx, ASM330LHH_4g);
  asm330lhh_gy_full_scale_set(&dev_ctx, ASM330LHH_2000dps);

  
  /*
   * Configure filtering chain(No aux interface).
   */
  /*
   * Accelerometer - LPF1 + LPF2 path.
   */
  asm330lhh_xl_hp_path_on_out_set(&dev_ctx, ASM330LHH_LP_ODR_DIV_100);
  asm330lhh_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);  
  //asm330lhh_gy_lp1_bandwidth_set(&dev_ctx,ASM330LHH_XTREME);
}
//陀螺仪数据读取
void asm330lhh_run(void)
{
  static axis3bit16_t data_raw_acceleration;
  static axis3bit16_t data_raw_angular_rate;  
  /*
   * Read samples in polling mode (no int).
   */
    asm330lhh_reg_t reg;
    asm330lhh_status_reg_get(&dev_ctx, &reg.status_reg);

    if (reg.status_reg.xlda)
    {
      /*
       * Read acceleration field data.
       */
      memset(data_raw_acceleration.u8bit, 0x00, 3 * sizeof(int16_t));
      asm330lhh_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);
      angle.acceleration_g[0] =
    		  asm330lhh_from_fs4g_to_mg(data_raw_acceleration.i16bit[0])/1000;
      angle.acceleration_g[1] =
    		  asm330lhh_from_fs4g_to_mg(data_raw_acceleration.i16bit[1])/1000;
      angle.acceleration_g[2] =
    		  asm330lhh_from_fs4g_to_mg(data_raw_acceleration.i16bit[2])/1000;
    }
   if (reg.status_reg.gda)
   {
     /*
      * Read angular rate field data.
      */
     memset(data_raw_angular_rate.u8bit, 0x00, 3 * sizeof(int16_t));
     asm330lhh_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
     angle.angular_rate_dps[0] =
    		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0])/1000;
     angle.angular_rate_dps[1] =
    		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1])/1000;
     angle.angular_rate_dps[2] =
   		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2])/1000;
   }    

}
//陀螺仪计算零漂
void asm330lhh_init()
{
  asm330lhh_device_init();

  for(int i=0;i<100;i++)              //隐形bug 采样500次时 0漂值错得离谱
  {
    asm330lhh_run();
    angle.zero_angular_rate_dps[0]+=angle.angular_rate_dps[0];
    angle.zero_angular_rate_dps[1]+=angle.angular_rate_dps[1];
    angle.zero_angular_rate_dps[2]+=angle.angular_rate_dps[2];
    angle.zero_acceleration_g[0]+=angle.acceleration_g[0];
    angle.zero_acceleration_g[1]+=angle.acceleration_g[1];
    angle.zero_acceleration_g[2]+=angle.acceleration_g[2];
  }
  Delay(60000);

  angle.zero_angular_rate_dps[0]/=100;
  angle.zero_angular_rate_dps[1]/=100;
  angle.zero_angular_rate_dps[2]/=100;
  angle.zero_acceleration_g[0]/=100;
  angle.zero_acceleration_g[1]/=100;
  angle.zero_acceleration_g[2]/=100;
  for(int c=0;c<6;c++)             //bug 不延时会有bug
  {
    Delay(60000);
  }
}
//角速度积分计算角度
float delta_time=0.005;
Angle pre_angle;
void Get_Yaw_angle()
{
  asm330lhh_run();
  //对pre_angle赋值  
  pre_angle.acceleration_g[0]= angle.acceleration_g[0];
  pre_angle.acceleration_g[1]= angle.acceleration_g[1];
  pre_angle.acceleration_g[2]= angle.acceleration_g[2];
  pre_angle.angular_rate_dps[0]= angle.angular_rate_dps[0];
  pre_angle.angular_rate_dps[1]= angle.angular_rate_dps[1];
  pre_angle.angular_rate_dps[2]= angle.angular_rate_dps[2];
  // 六轴的零漂
  pre_angle.zero_angular_rate_dps[0] = 286.9881557/1000;
  pre_angle.zero_angular_rate_dps[1] = -383.649635/1000;
  pre_angle.zero_angular_rate_dps[2] = -201.3589212/1000;
  pre_angle.zero_acceleration_g[0] = -35.5791181/1000;
  pre_angle.zero_acceleration_g[1] = -25.10951367/1000;
  pre_angle.zero_acceleration_g[2] = 0;     //1006.11278/1000;
  //卡尔曼与四元数
  Slide(&pre_angle);
  
//  float test[3];
//  get_angle(pre_angle.acceleration_g,pre_angle.angular_rate_dps,angle.yawangle,test);
  
  
  IMU_Update(&angle,pre_angle.acceleration_g[0],pre_angle.acceleration_g[1],pre_angle.acceleration_g[2],
             pre_angle.angular_rate_dps[0]*PI/180,pre_angle.angular_rate_dps[1]*PI/180,pre_angle.angular_rate_dps[2]*PI/180);
  //解算系数
//  float k1 = -sin(angle.yawangle[1]*PI/180)*cos(angle.yawangle[0]*PI/180);
//  float k2 = sin(angle.yawangle[0]*PI/180);
//  float k3 = cos(angle.yawangle[0]*PI/180)*cos(angle.yawangle[1]*PI/180);
  
  float k1 = -sin(angle.yawangle[1]*PI/180);
  float k2 = sin(angle.yawangle[0]*PI/180)*cos(angle.yawangle[1]*PI/180);
  float k3 = cos(angle.yawangle[0]*PI/180)*cos(angle.yawangle[1]*PI/180);
  
  angle.angular_rate[2] = angle.angular_rate_dps[0] * k1 + angle.angular_rate_dps[1] * k2 + angle.angular_rate_dps[2] * k3;  
  //去零漂
  
 // uprintf("%f\r",angle.angular_rate[2]);
  
  
  if(angle.angular_rate[2]>0.05)  
  {
    float delta_angle = delta_time * (angle.angular_rate[2]+0.35);
    angle.delta_yawangle[2] = delta_angle;
    angle.yawangle[2] += delta_angle;     

  }
 
  if(angle.angular_rate[2]<-0.4)  
  {
    float delta_angle = delta_time * (angle.angular_rate[2]+0.35);
    angle.delta_yawangle[2] = delta_angle;
    angle.yawangle[2] += delta_angle;     

  }
  
}

void Get_Yaw_angle_0()
{
    
  asm330lhh_run();
  if(angle.angular_rate_dps[2]>0.1||angle.angular_rate_dps[2]<-0.35)
  {
    float delta_angle=delta_time * (angle.angular_rate_dps[2]+0.238311837 );    //固定零票
//    float delta_angle=delta_time * (angular_rate_dps[2]-zero_angular_rate[2])/1000;
//    float delta_angle=delta_time * (angular_rate_dps[2])/1000;//不加零漂
    angle.delta_yawangle[2]=delta_angle;
    angle.yawangle[2]+=delta_angle; 
  }
}


