
#include <stdint.h>
#include <string.h>
#include "stm32f10x.h"
#include "CanP_DispCom.h"

#define __UART_DRV_C__
#include "uart_drv.h"

static volatile u8 RxData;
static volatile u8 Uart_RxTemp;

#define DEF_BPS	115200	//256000	//	//115200


USART_TypeDef* uart_prot_buf[]=
{
	NULL,
	USART1,
	USART2,
	USART3,
	UART4,
	UART5,
};

void USART1_IRQHandler(void)
{
	if(uart_struct[0].run_flag)
		Uart_Irq(&uart_struct[0]);
	else
		USART_Cmd(USART1, DISABLE);
}

/*
void USART2_IRQHandler(void)
{
	if(uart_struct[1].run_flag)
		Uart_Irq(&uart_struct[1]);
	else
		USART_Cmd(USART2, DISABLE);
}
*/

void Uart_Struct_Clear(Uart_Buf_Struct *p)
{
	memset(p,0,sizeof(Uart_Buf_Struct));
}

void Uart_Init(void)
{
	memset(uart_struct,0,sizeof(uart_struct));
}

void Uart_Irq(Uart_Buf_Struct *p)
{
	
	if(USART_GetITStatus(uart_prot_buf[p->uart_port], USART_IT_TXE) != RESET)
	{
		p->tx_curr++;
		if(p->tx_curr < p->tx_leng)
			uart_prot_buf[p->uart_port]->DR=p->tx_buf[p->tx_curr];
		else
			USART_ITConfig(uart_prot_buf[p->uart_port], USART_IT_TXE, DISABLE);
			
		USART_ClearITPendingBit(uart_prot_buf[p->uart_port], USART_IT_TXE);		
	}
	
	if(USART_GetITStatus(uart_prot_buf[p->uart_port], USART_IT_RXNE) != RESET)
	{
		p->rx_times = 0;
		RxData = uart_prot_buf[p->uart_port]->DR;
		USART_ClearITPendingBit(uart_prot_buf[p->uart_port], USART_IT_RXNE);
		if(p->rx_leng < p->rx_size)
			p->rx_buf[p->rx_leng++] = RxData;
		else if(p->rx_overs < CONST_OVERMAX)
			p->rx_overs++;

	}
	
	if(USART_GetFlagStatus(uart_prot_buf[p->uart_port], USART_FLAG_ORE) != RESET)
	{
		Uart_RxTemp = uart_prot_buf[p->uart_port]->DR;
	}
	
}

void UartTx_Clear(Uart_Buf_Struct *p)
{
	USART_ITConfig(uart_prot_buf[p->uart_port], USART_IT_TXE, DISABLE);
	p->tx_leng = 0;
	p->tx_curr = 0;
	p->tx_overs = 0;
	USART_ClearITPendingBit(uart_prot_buf[p->uart_port], USART_IT_TXE);
}

void UartTx_AddStr(u8 *b,u8 ml,Uart_Buf_Struct *p)
{
	u8 i;
	for(i = 0;i<ml;i++)
	{
		if(b[i] == 0)
			break;
		if(p->tx_leng<p->tx_size)
			p->tx_buf[p->tx_leng++] = b[i];
		else if(p->tx_overs < CONST_OVERMAX)
			p->tx_overs++;
	}
}

void UartTx_AddChar(u8 d,Uart_Buf_Struct *p)
{
	if(p->tx_leng < p->tx_size)
		p->tx_buf[p->tx_leng++] = d;
	else if(p->tx_overs < CONST_OVERMAX)
		p->tx_overs++;
}


void UartTx_Start(Uart_Buf_Struct *p)
{
	p->tx_curr = 0;
	uart_prot_buf[p->uart_port]->DR=p->tx_buf[0];
	USART_ITConfig(uart_prot_buf[p->uart_port], USART_IT_TXE, ENABLE);

}

