

#ifndef __DRIVE_OLED_H__
#define __DRIVE_OLED_H__	

#include "main.h"
#include "stdlib.h"	


#define WS_Peripheral_OLED 1



/*-----------------OLED IIC端口定义----------------*/  	


//  显示模式
#define OLED_DIS_MODE_5X7  0
#define OLED_DIS_MODE_8X16 1
		     

//  开启OLED显示    
void openOledDis(void);

// 关闭OLED显示     
void closeOledDis(void); 

//初始化SSD1306					    
void OLEDInit(void);

// 全屏熄灭	  
void setOLEDClear(void) ;

// 连续写数据进入OLED 
char writeOledBuf(char adds , unsigned  char *data  , int len);


//   初始化
void   WS_OLED_Init(void );

//   字符串显示
void WS_OLED_Dis_String(int  x,int   y,char  *string, char disMode);

//   格式化显示
void WS_OLED_Printf(int  x,int  y, char disMode , char *p,... );

//  设置坐标
void  setOledPoint(int  x, int  y) ;
void writeOledData(unsigned char dat);
//显示 16 * 16 点阵信息
void WS_OLED_Dis_16X16_Date(int  x,int   y,char  *date);
//显示 24 * 24 点阵信息
void WS_OLED_Dis_24X24_Date(int  x,int   y,char  *date);
//显示 32 * 32 点阵信息
void WS_OLED_Dis_32X32_Date(int  x,int   y,char  *date);




#endif  

