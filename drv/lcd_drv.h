
#ifndef __LCD_DRV_H__
#define __LCD_DRV_H__


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
//#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
#endif //__COLOR_DEF__

 void Lcd_Init(void);
 void Lcd_FillAll(u16 c);
 void Lcd_Test(void);

 void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

 void DisplayChar_6x12(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);

 void DisplayChar_8x16(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
 void DisplayChar_16x16(u8 *p, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
 void DisplayChar_spizk_16x16(u8 *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);

 void LCD_WinRead(u16 sx,u16 sy,u16 ex,u16 ey,u16 *buf);
 void LCD_WinWrite(u16 sx,u16 sy,u16 ex,u16 ey,u16 *buf);

 void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 c);

 void DisplayChar_12x24(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
 void DisplayChar_spizk_24x24(uint8_t *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);

 void DisplayChar_spizk_16x32(char c, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
 void Displayhz_spizk_32x32(uint8_t *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);

 void DispStr_Allsize(uint16_t hsize,
							uint8_t *str,
							uint8_t zc,
							uint32_t ml,
							uint16_t x,
							uint16_t y,
							uint16_t bk,
							uint16_t ft);

void LCD_DrawArc(uint16_t x0,uint16_t y0,uint16_t r,uint16_t c);



#endif //__LCD_DRV_H__

