


#include "w25q32_drv.h"



uint32_t W25Q32_CMD_Read_JEDEC_ID(void)  
{
	uint32_t Rt = 0;
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x9f);
	Rt =  SFlash_SpiRW(0xff);
	if(Rt == 0x0ff)
		Rt = SFlash_SpiRW(0xff);
	Rt = (Rt << 8) + SFlash_SpiRW(0xff);
	Rt = (Rt << 8) + SFlash_SpiRW(0xff);
	SFlash_Cs_AllUn();
	return Rt;
}

uint16_t W25Q32_CMD_ReadStatusReg(void)
{
	uint16_t Rt;
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x35);
	Rt = SFlash_SpiRW(0xff);
	SFlash_Cs_AllUn();
	
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x05);
	Rt = (Rt << 8) + SFlash_SpiRW(0xff);
	SFlash_Cs_AllUn();
	
	return Rt;
}

void W25Q32_CMD_WriteStatusReg(uint16_t d)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x01);
	SFlash_SpiRW(d&0xff);
	SFlash_SpiRW(d>>8);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_WriteEnable(void)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x06);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_WriteDisable(void)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x04);
	SFlash_Cs_AllUn();	
}

static void W25Q32_CMD_SendAddr(uint32_t a)
{
	SFlash_SpiRW(a>>16);
	SFlash_SpiRW(a>>8);
	SFlash_SpiRW(a);
}

void W25Q32_CMD_Read(uint8_t *p,uint32_t l,uint32_t a)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x03);
	W25Q32_CMD_SendAddr(a);
	while(l--)
		*p++ = SFlash_SpiRW(0xff);
	SFlash_Cs_AllUn();	
}

void W25Q32_CMD_Write(uint8_t *p,uint32_t l,uint32_t a)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x02);
	W25Q32_CMD_SendAddr(a);
	while(l--)
		SFlash_SpiRW(*p++);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_EraseSector4KB(uint32_t a)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x20);
	W25Q32_CMD_SendAddr(a);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_EraseBlock32KB(uint32_t a)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0x52);
	W25Q32_CMD_SendAddr(a);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_EraseBlock64KB(uint32_t a)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0xd8);
	W25Q32_CMD_SendAddr(a);
	SFlash_Cs_AllUn();
}

void W25Q32_CMD_EraseChip(void)
{
	SFlash_Cs_EnDataF();
	SFlash_SpiRW(0xc7);
	SFlash_Cs_AllUn();
}

void W25Q32_WaitBusy(void)
{
	uint16_t r;
	while(1)//Sprom_CMD_ReadStatusReg()&0x01)
	{
		r = W25Q32_CMD_ReadStatusReg();
		if(r & 0x1c)
			W25Q32_CMD_WriteStatusReg(r&0xc463);
		else if((r&0x02) == 0)
			W25Q32_CMD_WriteEnable();
		else if((r&0x01) == 0)
			break;
		osDelay(10);
	}	
}

void W25Q32_Data_Clear(uint32_t a)
{
	W25Q32_WaitBusy();
	W25Q32_CMD_EraseSector4KB(a);
	W25Q32_WaitBusy();
}

void W25Q32_Data_Read(void *pbuf,uint32_t l_byte,uint32_t a)
{
	W25Q32_WaitBusy();
	W25Q32_CMD_Read((uint8_t *)pbuf,l_byte,a);
}

void W25Q32_Data_Write(void *pbuf,uint32_t l_byte,uint32_t a)
{
	W25Q32_WaitBusy();
	W25Q32_CMD_Write((uint8_t *)pbuf,l_byte,a);
	W25Q32_WaitBusy();
}

static uint8_t tbuf[16];

void W25Q32_Test(void)
{
#if 0//1
	uint32_t a;
	a = 0x00030;
	MLib_memset(tbuf,0,sizeof(tbuf));
	W25Q32_Data_Read(tbuf,sizeof(tbuf),a);
	//W25Q32_Data_Clear(a);
	W25Q32_Data_Read(tbuf,sizeof(tbuf),a);
	MLib_memint(tbuf,0,sizeof(tbuf));
	//W25Q32_Data_Write(tbuf,sizeof(tbuf),a);
	MLib_memset(tbuf,0,sizeof(tbuf));
	W25Q32_Data_Read(tbuf,sizeof(tbuf),a);
	osDelay(10);	

#endif
}

void W25Q32_TestID(void)
{
	uint32_t Id , i = 0;
	
	while(++i<= 10)
	{
		Id = W25Q32_CMD_Read_JEDEC_ID();
		Id = W25Q32_CMD_Read_JEDEC_ID();
		if((Id != 0)&&(Id != 0xffffffff))
			break;
		osDelay(10);
		
	}
	WS_Debug(1,"W25 ID = :%X\r\n",Id);
}



