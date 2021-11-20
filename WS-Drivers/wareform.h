#ifndef __WAREFORM_H__
#define __WAREFORM_H__



typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long  u64;

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "stdbool.h"
//#include "complex.h"
#include "stdarg.h"		  //  标准头文件

enum SHAPE
{
	NOTHING = 0,
	Sine = 1,							//正弦波
	Rectangle = 2,				//矩形波
	Triangle = 3,					//三角波
	Sawtooth = 4,  				//正锯齿波
	REsawtooth = 5,  			//反锯齿波
	Impulse_wave = 6,			//冲击波
};

enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty);	//ADC值 , 占空比








#endif
