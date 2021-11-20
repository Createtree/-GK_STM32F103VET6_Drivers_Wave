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
#include "stdarg.h"		  //  ��׼ͷ�ļ�

enum SHAPE
{
	NOTHING = 0,
	Sine = 1,							//���Ҳ�
	Rectangle = 2,				//���β�
	Triangle = 3,					//���ǲ�
	Sawtooth = 4,  				//����ݲ�
	REsawtooth = 5,  			//����ݲ�
	Impulse_wave = 6,			//�����
};

enum SHAPE Wareform(u16 *rec , float Vmax , float Vmin , u16 duty);	//ADCֵ , ռ�ձ�








#endif
