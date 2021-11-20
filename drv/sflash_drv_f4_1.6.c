
#include <stdint.h>
#include "stm32lib.h"
#include "delay_drv.h"

#define __SFLASH_DRV_C__
#include "sflash_drv.h"


#define SPI_PORT	1

#if SPI_PORT == 1
#define SFLASH_SPI	SPI1
#elif SPI_PORT == 3
#define SFLASH_SPI	SPI3
#endif


void SFlash_Init(void)
{
	//gpio 
	//cs PC13 PB6
	//CK PB3
	//MI PB4
	//MO PB5
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB 
						   | RCC_AHB1Periph_GPIOC 
							, ENABLE);

#if SPI_PORT == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);	
#elif SPI_PORT == 3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);		
#endif
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_InitStructure.GPIO_Pin);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_InitStructure.GPIO_Pin);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
#if SPI_PORT == 1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
#endif
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//SPI_BaudRatePrescaler_16;
	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SFLASH_SPI, &SPI_InitStructure);
	
	/*!< Enable the sFLASH_SPI  */
	SPI_Cmd(SFLASH_SPI, ENABLE);	
	
}

void SFlash_Cs_EnUfm(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	//sleep(100);
}

void SFlash_Cs_EnDataF(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void SFlash_Cs_AllUn(void)
{
	//sleep(100);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);

}

uint8_t SFlash_SpiRW(uint8_t d)
{
  /*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(SFLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SFLASH_SPI, d);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SFLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SFLASH_SPI);	
}

//end file

