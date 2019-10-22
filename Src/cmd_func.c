#include "cmd_func.h"
#include "can_utils.h"
#include "calculate.h"
#include "flash.h"
#include "encoder.h"
#include "stdlib.h"
#include "can_func.h"

void cmd_can_test(int argc, char *argv[]) {
    uprintf("\r\ncan send test\r\n");
    can_send_test();
}


void cmd_hello_func(int argc,char *argv[])  //hello
{
  uprintf("\r\nHELLO!\r\n");
}

void cmd_version_func(int argc,char *argv[])  //vesion
{
  uprintf("\r\nVESION 1.0 !\r\n");
}

void cmd_reset_func(int argc,char *argv[])  //vesion                 
{
  __set_FAULTMASK(1);   //STM32程序软件复位  
  NVIC_SystemReset(); 
}

void cmd_default_func(int argc,char *argv[])                 
{
    kx_center = 83;
    ky_center = 325;
    X_diameter = 1.056604;
    y_diameter = 1.037778;
    time=5;
    CANSEND_ID=0x12;
    encoder.encoderX_dir=1;
    encoder.encoderY_dir=1;
    flash_data[0]=kx_center;
    flash_data[1]=ky_center;
    flash_data[2]=X_diameter;
    flash_data[3]=y_diameter;
    flash_data[4]=(float)time;
    flash_data[5]=(float)CANSEND_ID;
    flash_data[6]=(float)encoder.encoderX_dir;
    flash_data[7]=(float)encoder.encoderY_dir;
    write_prams();
    uprintf("Center_Distance : kx_center = %f  ky_center = %f    \r\n", kx_center,ky_center);
    uprintf("Wheel_Diameter : X_diameter =%f   y_diameter =%f   \r\n",X_diameter,y_diameter);
    uprintf("Sampling_Time : %d   \r\n", time);
    uprintf("CANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}

void cmd_read_func(int argc,char *argv[])
{
  flag.read = atoi(argv[1]);
  if(flag.read == 1)
    uprintf("read OK\r\n");
  else 
    uprintf("read Stoped\r\n");
}

void cmd_read_encoder_func(int argc,char *argv[])
{
  if(atoi(argv[1])==1)
  {
    uprintf("encoder OK\r\n");
    flag.encoder =1;
  }
  else
  {
    uprintf("encoder Stoped\r\n");
    flag.encoder =0;
  }
}

void cmd_send_wave_func(int argc,char *argv[])    
{
  if(atoi(argv[1])==1)
  {
    uprintf("\r\nwave OK\r\n");
    flag.wave =1;
  }
  else
  {
    uprintf("\r\nwave Stoped\r\n");
    flag.wave =0;
  }  
}
/*       PARAM
*       功能：修改定位参考点距定位模块两全向伦垂直距离
*       参数：modify_L kx_center ky_center 
*/
void cmd_modify_C_func(int argc,char *argv[])
{
  kx_center = atof(argv[1]);
  ky_center = atof(argv[2]);    
  flash_data[0]=kx_center;
  flash_data[1]=ky_center;
  write_prams();
  uprintf("\r\nCenter position: kx_center = %f  ky_center = %f  \r\n",kx_center,ky_center);  
}
/*       PARAM
*       功能：修改两轮直径尺寸
*       参数：modify_D X_diameter y_diameter 
*/
void cmd_modify_D_func(int argc,char *argv[])
{
  X_diameter = atof(argv[1]);
  y_diameter = atof(argv[2]);    
  flash_data[2]=X_diameter;
  flash_data[3]=y_diameter;
  write_prams();
  uprintf("\r\nWheel diameter: X_diameter = %f  y_diameter = %f  \r\n",X_diameter,y_diameter);
}

void cmd_modify_CANID_func(int argc,char *argv[])                                    
{
    CANSEND_ID = (uint16_t)atof(argv[1]);
    flash_data[5]=(float)CANSEND_ID;    
    write_prams();
    uprintf("\r\nCANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}
/*       PARAM
*       功能：修改采样时间
*       参数：time value
*/
void cmd_change_time_func(int argc,char *argv[]) 
{
  time=atoi(argv[1]);  
  flash_data[4]=(float)time;
  write_prams();
  uprintf("\r\nSampling time：%d\r\n",time);
}
/*       SET
*       功能：set xy
*       参数：setxy x y
*/
void cmd_setxy_func(int argc,char *argv[])                                                         
{
  triangle.x= atof(argv[1]);
  triangle.y = atof(argv[2]);
  triangle.angle = atof(argv[3]);
  triangle.showangle=angle_toshow(triangle.angle);
  uprintf("\r\nNow:x = %f  y = %f angle=%f \r\n", triangle.x, triangle.y,triangle.showangle);
}
/*    CHECK
*       功能：检查移动是否到位
*       参数：check [1/2/3] value (角度范围为【-180，180】)
*/
void cmd_check_func(int argc,char *argv[])//
{
  float target = atof(argv[2]);
  uprintf("start check data\r\n");
  
  if(atoi(argv[1])==1)
  {
    if(triangle.x-target<=2&&triangle.x-target>=-2)
      uprintf("\r\nX vaule is ok!\r\n");
    else
      uprintf("\r\nX error :%f\r\n",triangle.x-target);
  }
  else if(atoi(argv[1])==2)
  {
    if(triangle.y-target<=2&&triangle.y-target>=-2)
      uprintf("\r\nY vaule is ok!\r\n");
    else
      uprintf("\r\nY error :%f\r\n",triangle.y-target);
  }
  else if(atoi(argv[1])==3)
  {
    if(triangle.showangle-target<=0.02&&triangle.showangle-target>=-0.02)
      uprintf("\r\nAngle vaule is ok!\r\n");
    else
      uprintf("\r\nAngle error %f\r\n",triangle.showangle-target);
  }
  else 
    uprintf("\r\nplease input check 1/2/3 data\r\n");
} 
/*      SHOW
*       功能：输出个param
*       参数：check [1/2/3] value (角度范围为【-180，180】)
*/
void cmd_show_func(int argc,char *argv[])                                                    
{
  uprintf("\r\nCenter_Distance : kx_center = %f  ky_center = %f    \r\n", kx_center,ky_center);
  uprintf("Wheel_Diameter : %f   %f   \r\n",X_diameter,y_diameter);
  uprintf("Sampling_Time : %d   \r\n", time);
  uprintf("CANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}

void cmd_encoderXdir_func(int argc,char *argv[])
{
  HAL_Delay(1500);
  int dir=0;
  float cnt=0;
  for(int i=0;i<20;i++)
  {
  TIM4->CNT=0;
  HAL_Delay(50);
  if(TIM4->CNT >= 30000)
    cnt = (float)TIM4->CNT-65536;
  else
    cnt = (float)TIM4->CNT; 
  if(cnt>0)
    dir +=1;
  if(cnt<0)
    dir -=1;
  } 
  if(dir>=0) 
  encoder.encoderX_dir = 1;
  else 
  encoder.encoderX_dir = -1;  
  flash_data[6]=(float)encoder.encoderX_dir;  
  write_prams();
}

void cmd_encoderYdir_func(int argc,char *argv[])
{
  HAL_Delay(1500);
  int dir=0;
  float cnt=0;
  for(int i=0;i<20;i++)
  {
  TIM4->CNT=0;
  HAL_Delay(50);
  if(TIM4->CNT >= 30000)
    cnt = (float)TIM4->CNT-65536;
  else
    cnt = (float)TIM4->CNT; 
  if(cnt>0)
    dir +=1;
  if(cnt<0)
    dir -=1;
  } 
  if(dir>=0) 
  encoder.encoderY_dir = 1;
  else 
  encoder.encoderY_dir = -1;  
  flash_data[7]=(float)encoder.encoderY_dir;
  write_prams();
}







void cmd_func_init(void) {
    cmd_add("hello", "hello", cmd_hello_func);
    cmd_add("can_test", "test can", cmd_can_test);
    cmd_add("version", "current version", cmd_version_func);
    cmd_add("reset", "reset system", cmd_reset_func);
    cmd_add("default", "get parameters back to default", cmd_default_func);
    cmd_add("read", "read 1 to start read position", cmd_read_func);
    cmd_add("encoder", "encoder 1 to start read encoder ", cmd_read_encoder_func);
    cmd_add("wave", "show wave", cmd_send_wave_func);
    cmd_add("modify_C", "modify the center position", cmd_modify_C_func);
    cmd_add("modify_D", "modify the wheel diameter", cmd_modify_D_func);
    cmd_add("modify_ID", "modify the CANSEN_ID", cmd_modify_CANID_func);
    cmd_add("modify_time", "modify the sampling time", cmd_change_time_func);
    cmd_add("setpos", "just", cmd_setxy_func);
    cmd_add("checkpos", "test can", cmd_check_func);
    cmd_add("showpra", "just", cmd_show_func);
    cmd_add("encoderXdir", "test can", cmd_encoderXdir_func);
    cmd_add("encoderYdir", "just", cmd_encoderYdir_func);
}