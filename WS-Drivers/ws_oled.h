

#ifndef __DRIVE_OLED_H__
#define __DRIVE_OLED_H__	

#include "main.h"
#include "stdlib.h"	


#define WS_Peripheral_OLED 1



/*-----------------OLED IIC�˿ڶ���----------------*/  	


//  ��ʾģʽ
#define OLED_DIS_MODE_5X7  0
#define OLED_DIS_MODE_8X16 1
		     

//  ����OLED��ʾ    
void openOledDis(void);

// �ر�OLED��ʾ     
void closeOledDis(void); 

//��ʼ��SSD1306					    
void OLEDInit(void);

// ȫ��Ϩ��	  
void setOLEDClear(void) ;

// ����д���ݽ���OLED 
char writeOledBuf(char adds , unsigned  char *data  , int len);


//   ��ʼ��
void   WS_OLED_Init(void );

//   �ַ�����ʾ
void WS_OLED_Dis_String(int  x,int   y,char  *string, char disMode);

//   ��ʽ����ʾ
void WS_OLED_Printf(int  x,int  y, char disMode , char *p,... );

//  ��������
void  setOledPoint(int  x, int  y) ;
void writeOledData(unsigned char dat);
//��ʾ 16 * 16 ������Ϣ
void WS_OLED_Dis_16X16_Date(int  x,int   y,char  *date);
//��ʾ 24 * 24 ������Ϣ
void WS_OLED_Dis_24X24_Date(int  x,int   y,char  *date);
//��ʾ 32 * 32 ������Ϣ
void WS_OLED_Dis_32X32_Date(int  x,int   y,char  *date);




#endif  

