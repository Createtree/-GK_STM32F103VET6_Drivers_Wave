
#ifndef _APP_CAPTURE_H__
#define _APP_CAPTURE_H__

#include "ws_main.h"


void ccpIntrruptFun(void);


//   �ֶ�������㺯��
//  mode: ָ������, 0 �� �趨������㣬����0 ��
//                  1 �� �����յ㣬������㵽�յ�ò���ʱ��,���ʱ�䲻�޸�
//                  2 �� �������/�ص㣬ÿ�ε��ò����ϴε��õ����ε��õ�ʱ��
long WS_CCP_Measure_Time(char chx , char mode);

#endif

