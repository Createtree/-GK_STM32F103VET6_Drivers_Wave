#ifndef __WS_LEDS_H___
#define __WS_LEDS_H___


#include "ws_main.h"


//  ɨ������
#define leds_Row 32      //  ����������
#define leds_Column 8    //  ������������8�й���һ���ֽڣ�ʵ�ʶ����ֽ�����64�ж��峤��Ϊ8




//   ��Ļ��ʼ��
void ledsInit(void);


//   ˢ�º���
void WS_Leds_Updata_Buffer(void);

//��ʾ8X16��������ַ��������ݺ�����ʾ
void WS_Leds_Dis_8X16_String(int x,int y,char  *p_string );
//��ʾ5X7 ��������ַ���
void WS_Leds_Dis_5X7_String (int x,int y,char  *p_string) ;
//��ʾ7X8 ��������ַ���
void WS_Leds_Dis_7X8_String (int x,int y,char  *p_string);

//����
void WS_Leds_Dis_Clean(void);
//ȫ������
void WS_Leds_Dis_Full(void) ;



//   ������ʾ��
void setLedsLine(char n);

//  ��ʾ�ر�
void closeLedsDis(void)	;

//  ��ʾ��
void openLedsDis(void) ;



//  �����ʾ
void cleanLedsDis(void);

//  ȫ����ʾ
void fullLedsDis(void);

void WS_Leds_Set_Point(int x,int y,char stu);

void setLedsByte(int x,int y,char dat)    ;


//   ��ʾ16*16����
void dis16X16Data(int x,int y,char *p);
//   ��ʾ8*8����
void dis8X8Data(int x,int y,char *p);

//  ��ʾ8*16����
void dis8X16String(int x,int y,char *p);
//  ��ʾ8*16����
void dis8X16Number(int x,int y,int n);


//  ��ʾ8*8�ַ�
void dis8X8String(int x,int y,char *p);
//  ��ʾ8����
void dis8Number(int x,int y,int n);

//  ��ʾ16*16����
void dis16X16String(int x,int y,char *p,char n);

//  ��ʾ16*32�ַ�
void dis16X32String(int x,char *p);







//  ��ʾ�뺯��
void dis16Sec(int sec);

//  С������˸
void openDPToClose(void);

//  ��ʾС����
int openDp(void) ;

//  �ر�С����
int closeDp(void);

//  ��ʾ8*8�ַ�
void dis8NumberString(char x,char y,char *p);

//   �ر�ĳ���ַ���ʾ
void close8Char(char x,char y);

//   ������״̬
void WS_Leds_Set_Line(int x,int y,char stu );

#endif
