#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"
#include "com_protocol.h"
#include "my_lib.h"

#define __KEYLED_DRV_C__
#include "keyled_drv.h"

static u16 cled_flag;

void KL_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC
						   , ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 
								| GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}


static uint8_t KL_GetNewState(uint8_t of,uint8_t cf,uint8_t b)
{
	uint8_t Rt = 0;
	uint8_t t = 0x01<<b;
	
	if(((of&t) == 0)&&((cf&t) != 0))
	   Rt = 1;
	
	return Rt;
}

uint8_t KL_GetKeyState(void)
{
	uint8_t Rt;
	static uint8_t Of;
	Rt = GPIO_ReadInputData(GPIOC)&0x3f;
	if(Of != Rt)
	{
		if((Of&0x20) != (Rt&0x20))
		{
			if(Of&0x20)
			{
				ComP_UartKeySend(6);
				//UD_ReClear();
			}
		}
		
		if(KL_GetNewState(Of,Rt,0))
			ComP_UartKeySend(1);
		if(KL_GetNewState(Of,Rt,1))
			ComP_UartKeySend(2);
		if(KL_GetNewState(Of,Rt,2))
			ComP_UartKeySend(3);
		if(KL_GetNewState(Of,Rt,3))
			ComP_UartKeySend(4);
		if(KL_GetNewState(Of,Rt,4))
			ComP_UartKeySend(5);
		
		Of = Rt;
		
	}
	
	return Rt;
}


uint8_t KL_GetKeyIoState(void)
{
	uint8_t Rt;
	Rt = GPIO_ReadInputData(GPIOC)&0x3f;
	return Rt;
}
//end file

