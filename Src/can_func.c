#include "can_func.h"
#include "cmd.h"
#include "calculate.h"

void can_func_init() {
    can_callback_add(1, can_suc_rx);
}

void can_suc_rx(can_msg *data) {
    uprintf("can rx ok\r\n");
}








/**************************以下为can发送函数，不是接受调用********************/
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