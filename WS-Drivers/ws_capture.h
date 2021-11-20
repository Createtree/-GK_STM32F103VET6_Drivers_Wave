
#ifndef _APP_CAPTURE_H__
#define _APP_CAPTURE_H__

#include "ws_main.h"


void ccpIntrruptFun(void);


//   手动捕获计算函数
//  mode: 指定操作, 0 ： 设定测量起点，返回0 ；
//                  1 ： 测量终点，返回起点到终点得测量时间,起点时间不修改
//                  2 ： 测量起点/重点，每次调用测量上次调用到本次调用的时间
long WS_CCP_Measure_Time(char chx , char mode);

#endif

