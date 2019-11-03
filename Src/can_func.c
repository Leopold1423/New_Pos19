#include "can_func.h"
#include "cmd.h"
#include "calculate.h"
#include "read_data_simple.h"

float vega = 0;
float last_vega=0;
float vega_speed=0;
static int ch=0;
void vega_msg(can_msg *data) {
  last_vega=vega;
    vega = data->fl[0];
    vega_speed=last_vega-vega;
//    ch++;
//    if(ch==10)
//    {
//      ch=0;
//      if(angle.delta_yawangle[2]!=0)
//    uprintf("%f\r",vega_speed/angle.delta_yawangle[2]);
//    }
    
}




void can_func_init() {
    can_callback_add(1, can_suc_rx);
    can_callback_add(0x10,vega_msg);
}

void can_suc_rx(can_msg *data) {
    uprintf("can rx ok\r\n");
}






/**************************以下为can发送函数，不是接受调用********************/
uint16_t CANSEND_ID=0x10;
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