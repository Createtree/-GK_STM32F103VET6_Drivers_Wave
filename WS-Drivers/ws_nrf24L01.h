
#ifndef _DRIVE_NRF24L01_H_
#define _DRIVE_NRF24L01_H_

#include "ws_main.h"


#define uchar   uint8_t 
#define uint    uint16_t


#define NRF24L01_EN
#define NRF24L01_RXEN
//#define NRF24L01_SEND_EN

//*********NRF24L01_IO�˿ڶ���*****
//   CE��
#define SET_NFR24L01_CE   NRF_CE_GPIO_Port->ODR |=  NRF_CE_Pin;
#define CLR_NFR24L01_CE   NRF_CE_GPIO_Port->ODR &= ~NRF_CE_Pin;

//   CSN��
#define SET_NFR24L01_CSN  NRF_CSN_GPIO_Port->ODR |=  NRF_CSN_Pin;
#define CLR_NFR24L01_CSN  NRF_CSN_GPIO_Port->ODR &= ~NRF_CSN_Pin;

//   �������

#define SET_NFR24L01_MOSI   GPIOB->ODR |=  1<<6;
#define CLR_NFR24L01_MOSI   GPIOB->ODR &=~(1<<6);

//  ��������
#define GET_NFR_MISO HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)
                    
//  ʱ��
#define SET_NFR_SCK   GPIOB->ODR |=  1<<2;
#define CLR_NFR_SCK   GPIOB->ODR &=~(1<<2);



//  ����ʹ��
#ifdef  NRF24L01_EN



#define TX_DS  1<<5
#define MAX_RT 1<<4
#define RX_DR  1<<6


//*********************************************NRF24L01*************************************


//***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define NRF_READ_REG        0x00   // ���Ĵ���ָ��
#define NRF_WRITE_REG       0x20   // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61   // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0   // д��������ָ��
#define FLUSH_TX        0xE1   // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2   // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3   // �����ظ�װ������ָ��
#define NOP             0xFF   // ����
//*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
#define DYNPD           0X1C  // ʹ�ܶ�̬���س���
#define FEATURE         0x1d  //  

//   �õ�������ַ
char *getNrfSelfAdd(void);
//   ���ñ�����ַ
void setNrfSelfAdd(char *p);


/*******************************��*****��*****ģ*****ʽ*****��*****��*************************************/
void nrfTxMode(void);

/*******************************����*****ģ*****ʽ*****��*****��*************************************/
void powerDownModeNrf(void);


/****************************************************************************************************/
//������WS_NRF_Set_Rx_Mode(char *add, uint8_t  len);
//      add:���յ�ַ �� len:���ճ���
//���ܣ����ݽ������� 
/***************************************************************************************************/
void WS_NRF_Set_Rx_Mode(char *add, uint8_t  len);

//  Ѱַ��������
char sendCommandRNF(char *targetAdd, char command, char *dat);

//   ���ͺ���
//   0 : �ɹ��� 1ʧ��
char sendInfoRNF(char *targetAdd, char command, char *dat);

//   NRF��Ⲣ��ȡ����
int  WS_NRF_Receive_Task(void * date);


char getNrfRxDate(void)               ;


#endif





#endif


