#include "stdlib.h"
#include "cmd_func.h"
#include "can_utils.h"
#include "can_func.h"
#include "calculate.h"
#include "flash.h"
#include "as5047p.h"
#include "read_data_simple.h"
#include "calculate.h"

int test_flag_value[5] ={0};
int test_int_value[5]={0};
float test_float_value[5]={0};

//hello
void cmd_hello_func(int argc,char *argv[]){
  uprintf("Hello!\r\n");
}
//reset  
void cmd_reset_func(int argc,char *argv[]){
  __set_FAULTMASK(1);   //STM32�?件�?�位
  NVIC_SystemReset(); 
}
//vesion
void cmd_version_func(int argc,char *argv[]){
  uprintf("Version 1.0 !\r\n");
}
void cmd_reset_vega_func(int argc, char *argv[]){
  can_msg vegatest;
  can_send_msg(0x10,&vegatest,8);
  uprintf("vega reset ok");
}
//default
void cmd_default_func(int argc,char *argv[]){
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
//canid
void cmd_modify_CANID_func(int argc,char *argv[]){
  CANSEND_ID = (uint16_t)atof(argv[1]);  
  uprintf("Cansend_ID : %d\r\n", CANSEND_ID);
}


//read
void cmd_read_pos_func(int argc,char *argv[]){
  uprintf("POS:\r\nx:%6fy:%6fangle:%6f\r\n",position.world_x,position.world_y,position.world_yaw); 
}
void cmd_read_vega_func(int argc,char *argv[]){
  vega_print_pos();
}
void cmd_pos_func(int argc,char *argv[]){
  uprintf("POS:\r\nx:%6fy:%6fangle:%6f\r\n",position.world_x,position.world_y,position.world_yaw); 
  vega_print_pos();
}

void cmd_read_circle_func(int argc,char *argv[]){
  float x_circle = wheel_x.now_circlenum + wheel_x.circlesum/360;
  float y_circle = wheel_y.now_circlenum + wheel_y.circlesum/360;
  uprintf("Circle:\r\nx:%6fy:%6f\r\n",x_circle,y_circle);
}



void cmd_xypart_func(int argc,char *argv[]){

  uprintf("xpart:\r\nx:%f y:%f\r\n",partx[0],partx[1]);
  uprintf("ypart:\r\nx:%f y:%f\r\n",party[0],party[1]);
}


//test
void cmd_test_int_func(int argc,char *argv[]){
  test_int_value[0]=atoi(argv[1]);
  test_int_value[1]=atoi(argv[2]);
  // test_int_value[2]=atoi(argv[3]);
  // test_int_value[3]=atoi(argv[4]);
  // test_int_value[4]=atoi(argv[5]);
  uprintf("%4d %4d\r\n",atoi(argv[1]),atoi(argv[2]));
}
void cmd_test_float_func(int argc,char *argv[]){
  test_float_value[0]=atof(argv[1]);
  test_float_value[1]=atof(argv[2]);
  // test_float_value[2]=atof(argv[3]);
  // test_float_value[3]=atof(argv[4]);
  // test_float_value[4]=atof(argv[5]);
  uprintf("%4f %4f\r\n",atof(argv[1]),atof(argv[2]));
}
void cmd_test_flag_func(int argc,char *argv[]){
  int n = atoi(argv[1]);
  switch (n){  
  case 1:test_flag_value[0] = 1;test_flag_value[1] = 0;test_flag_value[3] = 0;uprintf("%6f\r\n",angle.yawangle[2]);break;//uprintf("test1 OK\r\n");break;
  case 2:test_flag_value[1] = 1;test_flag_value[0] = 0;test_flag_value[3] = 0;uprintf("test2 OK\r\n");break;
  case 3:test_flag_value[3] = 1;test_flag_value[0] = 0;test_flag_value[1] = 0;uprintf("test3 OK\r\n");break;
  case 4:test_flag_value[0] = 2;test_flag_value[1] = 0;test_flag_value[3] = 0;uprintf("test4 OK\r\n");break;
  case 5:test_flag_value[1] = 2;test_flag_value[0] = 0;test_flag_value[3] = 0;uprintf("test5 OK\r\n");break;
  case 6:test_flag_value[3] = 2;test_flag_value[0] = 0;test_flag_value[1] = 0;uprintf("test6 OK\r\n");break;
  case 7:test_flag_value[0] = 3;test_flag_value[1] = 0;test_flag_value[3] = 0;uprintf("test7 OK\r\n");break;
  case 8:test_flag_value[1] = 3;test_flag_value[0] = 0;test_flag_value[3] = 0;uprintf("test8 OK\r\n");break;
  case 9:test_flag_value[3] = 3;test_flag_value[0] = 0;test_flag_value[1] = 0;uprintf("test9 OK\r\n");break;
  default:test_flag_value[0] = 0;test_flag_value[1] = 0;test_flag_value[3] = 0;uprintf("test stopped\r\n");

  }
}

//陀螺仪逆时针旋转kn
void change_kn(int argc, char *argv[]){
  k_n = atof(argv[1]);
  uprintf("k_n:%f\r\n",k_n);
}
//陀螺仪逆时顺旋转ks
void change_ks(int argc, char *argv[]){
  k_s = atof(argv[1]);
  uprintf("k_s:%f\r\n",k_s);
}
//两轮到定位中心垂直距离center
void cmd_modify_C_func(int argc,char *argv[])
{
  kx_center = atof(argv[1]);
  ky_center = atof(argv[2]);    
  uprintf("Center position:\r\nkx_center = %6f ky_center = %6f\r\n",kx_center,ky_center);  
}
//两轮直径diamter
void cmd_modify_D_func(int argc,char *argv[])
{
  diameter_x = atof(argv[1]);
  diameter_y = atof(argv[2]);    
  uprintf("Wheel_Diameter:\r\nX_diameter =%6f y_diameter =%6f\r\n",diameter_x,diameter_y); 
}
//setpos
void cmd_setpos_func(int argc,char *argv[]){
  position.world_x = atof(argv[1]);
  position.world_y = atof(argv[2]);
  angle.yawangle[2] = atof(argv[3]);
  position.world_yaw =angle_toshow(angle.yawangle[2]); 
  uprintf("POS:\r\nx:%6fy:%6fangle:%6f\r\n",position.world_x,position.world_y,position.world_yaw); 
}
// all in one 
void cmd_func_init(void) {
  cmd_add("hello", "hello", cmd_hello_func);
  cmd_add("version", "current version", cmd_version_func);
  cmd_add("reset", "", cmd_reset_func);
  cmd_add("default", "", cmd_default_func);
  cmd_add("canid", "", cmd_modify_CANID_func);

  cmd_add("readpos", "", cmd_read_pos_func);
  cmd_add("readvega", "", cmd_read_vega_func);
  cmd_add("pos", "", cmd_pos_func);
  cmd_add("readcircle", "", cmd_read_circle_func);  

  cmd_add("setpos", "just", cmd_setpos_func); 
  cmd_add("kn", "just", change_kn); 
  cmd_add("ks", "just", change_ks);
  cmd_add("center", "modify the center position", cmd_modify_C_func);
  cmd_add("diameter", "modify the wheel diameter", cmd_modify_D_func); 
  cmd_add("test_float", "", cmd_test_float_func); 
  cmd_add("test_int", "", cmd_test_int_func); 
  cmd_add("test_flag", "", cmd_test_flag_func); 
  cmd_add("part", "", cmd_xypart_func); 
}