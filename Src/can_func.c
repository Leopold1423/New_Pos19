#include "can_func.h"
#include "cmd.h"
#include "calculate.h"
#include "read_data_simple.h"

void can_suc_rx(can_msg *data) {
    uprintf("can rx ok\r\n");
}

float vega = 0;
float last_vega=0;
float vega_speed=0;
void vega_msg(can_msg *data) 
{
  last_vega=vega;
    vega = data->fl[0];
    vega_speed=last_vega-vega;  
}

void response (can_msg *data)
{
    send_Angle(angle.yawangle[2]);
}




void can_func_init() {
    can_callback_add(1, can_suc_rx);
    can_callback_add(0x10,vega_msg);
    can_callback_add(0x11,response);
}








/**************************·¢ËÍº¯Êý********************/
uint16_t CANSEND_ID=0x12;
int send_XY(float x,float y)
{
    can_msg data;
    data.fl[0]=x;
    data.fl[1]=y;
   return can_send_msg(CANSEND_ID,&data,8);
}

int send_Angle(float angle)
{    
    can_msg data;
    data.fl[0]=angle;
   return can_send_msg(CANSEND_ID,&data,4);
}