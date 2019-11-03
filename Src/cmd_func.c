#include "stdlib.h"
#include "cmd_func.h"
#include "can_utils.h"
#include "can_func.h"
#include "calculate.h"
#include "flash.h"
#include "as5047p.h"

//test
void cmd_test_func(int argc,char *argv[])
{
  int n = atoi(argv[1]);
  switch (n)
  {
  case 1:flag.test1 = 1;flag.test2 = 0;flag.test3 = 0;uprintf("test1 OK\r\n");break;
  case 2:flag.test2 = 1;flag.test1 = 0;flag.test3 = 0;uprintf("test2 OK\r\n");break;
  case 3:flag.test3 = 1;flag.test1 = 0;flag.test2 = 0;uprintf("test3 OK\r\n");break;
  case 4:flag.test1 = 2;flag.test2 = 0;flag.test3 = 0;uprintf("test4 OK\r\n");break;
  case 5:flag.test2 = 2;flag.test1 = 0;flag.test3 = 0;uprintf("test5 OK\r\n");break;
  case 6:flag.test3 = 2;flag.test1 = 0;flag.test2 = 0;uprintf("test6 OK\r\n");break;
  default :flag.test1 = 0;flag.test2 = 0;flag.test3 = 0;uprintf("test stopped\r\n");
  }
}

extern float k_n;
extern float k_s;
void change_kn(int argc, char *argv[])
{
  k_n = atof(argv[1]);
  uprintf("k_n:%f\r",k_n);
}
void change_ks(int argc, char *argv[])
{
  k_s = atof(argv[1]);
  uprintf("k_s:%f\r",k_s);
}

void cmd_resetvega_func(int argc, char *argv[]) {
  can_msg vegatest;
    can_send_msg(0x09,&vegatest,8);
    uprintf("vega reset ok");
}




void cmd_can_test(int argc, char *argv[]) {
    uprintf("\r\ncan send test\r\n");
    can_send_test();
}
 //hello
void cmd_hello_func(int argc,char *argv[]) 
{
  uprintf("\r\nHELLO!\r\n");
}
//vesion
void cmd_version_func(int argc,char *argv[])  
{
  uprintf("\r\nVESION 1.0 !\r\n");
}
//reset  
void cmd_reset_func(int argc,char *argv[])                
{
  __set_FAULTMASK(1);   //STM32程序软件复位  
  NVIC_SystemReset(); 
}
//default
void cmd_default_func(int argc,char *argv[])                 
{
    kx_center = 83;
    ky_center = 325;
    diameter_x = 1.056604;
    diameter_y = 1.037778;
    CANSEND_ID=0x12;
    flash_data[0]=kx_center;
    flash_data[1]=ky_center;
    flash_data[2]=diameter_x;
    flash_data[3]=diameter_y;
    flash_data[5]=(float)CANSEND_ID;
    write_prams();
    uprintf("Center_Distance : kx_center = %f  ky_center = %f    \r\n", kx_center,ky_center);
    uprintf("Wheel_Diameter : X_diameter =%f   y_diameter =%f   \r\n",diameter_x,diameter_y);
    uprintf("CANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}
//readpos
void cmd_readpos_func(int argc,char *argv[])
{
  flag.readpos = atoi(argv[1]);
  if(flag.readpos == 1)
    uprintf("readpos OK\r\n");
  else 
    uprintf("readpos Stoped\r\n");
}
//readcircle
void cmd_readcircle_func(int argc,char *argv[])
{
  flag.readcircle = atoi(argv[1]);
  if(flag.readcircle == 1)
    uprintf("readcircle OK\r\n");
  else 
    uprintf("readcircle Stoped\r\n");
}
//readangle
void cmd_readangle_func(int argc,char *argv[])
{
  flag.readangle = atoi(argv[1]);
  if(flag.readangle == 1)
    uprintf("readangle OK\r\n");
  else 
    uprintf("readangle Stoped\r\n");
}
//wave
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
//center
void cmd_modify_C_func(int argc,char *argv[])
{
  kx_center = atof(argv[1]);
  ky_center = atof(argv[2]);    
  flash_data[0]=kx_center;
  flash_data[1]=ky_center;
  write_prams();
  uprintf("\r\nCenter position: kx_center = %f  ky_center = %f  \r\n",kx_center,ky_center);  
}
//diamter
void cmd_modify_D_func(int argc,char *argv[])
{
  diameter_x = atof(argv[1]);
  diameter_y = atof(argv[2]);    
  flash_data[2]=diameter_x;
  flash_data[3]=diameter_y;
  write_prams();
  uprintf("Wheel_Diameter : X_diameter =%f   y_diameter =%f   \r\n",diameter_x,diameter_y); 
}
//canid
void cmd_modify_CANID_func(int argc,char *argv[])                                    
{
    CANSEND_ID = (uint16_t)atof(argv[1]);
    flash_data[5]=(float)CANSEND_ID;    
    write_prams();
    uprintf("\r\nCANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}
//setpos
void cmd_setxy_func(int argc,char *argv[])                                                         
{
  position.world_x = atof(argv[1]);
  position.world_y = atof(argv[2]);
  position.world_yaw = atof(argv[3]);
  uprintf("\r\nNOW POS : %f     %f      %f   \r\n",position.world_x,position.world_y,position.world_yaw);
}
//showpra
void cmd_show_func(int argc,char *argv[])                                                    
{
  uprintf("\r\nCenter_Distance : kx_center = %f  ky_center = %f    \r\n", kx_center,ky_center);
  uprintf("Wheel_Diameter : %f   %f   \r\n",diameter_x,diameter_y);
  uprintf("CANSEN_ID : 0x%x   \r\n", CANSEND_ID);
}
//    all in one 
void cmd_func_init(void) {
    cmd_add("hello", "hello", cmd_hello_func);
    cmd_add("can_test", "test can", cmd_can_test);
    cmd_add("version", "current version", cmd_version_func);
    cmd_add("reset", "reset system", cmd_reset_func);
    cmd_add("default", "get parameters back to default", cmd_default_func);
    cmd_add("readpos", "read 1 to start read position", cmd_readpos_func);
    cmd_add("readcircle", "read 1 to start read circle", cmd_readcircle_func);
    cmd_add("readangle", "read 1 to start read angle", cmd_readangle_func);
    cmd_add("wave", "show wave", cmd_send_wave_func);
    cmd_add("center", "modify the center position", cmd_modify_C_func);
    cmd_add("diameter", "modify the wheel diameter", cmd_modify_D_func);
    cmd_add("canid", "modify the CANSEN_ID", cmd_modify_CANID_func);
    cmd_add("setpos", "just", cmd_setxy_func);
    cmd_add("showpra", "just", cmd_show_func);
    cmd_add("test", "just", cmd_test_func);  
    cmd_add("vega", "just", cmd_resetvega_func); 
    cmd_add("kn", "just", change_kn); 
    cmd_add("ks", "just", change_ks); 
}