u8 UartTx_EndCheck(Uart_Buf_Struct *p)
{
	return (p->tx_curr == p->tx_leng)? 1:0;
}

void UartRx_Clear(Uart_Buf_Struct *p)
{
	USART_ITConfig(uart_prot_buf[p->uart_port], USART_IT_RXNE, DISABLE);
	p->rx_leng = 0;
	p->rx_overs = 0;
	p->rx_times = 0;
	USART_ITConfig(uart_prot_buf[p->uart_port], USART_IT_RXNE, ENABLE);
}



u32 UartRx_GetData(u8 *pr,u16 ml,Uart_Buf_Struct *p)
{
	if(ml > p->rx_leng)
		ml = p->rx_leng;
	if((pr != NULL)&&(ml != 0))
		memcpy(pr,p->rx_buf,ml);
	return ml;
}

#if 0
u32 UartRx_GetLen(Uart_Buf_Struct *p)
{
	return p->rx_leng;
}


u8 UartRx_GetChar(u32 offset,Uart_Buf_Struct *p)
{
	return p->rx_buf[offset];
}

u32 UartRx_GetLostTime(Uart_Buf_Struct *p)
{
	return 	p->rx_times;
}
#endif

void UartRx_TimeCb(void)
{
	static u8 i;
	for(i = 0;i<(sizeof(uart_struct)/sizeof(uart_struct[0]));i++)
	{
		if((uart_struct[i].run_flag)&&(uart_struct[i].rx_times < UINT32_MAX))
			uart_struct[i].rx_times++;
	}

}

//______________________________________________________________________________

void STM_EVAL_COMInit( USART_InitTypeDef* USART_InitStruct)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	/* USART configuration */
	USART_Init(USART1, USART_InitStruct);
	
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* USARTx configured as follows:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	//USART_InitStructure.USART_BaudRate = 115200;
	//USART_InitStructure.USART_BaudRate = 2000000;
	USART_InitStructure.USART_BaudRate = DEF_BPS;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	STM_EVAL_COMInit( &USART_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the TIM2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	

}

#define CR1_OVER8_Set             ((u16)0x8000)  /* USART OVER8 mode Enable Mask */
#define CR1_OVER8_Reset           ((u16)0x7FFF)  /* USART OVER8 mode Disable Mask */

void UartDrv_BaudRate_Set(USART_TypeDef* USARTx, uint32_t USART_BaudRate)
{
	uint32_t tmpreg = 0x00, apbclock = 0x00;
	uint32_t integerdivider = 0x00;
	uint32_t fractionaldivider = 0x00;
	uint32_t usartxbase = 0;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	usartxbase = (uint32_t)USARTx;
	
	/*---------------------------- USART BRR Configuration -----------------------*/
	/* Configure the USART Baud Rate -------------------------------------------*/
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	if (usartxbase == USART1_BASE)
	{
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else
	{
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}
	
	/* Determine the integer part */
	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
	{
		/* Integer part computing in case Oversampling mode is 8 Samples */
		integerdivider = ((25 * apbclock) / (2 * (USART_BaudRate)));    
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		/* Integer part computing in case Oversampling mode is 16 Samples */
		integerdivider = ((25 * apbclock) / (4 * (USART_BaudRate)));    
	}
	tmpreg = (integerdivider / 100) << 4;
	
	/* Determine the fractional part */
	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
	
	/* Implement the fractional part in the register */
	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
	{
		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
	}
	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
	{
		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	}
	
	/* Write to USART BRR */
	USARTx->BRR = (uint16_t)tmpreg;
}

uint32_t UartDrv_BaudRate_Get(USART_TypeDef* USARTx)
{
	uint32_t Rt;
	uint32_t apbclock;
	uint32_t usartxbase = 0;	
	RCC_ClocksTypeDef RCC_ClocksStatus;
	
	usartxbase = (uint32_t)USARTx;
	
	RCC_GetClocksFreq(&RCC_ClocksStatus);
	if (usartxbase == USART1_BASE)
	{
		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	}
	else
	{
		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
	}	
	Rt = apbclock/USARTx->BRR;
	
	return Rt;
}

//______________________________________________________________________________
//com2 PA2(STM32_TX),PA3(STM32_RX)	WIFI
void WifiCom_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	/* USART configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	
	USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART2, ENABLE);	
	
}

//com3 PB10,PB11 ZIGBEE
void ZigbeeCom_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/* USART configuration */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	
	USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE);	
}

