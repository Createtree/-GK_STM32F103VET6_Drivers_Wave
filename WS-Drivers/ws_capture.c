


#include "ws_capture.h"

/*************************************************

定时器配置要求： 每个脉冲的时间1us , 计满0xffff产生溢出中断

**************************************************/

//       定义测量单元
#define  CCPCTR_LEN 10

struct  
{
	 unsigned short int intNum;
	 unsigned short int ccpValue1;
	 unsigned short int ccpValue2;
	 long               ccpT;

}ccpCtr[CCPCTR_LEN];


//  由软件触发捕获
void triggerCCP(void)
{
	   TIM5->EGR |= 1<<1;  //  由事件寄存器，产生一个软捕获
}
//  读取捕获寄存器值
unsigned int getCCPValue(void)
{
   return TIM5->CCR1;
}


//   捕获初始化，写单脉冲微妙与溢出中断脉冲数
void initCHxCaptureTime(unsigned short int um,unsigned short int upCount)
{
	 char i;
	 static char initIfg = 0 ;  // 首次调用初始化
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

//  溢出中断函数
void ccpIntrruptFun(void)
{ 
	   char i;   
		 for(i = 0;i < CCPCTR_LEN;i++) 
			{
				 ccpCtr[i].intNum ++  ;
			}	 		 
}

/******************************************************************************
	*方法名称： long WS_CCP_Measure_Time(char chx , char mode)
	*功能：     利用调用的间隔测量时间间隔
              示范：
              WS_CCP_Measure_Time(0,0);
              WS_Delay_MS(10);
              time = WS_CCP_Measure_Time(0,2);
              time 变量存贮两次调用的时间间隔，实际测量的 
                   WS_Delay_MS(10)函数执行完所需的时间，单位US
 形参：    mode:   0 ： 设定测量起点，返回0 ；
                   1 ： 测量终点，返回起点到终点得测量时间,起点时间不修改
                   2 ： 测量起点/重点，每次调用测量上次调用到本次调用的时间
           chx:    测量通道（0~CCPCTR_LEN）， 支持多路通知测量
	*返回:		  无
*******************************************************************************/



long WS_CCP_Measure_Time(char chx , char mode)
{
	 long  T  = 0;  
	 switch(mode)
	 {
		 // 设定起点
		 case 0:  triggerCCP();
							ccpCtr[chx].ccpValue2  = getCCPValue();  // 
              ccpCtr[chx].intNum     = 0;		 
			        break;
		 
		 //  设定重点，返回测量时间
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
		 //  连续测量
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

