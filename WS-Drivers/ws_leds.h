#ifndef __WS_LEDS_H___
#define __WS_LEDS_H___


#include "ws_main.h"


//  扫描行列
#define leds_Row 32      //  点阵行数量
#define leds_Column 8    //  点阵列数量，8列构成一个字节，实际定义字节数，64列定义长度为8




//   屏幕初始化
void ledsInit(void);


//   刷新函数
void WS_Leds_Updata_Buffer(void);

//显示8X16号字体的字符串，兼容汉字显示
void WS_Leds_Dis_8X16_String(int x,int y,char  *p_string );
//显示5X7 号字体的字符串
void WS_Leds_Dis_5X7_String (int x,int y,char  *p_string) ;
//显示7X8 号字体的字符串
void WS_Leds_Dis_7X8_String (int x,int y,char  *p_string);

//清屏
void WS_Leds_Dis_Clean(void);
//全屏点亮
void WS_Leds_Dis_Full(void) ;



//   设置显示行
void setLedsLine(char n);

//  显示关闭
void closeLedsDis(void)	;

//  显示打开
void openLedsDis(void) ;



//  清除显示
void cleanLedsDis(void);

//  全屏显示
void fullLedsDis(void);

void WS_Leds_Set_Point(int x,int y,char stu);

void setLedsByte(int x,int y,char dat)    ;


//   显示16*16点阵
void dis16X16Data(int x,int y,char *p);
//   显示8*8点阵
void dis8X8Data(int x,int y,char *p);

//  显示8*16数字
void dis8X16String(int x,int y,char *p);
//  显示8*16数字
void dis8X16Number(int x,int y,int n);


//  显示8*8字符
void dis8X8String(int x,int y,char *p);
//  显示8数字
void dis8Number(int x,int y,int n);

//  显示16*16点阵
void dis16X16String(int x,int y,char *p,char n);

//  显示16*32字符
void dis16X32String(int x,char *p);







//  显示秒函数
void dis16Sec(int sec);

//  小数点闪烁
void openDPToClose(void);

//  显示小数点
int openDp(void) ;

//  关闭小数点
int closeDp(void);

//  显示8*8字符
void dis8NumberString(char x,char y,char *p);

//   关闭某个字符显示
void close8Char(char x,char y);

//   设置列状态
void WS_Leds_Set_Line(int x,int y,char stu );

#endif