void UartDrv_ComInit(void)
{
	WifiCom_Init();
	ZigbeeCom_Init();
	UartDrv_IrqTxStop();
}

#define WIFI_COM	USART2
#define ZIGBEE_COM	USART3

void UartDrv_ToUsart_Check(void)
{
	uint8_t t8;
	//can to usart check
	if(CanP_Fifo_Rden(CANP_WIFI_FALG_WIFITX))
	{
		if(WIFI_COM->SR & USART_FLAG_TXE)
		{
			t8 = CanP_Fifo_Rdata(CANP_WIFI_FALG_WIFITX);
			WIFI_COM->DR = t8;
		}
	}

	if(CanP_Fifo_Rden(CANP_WIFI_FALG_ZIGBTX))
	{
		if(ZIGBEE_COM->SR & USART_FLAG_TXE)
		{
			t8 = CanP_Fifo_Rdata(CANP_WIFI_FALG_ZIGBTX);
			ZIGBEE_COM->DR = t8;
		}		
	}
}

void USART2_IRQHandler(void)
{
	CanP_Fifo_Wdata(CANP_WIFI_FALG_WIFIRX,USART2->DR);
	USART_ClearITPendingBit(USART2, USART_IT_TXE);
	
}

void USART3_IRQHandler(void)
{
	CanP_Fifo_Wdata(CANP_WIFI_FALG_ZIGBRX,USART3->DR);
	USART_ClearITPendingBit(USART3, USART_IT_TXE);
}

void UartDrv_IrqRxStart(void)
{
	USART_ITConfig(WIFI_COM,USART_IT_RXNE,ENABLE);
	USART_ITConfig(ZIGBEE_COM,USART_IT_RXNE,ENABLE);
}

void UartDrv_IrqTxStop(void)
{
	USART_ITConfig(WIFI_COM,USART_IT_RXNE,DISABLE);
	USART_ITConfig(ZIGBEE_COM,USART_IT_RXNE,DISABLE);
}

//______________________________________________________________________________
//code for car
#if 0

#define  RXD1_MAX_NUM  200
#define  MAX_NUM  40

u8 USART1_RX_BUF[RXD1_MAX_NUM];     //接收缓冲,最大200个字节.  	
u8 USART2_RX_BUF[MAX_NUM];     //接收缓冲,最大8个字节.
//接收状态
//bit7:接收是否完成标志
u8 flag1=0,flag2=0,rx2zt_flag=0;   //串口接收状态;
u8 flag3=0;		 //add by ygm

u8 rxd1_over_flag=0;  // 接收完成标志

u8 USART_RX_STA=0;       // 接收状态标记	 
u8 RX_num1=0,RX_num2=0;     // 接收到的有效字节数目
u8 RX2_MAX=8;			// 定义接收的最长字节
u8 USART1_RXNUM=0;      // 定义串口1接收有效数据长度

u8 rxd1_timer4_flag;
u8 rxd1_timer4_1ms;

