#ifndef __can_func_H
#define __can_func_H
#ifdef __cplusplus
extern "C" {
#endif
#include "can_utils.h"
#include "can.h"
    
void can_func_init();
void can_suc_rx(can_msg *data);






/**************发送位�?信息**********/
extern uint16_t CANSEND_ID;
int send_Angle(float angle);
int send_XY(float x,float y);

#ifdef __cplusplus
}
#endif
#endif /*__ can_func_H */