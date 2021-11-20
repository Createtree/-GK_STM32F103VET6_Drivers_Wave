
#include "sflash_drv.h"

extern   SPI_HandleTypeDef hspi2; 





void SFlash_Cs_EnDataF(void)
{
	//GPIO_ResetBits(GPIOA,GPIO_Pin_1);//GPIO_Pin_3);
	W25_CS_GPIO_Port->ODR &= ~W25_CS_Pin ;
}

void SFlash_Cs_AllUn(void)
{
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_1);//GPIO_Pin_3);
  W25_CS_GPIO_Port->ODR |= W25_CS_Pin ;
}

uint8_t SFlash_SpiRW(uint8_t txDate)
{
	  uint8_t  rxDate ;
    HAL_SPI_TransmitReceive(&hspi2,&txDate, &rxDate,1,1);
	  return rxDate;
}