void USART1_IRQ_Handler(void)
{			
	
	u8 res;
	
	rxd1_timer4_1ms=0;
	res=USART1->DR;
	
	if(rxd1_timer4_flag==1) //  2ms时间 数据帧重新开始
	{
		rxd1_timer4_flag=0;
		TIM4->CR1|=0x01;     //使能定时器4
		USART1_RXNUM=0;
		USART1_RX_BUF[USART1_RXNUM]=res;
		
	}
	else if(USART1_RXNUM<RXD1_MAX_NUM)	
	{
		USART1_RX_BUF[++USART1_RXNUM]=res;	 // 接收数据帧
	}
	
	else   // 数据超出最大接收数据长度时，舍去不要
	{
		res=0;
	}  
	
	
}  
void USART2_IRQ_Handler(void)
{
	static u8 res,sum;
	
	res=USART2->DR;
	
	if(RX_num2>0)
	{
		USART2_RX_BUF[RX_num2]=res;
		RX_num2++;
	}
	else if (res==0x55)		// 寻找包头
	{
		
		USART2_RX_BUF[0]=res;
		RX_num2=1;
	}
	
	if(RX_num2>=RX2_MAX)
	{
		//		RX_num1=0;
		
		//if((USART2_RX_BUF[RX2_MAX-1]==0xbb)&&(rx2zt_flag==0))	 // 判断包尾
		if((USART2_RX_BUF[RX2_MAX-1]==0xbb)&&(rx2zt_flag==0)&&(USART2_RX_BUF[1]!=0xfd))	 // 判断包尾	//change by ygm
		{									  
			//主指令与三位副指令左求和校验
			//注意：在求和溢出时应该对和做256取余。
			RX_num2=0;	  // 计数清零
			rx2zt_flag=0;  // 接收状态清零
			sum=(USART2_RX_BUF[2]+USART2_RX_BUF[3]+USART2_RX_BUF[4]+USART2_RX_BUF[5])%256;
			if(sum==USART2_RX_BUF[6])
			{
				USART_RX_STA|=0x80;
				flag2=1;	//   指令验证正确，标志位置1
				
				//LED2=0;		//关闭蜂鸣器
			}
			else 
			{
				//LED1=2; 
				flag2=0;
			}
		}
		else if((USART2_RX_BUF[1]==0xfd)&&(rx2zt_flag==0))
		{
			if(USART2_RX_BUF[2]>8)	
			{ 
				RX2_MAX = USART2_RX_BUF[2];
				rx2zt_flag=1;  // 接收状态1  接着接收数据
			}
			else 			 
			{		 
				RX_num2=0;		// 计数清零
				if(USART2_RX_BUF[RX2_MAX-1]==0xbb)
				{
					rx2zt_flag=0;  // 接收状态清零
					//  send_Flag=1;			//change by ygm
					RX2_MAX=8;
					USART_WIFI_JX( USART2_RX_BUF ); // 解析串口数据
				}
			}	  
			
			
		}
		else if((rx2zt_flag==1)&&(USART2_RX_BUF[RX2_MAX-1]==0xbb))
		{
			RX_num2=0;	  // 计数清零
			rx2zt_flag=0;  // 接收状态清零
			//	send_Flag=1;
			RX2_MAX=8;
			USART_WIFI_JX( USART2_RX_BUF ); // 解析串口数据
		}
		else 
		{
			//LED1=2; 
			//flag2=0;
		}		 //接收错误指令，打开蜂鸣器
	}
	
	
} 
void uart1_init(u32 pclk2,u32 bound)
{  	 

}										 
//初始化IO 串口2
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
//CHECK OK
void uart2_init(u32 pclk2,u32 bound)
{  	 

}

void Delay(vu32 nCount) 
{
	for(; nCount != 0; nCount--);
}
int U1SendChar(int ch) 
{
	
	while (!(USART1->SR & USART_FLAG_TXE));
	USART1->DR = (ch & 0x1FF);
	
	return (ch);
}
//发送一个字符
int U2SendChar(int ch) 
{
	
	while (!(USART2->SR & USART_FLAG_TXE));
	USART2->DR = (ch & 0x1FF);
	
	return (ch);
}

int GetKey(void) 
{
	while (!(USART2->SR & USART_FLAG_RXNE));  
	
	return ((int)(USART2->DR & 0x1FF));
}

#endif

//end file

