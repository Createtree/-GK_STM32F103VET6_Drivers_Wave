
#ifndef __UART_DRV_H__
#define __UART_DRV_H__

#ifdef __UART_DRV_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL void KL_Init(void);
GLOBAL uint8_t KL_GetKeyState(void);

#undef GLOBAL

#endif //__UART_DRV_H__