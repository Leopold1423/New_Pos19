#ifndef __CMD_FUNC_H
#define __CMD_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmd.h"


extern int test_flag_value[5];
extern int test_int_value[5];
extern float test_float_value[5];

void cmd_func_init(void);
void cmd_hello_func(int argc,char *argv[]);  
void cmd_can_test(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* __CMD_FUNC_H */