

#include "wareform.h"
#include "ws_task.h"

#define Vrf 2.509

static int triangle[480];

static int abs(int x)
{
	return (x>0)? x:-x;
}

static void bubble_sort(int *arr, int len) {
    int i, j, temp;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1 - i; j++)
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty)	//ADC值 , 占空比
{
	int VPP_P1,VPP_N1,VPP_P2,VPP_N2;	//高低阈值 单位 V
	enum SHAPE shape;		//形状
	
	VPP_P1 = (int)Vmax;
	VPP_N1 = (int)Vmin;
	//方波检测
	int one_fourths1     = VPP_N1 + (VPP_P1 - VPP_N1) * 1 / 4;  //四分之一电压值
	int two_fourths1     = VPP_N1 + (VPP_P1 - VPP_N1) * 2 / 4;  //四分之二电压值
	int three_fourths1   = VPP_N1 + (VPP_P1 - VPP_N1) * 3 / 4;  //四分之三电压值
	
	int val_quarter1[4] = {0,0,0,0};        //四分之散点
	
	for(int i=0; i< 480 ; i++)
	{
		if (rec[i] > three_fourths1)
			val_quarter1[3] ++;
		else if (rec[i] > two_fourths1)
			val_quarter1[2] ++;
		else if (rec[i] > one_fourths1)
			val_quarter1[1] ++;
	}
	val_quarter1[0] = 480 - val_quarter1[3] - val_quarter1[2] - val_quarter1[1];
	
	for(int i=0; i< 475 ; i++)
	{
		triangle[i] = (int)((int)rec[i] + rec[i+1] + rec[i+2]) - (rec[i+3] + rec[i+4] + rec[i+5] );
	}
	
	bubble_sort(triangle,475);  //排序 475 个数据
	VPP_P2 = 0;       //峰峰值——高
	VPP_N2 = 0;       //单位 V
	for(int i=457; i< 467 ; i++)
	{
		VPP_P2 += triangle[i];
	}
	VPP_P2 /= 10;
	for(int i=13; i< 23 ; i++)
	{
		VPP_N2 += triangle[i];
	}
	VPP_N2 /= 10;
	
//	WS_Debug(1,"vpp = %4d %4d\r\n\r\n",VPP_P2,VPP_N2); 
	
	int one_fourths2     = VPP_N2 + (VPP_P2 - VPP_N2) * 1 / 4;  //四分之一电压值
	int two_fourths2     = VPP_N2 + (VPP_P2 - VPP_N2) * 2 / 4;  //四分之二电压值
	int three_fourths2   = VPP_N2 + (VPP_P2 - VPP_N2) * 3 / 4;  //四分之三电压值

	int val_quarter2[4] = {0,0,0,0};        //四分之散点
	int section_ZERO = 0;
	for(int i=0; i< 475 ; i++)
	{
		if (triangle[i] > three_fourths2)
			val_quarter2[3] ++;
		else if (triangle[i] > two_fourths2)
			val_quarter2[2] ++;
		else if (triangle[i] > one_fourths2)
			val_quarter2[1] ++;
		if (triangle[i] > 0)
			section_ZERO ++;
	}
	val_quarter2[0] = 475 - val_quarter2[3] - val_quarter2[2] - val_quarter2[1];
	
	
	//WS_Debug(1,"val = %4d %4d %4d %4d \r\n",val_quarter1[0],val_quarter1[1],val_quarter1[2],val_quarter1[3]); 
	//WS_Debug(1,"val = %4d %4d %4d %4d \r\n",val_quarter2[0],val_quarter2[1],val_quarter2[2],val_quarter2[3]); 
	
	shape = NOTHING;
	int Standard = abs(val_quarter1[0] - val_quarter2[0] ) + abs(val_quarter1[1] - val_quarter2[1] ) + 
                   abs(val_quarter1[2] - val_quarter2[2] ) + abs(val_quarter1[3] - val_quarter2[3] );
	
	if(VPP_P1-VPP_N1 < 0.05*4096/2.5)
	{
		shape = NOTHING;
	}
	//方波
	else if ((val_quarter1[0] + val_quarter1[3])> 440)
	{
		shape = Rectangle;     //矩形波
	}
	else if (abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 100  && 
		(section_ZERO > 340 || section_ZERO < 150)	&& Standard > 400)
	{
		if (section_ZERO < 250)
		{
			shape = Sawtooth;     //正锯齿波
		}
		else
		{
			shape = REsawtooth;   //反锯齿波
		}
	}
	else if ((abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 80  && Standard > 240))
	{
		shape = Triangle;      //三角波
	}
	else if ((abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 80  && Standard < 110))
	{
		shape = Sine;          //正弦波
	}
	return shape;
}









//enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty)	//ADC值 , 占空比
//{
//	int VAL_H,VAL_L;	//高低阈值 单位 V
//	float Rate_H,Rate_M,Rate_L;		//占有百分比
//	float EX_Rate,average;  //方差 平均值
//	int section_H,section_M,section_L;
//	int Difference;			//相减后得到的值
//	enum SHAPE shape;		//形状
//	int n;
//	
//	VAL_H = (int)(Vmax - Vmin) /10 + 50;				//高阈值 单位：ADC值
//	VAL_L = VAL_H * -1;												  //低阈值
//	
//	section_H = 0;   														//大于高阈值的区间
//	section_M = 0;   														//处于中阈值的区间
//	section_L = 0;   														//小于低阈值的区间
//	for(int i = 0; i < 475 ; i++)               //统计点的数量
//	{
//		Difference = (rec[i] + rec[i+1] + rec[i+2]) - (rec[i+3] + rec[i+4] + rec[i+5]);
//		if (Difference > VAL_H)
//		{
//				section_H ++;
//		}else if (Difference < VAL_L){
//				section_L ++;
//		}
//		else{
//				section_M ++;
//		}
//	}
//	
//	Rate_H = section_H *100.0 / 475.0;
//	Rate_M = section_M *100.0 / 475.0;
//	Rate_L = section_L *100.0 / 475.0;
//	
//	average = (Rate_H+Rate_M+Rate_L)/3.0;
//	
//	EX_Rate = (pow(Rate_H-average,2) + pow(Rate_M-average,2) + pow(Rate_L-average,2)) / 3.0;	//方差
//	
//	shape = NOTHING;
//	if( (int)(Vmax - Vmin) < 500) 	//低于0.4V
//	{
//			shape = NOTHING;
//	}
//	else if (Rate_M > 70)      //中间多，两边少 ，，， 矩形波 锯齿 冲击波
//	{
////		WS_Debug(1,"section_H = %d\r\n", section_H);
////		WS_Debug(1,"section_L = %d\r\n", section_L);
//		if(section_H > 10 && section_L > 10)
//		{
//			if(duty > 20)
//			{
//				shape = rectangle ; 	//矩形波
//			}else{
//				shape = Impulse_wave;	//冲击波
//			}
//		}else if ((section_H-section_L > 0) && section_L < 5){
//				shape = sawtooth ;  	//锯齿波
//		}else if (section_H < 5 && (section_L - section_H > 0)){
//				shape = REsawtooth ;  //反锯齿波
//		}
//	}else{
//		if((Rate_H + Rate_L - Rate_M)> 50 && Rate_M < 10 && EX_Rate)    //两边多，中间少  ，，， 三角波
//		{
//				shape = triangle;		//三角波
//		}
//		else if (Rate_H < 45 && Rate_L < 45 && Rate_M < 45 && Rate_H-Rate_M<20)    //两边多，中间少  ，，， 正弦波
//		{
//				shape = sine;				//正弦波
//		}
//	}
//	
////	WS_Debug(1,"H M L = %.2f , %.2f , %.2f\r\n",Rate_H,Rate_M,Rate_L);
////	WS_Debug(1,"shape = %s\r\n",(shape==1)? "正弦波":(shape==2)? 
////		"矩形波":(shape==3)? "三角波":(shape==4)? "正锯齿波":(shape==5)? "反锯齿波":""); 
////	WS_Debug(1,"VAL_H = %d\r\n",VAL_H); 
////	WS_Debug(1,"VAL_L = %d\r\n\r\n",VAL_L); 
//	return shape;
//}







//#include "wareform.h"
//#include "ws_task.h"

//#define Vrf 2.509

//static int triangle[480];

//static int abs(int x)
//{
//	return (x>0)? x:-x;
//}

