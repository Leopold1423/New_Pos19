#include "can_func.h"
#include "cmd.h"
#include "calculate.h"
#include "read_data_simple.h"

void can_suc_rx(can_msg *data) {
  uprintf("can rx ok\r\n");
}


/**************************接收函数********************/
float vega[3];
#define VEGA_ANGLE_ID 11
#define VEGA_XY_ID    22
void can_vega_angle(can_msg *data){
  vega[2] = data->fl[0]; 
}
void can_vega_xy(can_msg *data){
  vega[0] = data->fl[0]*1000; 
  vega[1] = data->fl[1]*1000; 
}
void vega_print_pos(){
  uprintf("VEGA:\r\nx:%6fy:%6fangle:%6f\r\n",vega[0],vega[1],vega[2]);
}



void can_func_init() {
  can_callback_add(1, can_suc_rx);
  can_callback_add(VEGA_ANGLE_ID,can_vega_angle);
  can_callback_add(VEGA_XY_ID,can_vega_xy);
}


/**************************发送函数********************/
uint16_t CANSEND_ID=13;
float n=0;
int send_XY(float x,float y){
  can_msg data;
  data.fl[0]=x;
  data.fl[1]=y;
  return can_send_msg(CANSEND_ID,&data,8);
}

int send_Angle(float angle){    
  can_msg data;
  data.fl[0]=angle;
  return can_send_msg(CANSEND_ID,&data,4);
}
