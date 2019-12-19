#include "stdlib.h"
#include "cmd_func.h"
#include "can_utils.h"
#include "can_func.h"
#include "calculate.h"
#include "flash.h"
#include "as5047p.h"
#include "read_data_simple.h"
#include "calculate.h"



//hello
void cmd_hello_func(int argc,char *argv[]){
  uprintf("HELLO!\r\n");
}
//vesion
void cmd_version_func(int argc,char *argv[]){
  uprintf("VESION 1.0 !\r\n");
}
//reset  
void cmd_reset_func(int argc,char *argv[]){
  __set_FAULTMASK(1);   //STM32程序�?件�?�位  
  NVIC_SystemReset(); 
}
void cmd_reset_vega_func(int argc, char *argv[]){
  can_msg vegatest;
  can_send_msg(0x09,&vegatest,8);
  uprintf("vega reset ok");
}
//read
void cmd_read_pos_func(int argc,char *argv[]){
  uprintf("POS:\r\nx---%6fy---%6fangle---%6f\r\n",position.world_x,position.world_y,position.world_yaw); 
}
void cmd_read_circle_func(int argc,char *argv[]){
  float x_circle = wheel_x.now_circlenum + wheel_x.now_circlenum/360;
  float y_circle = wheel_y.now_circlenum + wheel_y.now_circlenum/360;
  uprintf("Circle:\r\nx---%6fy---%6f\r\n",x_circle,y_circle);
}
void cmd_wave_func(int argc,char *argv[]){
  if(atoi(argv[1])==1){
    uprintf("\r\nwave OK\r\n");
    flag.wave =1;
  }
  else{
    uprintf("\r\nwave Stoped\r\n");
    flag.wave =0;
  }  
}

















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
  case 7:flag.test1 = 3;flag.test2 = 0;flag.test3 = 0;uprintf("test7 OK\r\n");break;
  case 8:flag.test2 = 3;flag.test1 = 0;flag.test3 = 0;uprintf("test8 OK\r\n");break;
  case 9:flag.test3 = 3;flag.test1 = 0;flag.test2 = 0;uprintf("test9 OK\r\n");break;
  default :flag.test1 = 0;flag.test2 = 0;flag.test3 = 0;uprintf("test stopped\r\n");
  }
}








//kn
void change_kn(int argc, char *argv[])
{
  k_n = atof(argv[1]);
  uprintf("k_n:%f\r",k_n);
}
//ks
void change_ks(int argc, char *argv[])
{
  k_s = atof(argv[1]);
  uprintf("k_s:%f\r",k_s);
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
// all in one 
void cmd_func_init(void) {
  
    cmd_add("hello", "hello", cmd_hello_func);
    cmd_add("version", "current version", cmd_version_func);
    cmd_add("reset", "reset system", cmd_reset_func);
    cmd_add("reset_vega", "just", cmd_reset_vega_func); 

    cmd_add("readpos", "read 1 to start read position", cmd_read_pos_func);
    cmd_add("readcircle", "read 1 to start read circle", cmd_read_circle_func);
    cmd_add("wave", "show wave", cmd_wave_func);


    cmd_add("default", "get parameters back to default", cmd_default_func);

    cmd_add("center", "modify the center position", cmd_modify_C_func);
    cmd_add("diameter", "modify the wheel diameter", cmd_modify_D_func);
    cmd_add("canid", "modify the CANSEN_ID", cmd_modify_CANID_func);
    cmd_add("setpos", "just", cmd_setxy_func);
    cmd_add("showpra", "just", cmd_show_func);
    cmd_add("test", "just", cmd_test_func);  

    cmd_add("kn", "just", change_kn); 
    cmd_add("ks", "just", change_ks); 
}