//static void bubble_sort(int *arr, int len) {
//    int i, j, temp;
//    for (i = 0; i < len - 1; i++)
//        for (j = 0; j < len - 1 - i; j++)
//            if (arr[j] > arr[j + 1]) {
//                temp = arr[j];
//                arr[j] = arr[j + 1];
//                arr[j + 1] = temp;
//            }
//}

//enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty)	//ADC值 , 占空比
//{
//	int VPP_P1,VPP_N1,VPP_P2,VPP_N2;	//高低阈值 单位 V
//	enum SHAPE shape;		//形状
//	
//	VPP_P1 = (int)Vmax;
//	VPP_N1 = (int)Vmin;
//	//方波检测
//	int one_fourths1     = VPP_N1 + (VPP_P1 - VPP_N1) * 1 / 4;  //四分之一电压值
//	int two_fourths1     = VPP_N1 + (VPP_P1 - VPP_N1) * 2 / 4;  //四分之二电压值
//	int three_fourths1   = VPP_N1 + (VPP_P1 - VPP_N1) * 3 / 4;  //四分之三电压值
//	
//	int val_quarter1[4] = {0,0,0,0};        //四分之散点
//	
//	for(int i=0; i< 480 ; i++)
//	{
//		if (rec[i] > three_fourths1)
//			val_quarter1[3] ++;
//		else if (rec[i] > two_fourths1)
//			val_quarter1[2] ++;
//		else if (rec[i] > one_fourths1)
//			val_quarter1[1] ++;
//	}
//	val_quarter1[0] = 480 - val_quarter1[3] - val_quarter1[2] - val_quarter1[1];
//	
//	for(int i=0; i< 475 ; i++)
//	{
//		triangle[i] = (int)((int)rec[i] + rec[i+1] + rec[i+2]) - (rec[i+3] + rec[i+4] + rec[i+5] );
//	}
//	
//	bubble_sort(triangle,475);  //排序 475 个数据
//	VPP_P2 = 0;       //峰峰值——高
//	VPP_N2 = 0;       //单位 V
//	for(int i=457; i< 467 ; i++)
//	{
//		VPP_P2 += triangle[i];
//	}
//	VPP_P2 /= 10;
//	for(int i=13; i< 23 ; i++)
//	{
//		VPP_N2 += triangle[i];
//	}
//	VPP_N2 /= 10;
//	
////	WS_Debug(1,"vpp = %4d %4d\r\n\r\n",VPP_P2,VPP_N2); 
//	
//	int one_fourths2     = VPP_N2 + (VPP_P2 - VPP_N2) * 1 / 4;  //四分之一电压值
//	int two_fourths2     = VPP_N2 + (VPP_P2 - VPP_N2) * 2 / 4;  //四分之二电压值
//	int three_fourths2   = VPP_N2 + (VPP_P2 - VPP_N2) * 3 / 4;  //四分之三电压值

//	int val_quarter2[4] = {0,0,0,0};        //四分之散点
//	int section_ZERO = 0;
//	for(int i=0; i< 475 ; i++)
//	{
//		if (triangle[i] > three_fourths2)
//			val_quarter2[3] ++;
//		else if (triangle[i] > two_fourths2)
//			val_quarter2[2] ++;
//		else if (triangle[i] > one_fourths2)
//			val_quarter2[1] ++;
//		if (triangle[i] > 0)
//			section_ZERO ++;
//	}
//	val_quarter2[0] = 475 - val_quarter2[3] - val_quarter2[2] - val_quarter2[1];
//	
//	
//	//WS_Debug(1,"val = %4d %4d %4d %4d \r\n",val_quarter1[0],val_quarter1[1],val_quarter1[2],val_quarter1[3]); 
//	//WS_Debug(1,"val = %4d %4d %4d %4d \r\n",val_quarter2[0],val_quarter2[1],val_quarter2[2],val_quarter2[3]); 
//	
//	shape = NOTHING;
//	int Standard = abs(val_quarter1[0] - val_quarter2[0] ) + abs(val_quarter1[1] - val_quarter2[1] ) + 
//                   abs(val_quarter1[2] - val_quarter2[2] ) + abs(val_quarter1[3] - val_quarter2[3] );
//	//方波
//	if ((val_quarter1[0] + val_quarter1[3])> 440)
//	{
//		shape = Rectangle;     //矩形波
//	}
//	else if (abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 100  && 
//		(section_ZERO > 340 || section_ZERO < 150)	&& Standard > 400)
//	{
//		if (section_ZERO < 250)
//		{
//			shape = Sawtooth;     //正锯齿波
//		}
//		else
//		{
//			shape = REsawtooth;   //反锯齿波
//		}
//	}
//	else if ((abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 80  && Standard > 240))
//	{
//		shape = Triangle;      //三角波
//	}
//	else if ((abs((val_quarter1[0] + val_quarter1[1] ) - (val_quarter1[2] + val_quarter1[3] )) < 80  && Standard < 110))
//	{
//		shape = Sine;          //正弦波
//	}
//	return shape;
//}









////enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty)	//ADC值 , 占空比
////{
////	int VAL_H,VAL_L;	//高低阈值 单位 V
////	float Rate_H,Rate_M,Rate_L;		//占有百分比
////	float EX_Rate,average;  //方差 平均值
////	int section_H,section_M,section_L;
////	int Difference;			//相减后得到的值
////	enum SHAPE shape;		//形状
////	int n;
////	
////	VAL_H = (int)(Vmax - Vmin) /10 + 50;				//高阈值 单位：ADC值
////	VAL_L = VAL_H * -1;												  //低阈值
////	
////	section_H = 0;   														//大于高阈值的区间
////	section_M = 0;   														//处于中阈值的区间
////	section_L = 0;   														//小于低阈值的区间
////	for(int i = 0; i < 475 ; i++)               //统计点的数量
////	{
////		Difference = (rec[i] + rec[i+1] + rec[i+2]) - (rec[i+3] + rec[i+4] + rec[i+5]);
////		if (Difference > VAL_H)
////		{
////				section_H ++;
////		}else if (Difference < VAL_L){
////				section_L ++;
////		}
////		else{
////				section_M ++;
////		}
////	}
////	
////	Rate_H = section_H *100.0 / 475.0;
////	Rate_M = section_M *100.0 / 475.0;
////	Rate_L = section_L *100.0 / 475.0;
////	
////	average = (Rate_H+Rate_M+Rate_L)/3.0;
////	
////	EX_Rate = (pow(Rate_H-average,2) + pow(Rate_M-average,2) + pow(Rate_L-average,2)) / 3.0;	//方差
////	
////	shape = NOTHING;
////	if( (int)(Vmax - Vmin) < 500) 	//低于0.4V
////	{
////			shape = NOTHING;
////	}
////	else if (Rate_M > 70)      //中间多，两边少 ，，， 矩形波 锯齿 冲击波
////	{
//////		WS_Debug(1,"section_H = %d\r\n", section_H);
//////		WS_Debug(1,"section_L = %d\r\n", section_L);
////		if(section_H > 10 && section_L > 10)
////		{
////			if(duty > 20)
////			{
////				shape = rectangle ; 	//矩形波
////			}else{
////				shape = Impulse_wave;	//冲击波
////			}
////		}else if ((section_H-section_L > 0) && section_L < 5){
////				shape = sawtooth ;  	//锯齿波
////		}else if (section_H < 5 && (section_L - section_H > 0)){
////				shape = REsawtooth ;  //反锯齿波
////		}
////	}else{
////		if((Rate_H + Rate_L - Rate_M)> 50 && Rate_M < 10 && EX_Rate)    //两边多，中间少  ，，， 三角波
////		{
////				shape = triangle;		//三角波
////		}
////		else if (Rate_H < 45 && Rate_L < 45 && Rate_M < 45 && Rate_H-Rate_M<20)    //两边多，中间少  ，，， 正弦波
////		{
////				shape = sine;				//正弦波
////		}
////	}
////	
//////	WS_Debug(1,"H M L = %.2f , %.2f , %.2f\r\n",Rate_H,Rate_M,Rate_L);
//////	WS_Debug(1,"shape = %s\r\n",(shape==1)? "正弦波":(shape==2)? 
//////		"矩形波":(shape==3)? "三角波":(shape==4)? "正锯齿波":(shape==5)? "反锯齿波":""); 
//////	WS_Debug(1,"VAL_H = %d\r\n",VAL_H); 
//////	WS_Debug(1,"VAL_L = %d\r\n\r\n",VAL_L); 
////	return shape;
////}




