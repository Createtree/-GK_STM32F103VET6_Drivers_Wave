
#ifndef __UART_DRV_H__
#define __UART_DRV_H__

#ifdef __UART_DRV_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

#define CONST_OVERMAX 0x7ffffff0

typedef struct _Uart_Buf_Struct
{
	u8 *tx_buf;
	u32 tx_size;
	u32 tx_leng;
	u32 tx_curr;
	u32 tx_overs;
	
	u8 *rx_buf;
	u32 rx_size;
	u32 rx_leng;
	u32 rx_overs;
	u32 rx_times;
	
	u8 uart_port;

	u8 run_flag;
	
}Uart_Buf_Struct;

GLOBAL Uart_Buf_Struct uart_struct[2];

GLOBAL void Uart_Struct_Clear(Uart_Buf_Struct *p);
GLOBAL void Uart_Init(void);

GLOBAL void Uart_Irq(Uart_Buf_Struct *p);
GLOBAL void UartTx_Clear(Uart_Buf_Struct *p);
GLOBAL void UartTx_AddStr(u8 *b,u8 ml,Uart_Buf_Struct *p);
GLOBAL void UartTx_AddChar(u8 d,Uart_Buf_Struct *p);
GLOBAL void UartTx_Start(Uart_Buf_Struct *p);
GLOBAL   u8 UartTx_EndCheck(Uart_Buf_Struct *p);

GLOBAL void UartRx_Clear(Uart_Buf_Struct *p);
//GLOBAL  u32 UartRx_GetLen(Uart_Buf_Struct *p);
GLOBAL  u32 UartRx_GetData(u8 *pr,u16 ml,Uart_Buf_Struct *p);
//GLOBAL   u8 UartRx_GetChar(u32 offset,Uart_Buf_Struct *p);
GLOBAL void UartRx_TimeCb(void);
//GLOBAL  u32 UartRx_GetLostTime(Uart_Buf_Struct *p);

#define UartRx_GetBuf(p)	(p)->rx_buf
#define UartRx_GetLen(p)	(p)->rx_leng
#define UartRx_GetChar(o,p)	(p)->rx_buf[o]
#define UartRx_GetLostTime(p)	(p)->rx_times
#define UartRx_GetBufSize(p)	(p)->rx_size

#define UartTx_GetBuf(p)	(p)->tx_buf
#define UartTx_GetLen(p)	(p)->tx_leng

GLOBAL void USART_Config(void);

GLOBAL void UartDrv_BaudRate_Set(USART_TypeDef* USARTx, uint32_t USART_BaudRate);
GLOBAL uint32_t UartDrv_BaudRate_Get(USART_TypeDef* USARTx);

//______________________________________________________________________________

GLOBAL void UartDrv_ComInit(void);
GLOBAL void UartDrv_ToUsart_Check(void);
GLOBAL void UartDrv_IrqRxStart(void);
GLOBAL void UartDrv_IrqTxStop(void);





//end file





















#undef GLOBAL
#endif //__UART_DRV_H__





