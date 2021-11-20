


#include "ws_capture.h"

/*************************************************

��ʱ������Ҫ�� ÿ�������ʱ��1us , ����0xffff��������ж�

**************************************************/

//       ���������Ԫ
#define  CCPCTR_LEN 10

struct  
{
	 unsigned short int intNum;
	 unsigned short int ccpValue1;
	 unsigned short int ccpValue2;
	 long               ccpT;

}ccpCtr[CCPCTR_LEN];


//  �������������
void triggerCCP(void)
{
	   TIM5->EGR |= 1<<1;  //  ���¼��Ĵ���������һ������
}
//  ��ȡ����Ĵ���ֵ
unsigned int getCCPValue(void)
{
   return TIM5->CCR1;
}


//   �����ʼ����д������΢��������ж�������
void initCHxCaptureTime(unsigned short int um,unsigned short int upCount)
{
	 char i;
	 static char initIfg = 0 ;  // �״ε��ó�ʼ��
	 if(initIfg == 0)
	 {  
		   initIfg = 1;
			 for(i = 0;i < CCPCTR_LEN;i++) 
					{
						 ccpCtr[i].intNum = 0   ;
						 ccpCtr[i].ccpValue2 = 0;
					}
		}
}

//  ����жϺ���
void ccpIntrruptFun(void)
{ 
	   char i;   
		 for(i = 0;i < CCPCTR_LEN;i++) 
			{
				 ccpCtr[i].intNum ++  ;
			}	 		 
}

/******************************************************************************
	*�������ƣ� long WS_CCP_Measure_Time(char chx , char mode)
	*���ܣ�     ���õ��õļ������ʱ����
              ʾ����
              WS_CCP_Measure_Time(0,0);
              WS_Delay_MS(10);
              time = WS_CCP_Measure_Time(0,2);
              time �����������ε��õ�ʱ������ʵ�ʲ����� 
                   WS_Delay_MS(10)����ִ���������ʱ�䣬��λUS
 �βΣ�    mode:   0 �� �趨������㣬����0 ��
                   1 �� �����յ㣬������㵽�յ�ò���ʱ��,���ʱ�䲻�޸�
                   2 �� �������/�ص㣬ÿ�ε��ò����ϴε��õ����ε��õ�ʱ��
           chx:    ����ͨ����0~CCPCTR_LEN���� ֧�ֶ�·֪ͨ����
	*����:		  ��
*******************************************************************************/



long WS_CCP_Measure_Time(char chx , char mode)
{
	 long  T  = 0;  
	 switch(mode)
	 {
		 // �趨���
		 case 0:  triggerCCP();
							ccpCtr[chx].ccpValue2  = getCCPValue();  // 
              ccpCtr[chx].intNum     = 0;		 
			        break;
		 
		 //  �趨�ص㣬���ز���ʱ��
		 case 1:
			        triggerCCP();
							ccpCtr[chx].ccpValue1  = getCCPValue();            
							ccpCtr[chx].ccpT  = ccpCtr[chx].intNum;
							ccpCtr[chx].ccpT <<= 16;
							ccpCtr[chx].ccpT += ccpCtr[chx].ccpValue1;
							ccpCtr[chx].ccpT -= ccpCtr[chx].ccpValue2;
							//ccpCtr[chx].ccpT *= ccp_um;					
							
		          T = ccpCtr[chx].ccpT;
		          break;
		 //  ��������
		 case 2:  triggerCCP();
							ccpCtr[chx].ccpValue1  = getCCPValue();            
							ccpCtr[chx].ccpT  = ccpCtr[chx].intNum;
							ccpCtr[chx].ccpT <<= 16;
							ccpCtr[chx].ccpT += ccpCtr[chx].ccpValue1;
							ccpCtr[chx].ccpT -= ccpCtr[chx].ccpValue2;
							//ccpCtr[chx].ccpT *= ccp_um;					
							ccpCtr[chx].ccpValue2 = ccpCtr[chx].ccpValue1;
							ccpCtr[chx].intNum    = 0;
		          T = ccpCtr[chx].ccpT;
			        break;
	 }
	 
	  return T  ;
}

