
#ifndef _DRIVE_NRF24L01_H_
#define _DRIVE_NRF24L01_H_

#include "ws_main.h"


#define uchar   uint8_t 
#define uint    uint16_t


#define NRF24L01_EN
#define NRF24L01_RXEN
//#define NRF24L01_SEND_EN

//*********NRF24L01_IO端口定义*****
//   CE脚
#define SET_NFR24L01_CE   NRF_CE_GPIO_Port->ODR |=  NRF_CE_Pin;
#define CLR_NFR24L01_CE   NRF_CE_GPIO_Port->ODR &= ~NRF_CE_Pin;

//   CSN脚
#define SET_NFR24L01_CSN  NRF_CSN_GPIO_Port->ODR |=  NRF_CSN_Pin;
#define CLR_NFR24L01_CSN  NRF_CSN_GPIO_Port->ODR &= ~NRF_CSN_Pin;

//   数据输出

#define SET_NFR24L01_MOSI   GPIOB->ODR |=  1<<6;
#define CLR_NFR24L01_MOSI   GPIOB->ODR &=~(1<<6);

//  数据输入
#define GET_NFR_MISO HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)
                    
//  时钟
#define SET_NFR_SCK   GPIOB->ODR |=  1<<2;
#define CLR_NFR_SCK   GPIOB->ODR &=~(1<<2);



//  总体使能
#ifdef  NRF24L01_EN



#define TX_DS  1<<5
#define MAX_RT 1<<4
#define RX_DR  1<<6


//*********************************************NRF24L01*************************************


//***************************************NRF24L01寄存器指令*******************************************************
#define NRF_READ_REG        0x00   // 读寄存器指令
#define NRF_WRITE_REG       0x20   // 写寄存器指令
#define RD_RX_PLOAD     0x61   // 读取接收数据指令
#define WR_TX_PLOAD     0xA0   // 写待发数据指令
#define FLUSH_TX        0xE1   // 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2   // 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3   // 定义重复装载数据指令
#define NOP             0xFF   // 保留
//*************************************SPI(nRF24L01)寄存器地址****************************************************
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
#define DYNPD           0X1C  // 使能动态负载长度
#define FEATURE         0x1d  //  

//   得到本机地址
char *getNrfSelfAdd(void);
//   设置本机地址
void setNrfSelfAdd(char *p);


/*******************************发*****送*****模*****式*****代*****码*************************************/
void nrfTxMode(void);

/*******************************掉电*****模*****式*****代*****码*************************************/
void powerDownModeNrf(void);


/****************************************************************************************************/
//函数：WS_NRF_Set_Rx_Mode(char *add, uint8_t  len);
//      add:接收地址 ， len:接收长度
//功能：数据接收配置 
/***************************************************************************************************/
void WS_NRF_Set_Rx_Mode(char *add, uint8_t  len);

//  寻址发送命令
char sendCommandRNF(char *targetAdd, char command, char *dat);

//   发送函数
//   0 : 成功， 1失败
char sendInfoRNF(char *targetAdd, char command, char *dat);

//   NRF检测并读取数据
int  WS_NRF_Receive_Task(void * date);


char getNrfRxDate(void)               ;


#endif





#endif


