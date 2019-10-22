#include <string.h>
#include "asm330lhh_reg.h"
#include "stm32f4xx_hal.h"
#include "read_data_simple.h"
#include "spi.h"
#include "cmd.h"
#include "tim.h"

float acceleration_mg[3]={0,0,0};
float angular_rate_mdps[3]={0,0,0};
float zero_angular_rate[3]={0,0,0};
float zero_acceleration[3]={0,0,0};
asm330lhh_ctx_t dev_ctx;
//基础spi通信
static int32_t platform_write(void *handle, uint8_t Reg, uint8_t *Bufp,
                              uint16_t len)
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
static int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp,
                             uint16_t len)
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
/*
 * Set Acc/Gyro ODR to 12.5 Hz
 * Set full scale to 2g (XL) 2000dps (Gyro)
 * Select LPF2 bandwidth to ODR/100
 * Enable digital data path to LPF2
 */
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
  asm330lhh_xl_full_scale_set(&dev_ctx, ASM330LHH_2g);
  asm330lhh_gy_full_scale_set(&dev_ctx, ASM330LHH_2000dps);

  
  /*
   * Configure filtering chain(No aux interface).
   */
  /*
   * Accelerometer - LPF1 + LPF2 path.
   */
  asm330lhh_xl_hp_path_on_out_set(&dev_ctx, ASM330LHH_LP_ODR_DIV_100);
  asm330lhh_xl_filter_lp2_set(&dev_ctx, PROPERTY_ENABLE);
  
  asm330lhh_gy_lp1_bandwidth_set(&dev_ctx,ASM330LHH_XTREME);
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
      acceleration_mg[0] =
    		  asm330lhh_from_fs2g_to_mg(data_raw_acceleration.i16bit[0]);
      acceleration_mg[1] =
    		  asm330lhh_from_fs2g_to_mg(data_raw_acceleration.i16bit[1]);
      acceleration_mg[2] =
    		  asm330lhh_from_fs2g_to_mg(data_raw_acceleration.i16bit[2]);
    }
   if (reg.status_reg.gda)
   {
     /*
      * Read angular rate field data.
      */
     memset(data_raw_angular_rate.u8bit, 0x00, 3 * sizeof(int16_t));
     asm330lhh_angular_rate_raw_get(&dev_ctx, data_raw_angular_rate.u8bit);
     angular_rate_mdps[0] =
    		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[0]);
     angular_rate_mdps[1] =
    		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[1]);
     angular_rate_mdps[2] =
   		  asm330lhh_from_fs2000dps_to_mdps(data_raw_angular_rate.i16bit[2]);
   }    

}
//陀螺仪计算零漂
void asm330lhh_init()
{
  asm330lhh_device_init();

  for(int i=0;i<500;i++)              //隐形bug 采样500次时 0漂值错得离谱
  {
    asm330lhh_run();
    zero_angular_rate[0]+=angular_rate_mdps[0];
    zero_angular_rate[1]+=angular_rate_mdps[1];
    zero_angular_rate[2]+=angular_rate_mdps[2];
    zero_acceleration[0]+=acceleration_mg[0];
    zero_acceleration[1]+=acceleration_mg[1];
    zero_acceleration[2]+=acceleration_mg[2];
  }
  zero_angular_rate[0]/=100;
  zero_angular_rate[1]/=100;
  zero_angular_rate[2]/=100;
  zero_acceleration[0]/=100;
  zero_acceleration[1]/=100;
  zero_acceleration[2]/=100;
}
//角速度积分计算角度
float yaw_angle =0;
float delta_time=0.005;
void Get_Yaw_angle()
{
  float delta_angle=0;
  asm330lhh_run();
  if(angular_rate_mdps[2]>0||angular_rate_mdps[2]<-350)
  {
    static int ch=0;
    ch++;
  delta_angle=delta_time * (angular_rate_mdps[2]+210)/1000;//固定零票
  //delta_angle=delta_time * (angular_rate_mdps[2]-zero_angular_rate[2])/1000;
 // delta_angle=delta_time * (angular_rate_mdps[2])/1000;//不加零漂
  yaw_angle+=delta_angle; 
  }
}


