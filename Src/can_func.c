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








/**************************·¢ËÍº¯Êı********************/
uint16_t CANSEND_ID=0x12;
float n=0;
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

int send_to_nrf()
{   
  static int ch=0;
  ch++;
  ch%=10;  
  can_msg data;
  data.in[0]=ch;
 
  
  /*****test**********/
  static float inin=0;
  inin++;
  switch (ch)
  {
  case 0 : data.fl[1] = inin; break;
  case 1 : data.fl[1] = inin; break;
  case 2 : data.fl[1] = inin; break;
  case 3 : data.fl[1] = inin; break;
  case 4 : data.fl[1] = inin; break;
  case 5 : data.fl[1] = inin; break;
  case 6 : data.fl[1] = 111; break;
  case 7 : data.fl[1] = 222; break;
  case 8 : data.fl[1] = 333; break;
  case 9 : data.fl[1] = 444; break;
  default:break;
  }
  /*****test**********/
  
//  switch (ch)
//  {
//  case 0 : data.fl[1] = 123; break;
//  case 1 : data.fl[1] = 456; break;
//  case 2 : data.fl[1] = 789; break;
//  case 3 : data.fl[1] = 987; break;
//  case 4 : data.fl[1] = 654; break;
//  case 5 : data.fl[1] = 321; break;
//  case 6 : data.fl[1] = 111; break;
//  case 7 : data.fl[1] = 222; break;
//  case 8 : data.fl[1] = 333; break;
//  case 9 : data.fl[1] = 444; break;
//  default:break;
//  }
  
  return can_send_msg(230,&data,8);
}

