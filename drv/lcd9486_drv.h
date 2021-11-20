
#ifndef __LCD9486_DRV_H__
#define __LCD9486_DRV_H__

#include "ws_main.h"



#ifndef __COLOR_DEF__
#define __COLOR_DEF__

//������ɫ
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


#endif 




#define LCD_DIR_MODE	       0           //0��������1��������	

#define LCD9486_RST_CLR	   LCD_RESET_GPIO_Port->ODR &= ~LCD_RESET_Pin ;  
#define LCD9486_RST_SET	   LCD_RESET_GPIO_Port->ODR |=  LCD_RESET_Pin ; 

#define LCD9486_LIGHT_CLOSE	 LCD_Light_GPIO_Port->ODR &= ~LCD_Light_Pin  ; 
#define   LCD9486_LIGHT_OPEN LCD_Light_GPIO_Port->ODR |=  LCD_Light_Pin  ;


#if LCD_DIR_MODE == 0
	  #define LCD9486_XMAX	240
	  #define LCD9486_YMAX	320
#else
		#define LCD9486_XMAX	320
		#define LCD9486_YMAX	240
#endif



 void LCD9486_GPIOInit(void);
 void LCD9486_Init(void);
 void LCD9486_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
 void LCD9486_SetWindows_ForRead(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
 void LCD9486_Clear(uint16_t Color);

 void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t c);
 
 
 void WS_TFT_Dis_8X8_String (int x,int y,char  *string , uint16_t color);
  void WS_TFT_Dis_8X16_String (int x,int y,char  *string , uint16_t color);


 void Back_Led_Init(void);
 void Back_Led_Con(uint8_t p);



#endif

