

#include "ws_ili9xxx.h"
#include "drive_dzcode.h"

      

__IO uint16_t *FSMC_TFT_Dat_Addr;       //  写数据地址
__IO uint16_t *FSMC_TFT_Cmd_Addr;       //  写命令地址


#define WS_TFT_Write_Cmd(d)	      (*FSMC_TFT_Cmd_Addr) = (d)		
#define WS_TFT_Write_Dat(d)	      (*FSMC_TFT_Dat_Addr) = (d)		
#define WS_TFT_Write_Cmd_Dat(r,d) (*FSMC_TFT_Cmd_Addr) = (r),(*FSMC_TFT_Dat_Addr) = (d)



//   设置TFT对应的内存地址
void WS_TFT_FSMC_Addr_Init(void)
{                               // 16bit位宽 ， LCD_RS->A23(NE1) , 对应第24个地址位    
	  FSMC_TFT_Dat_Addr = (uint16_t *)(((uint32_t)(0x60000000 | 1<<24)));
	  FSMC_TFT_Cmd_Addr = (uint16_t *)(((uint32_t)(0x60000000 | 1<<24))-2);
}


//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			  //LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				  //LCD ID
	uint8_t  dir;			    //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

_lcd_dev lcddev;

#define USE_HORIZONTAL LCD_DIR_MODE   	//	0//1

void WS_TFT_Set_Param(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;			
	WS_TFT_Write_Cmd_Dat(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;	
	WS_TFT_Write_Cmd_Dat(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
#endif
}



//   按照指定颜色清屏
void WS_TFT_Clear(uint16_t color)
{	
	uint32_t index=0;    
	WS_TFT_Set_Windows(0,0,lcddev.width-1,lcddev.height-1);	
	for(index=0;index<LCD9486_XMAX * LCD9486_YMAX ;index++)
	{
		WS_TFT_Write_Dat(color);		  
	}
} 


void WS_TFT_Init(void) 
{  										 
	WS_TFT_FSMC_Addr_Init();
//*************   2.8inch ILI9341   **********//	
	WS_TFT_Write_Cmd(0xCF);  
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0xC9); //C1 
	WS_TFT_Write_Dat(0X30); 
	WS_TFT_Write_Cmd(0xED) ;  
	WS_TFT_Write_Dat(0x64); 
	WS_TFT_Write_Dat(0x03); 
	WS_TFT_Write_Dat(0X12); 
	WS_TFT_Write_Dat(0X81); 
	WS_TFT_Write_Cmd(0xE8);  
	WS_TFT_Write_Dat(0x85); 
	WS_TFT_Write_Dat(0x10); 
	WS_TFT_Write_Dat(0x7A); 
	WS_TFT_Write_Cmd(0xCB);  
	WS_TFT_Write_Dat(0x39); 
	WS_TFT_Write_Dat(0x2C); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x34); 
	WS_TFT_Write_Dat(0x02); 
	WS_TFT_Write_Cmd(0xF7);  
	WS_TFT_Write_Dat(0x20); 
	WS_TFT_Write_Cmd(0xEA);  
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Cmd(0xC0);    //Power control 
	WS_TFT_Write_Dat(0x1B);   //VRH[5:0] 
	WS_TFT_Write_Cmd(0xC1);    //Power control 
	WS_TFT_Write_Dat(0x00);   //SAP[2:0];BT[3:0] 01 
	WS_TFT_Write_Cmd(0xC5);    //VCM control 
	WS_TFT_Write_Dat(0x30); 	 //3F
	WS_TFT_Write_Dat(0x30); 	 //3C
	WS_TFT_Write_Cmd(0xC7);    //VCM control2 
	WS_TFT_Write_Dat(0XB7); 
	WS_TFT_Write_Cmd(0x36);    // Memory Access Control 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Cmd(0x3A);   
	WS_TFT_Write_Dat(0x55); 
	WS_TFT_Write_Cmd(0xB1);   
	WS_TFT_Write_Dat(0x00);   
	WS_TFT_Write_Dat(0x1A); 
	WS_TFT_Write_Cmd(0xB6);    // Display Function Control 
	WS_TFT_Write_Dat(0x0A); 
	WS_TFT_Write_Dat(0xA2); 
	WS_TFT_Write_Cmd(0xF2);    // 3Gamma Function Disable 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Cmd(0x26);    //Gamma curve selected 
	WS_TFT_Write_Dat(0x01); 
	WS_TFT_Write_Cmd(0xE0);     //Set Gamma 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x2A); 
	WS_TFT_Write_Dat(0x28); 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Dat(0x0E); 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Dat(0x54); 
	WS_TFT_Write_Dat(0XA9); 
	WS_TFT_Write_Dat(0x43); 
	WS_TFT_Write_Dat(0x0A); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 		 
	WS_TFT_Write_Cmd(0XE1);    //Set Gamma 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x15); 
	WS_TFT_Write_Dat(0x17); 
	WS_TFT_Write_Dat(0x07); 
	WS_TFT_Write_Dat(0x11); 
	WS_TFT_Write_Dat(0x06); 
	WS_TFT_Write_Dat(0x2B); 
	WS_TFT_Write_Dat(0x56); 
	WS_TFT_Write_Dat(0x3C); 
	WS_TFT_Write_Dat(0x05); 
	WS_TFT_Write_Dat(0x10); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x3F); 
	WS_TFT_Write_Dat(0x3F); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Cmd(0x2B); 
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x01);
	WS_TFT_Write_Dat(0x3f);
	WS_TFT_Write_Cmd(0x2A); 
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0xef);	 
	WS_TFT_Write_Cmd(0x11);  //Exit Sleep
	WS_Delay_MS(120);
	WS_TFT_Write_Cmd(0x29);  //display on	

  
	WS_TFT_Set_Param();// 

	//   打开背光 
	LCD9486_LIGHT_OPEN   ;
	
	
	//   全屏幕刷色测试 ，R,G,B 
	WS_TFT_Clear(RED)    ;
	WS_Delay_MS(200)     ;
	WS_TFT_Clear(GREEN)  ;
	WS_Delay_MS(200)     ;
	WS_TFT_Clear(BLUE)   ;
	WS_Delay_MS(200)     ;
	WS_TFT_Clear(WHITE)  ;
	
	//   字符显示测试
  WS_TFT_Dis_8X16_String (0,0,  "2020/11/14 09:08:20 ",WHITE, RED);
	WS_TFT_Dis_5X7_String  (160,0,  "TFT 5x7 ASCII",WHITE, RED);
	WS_TFT_Dis_5X7_String  (160,8,  "TFT 5x7 ASCII",BLACK, WHITE);
	WS_TFT_Dis_8X16_String (0,16, "16x16 font:字库测试",WHITE,GREEN) ;	
	WS_TFT_Dis_12X24_String(0,32,"24x24 font:字库测试",WHITE,BLUE);
	WS_TFT_Dis_16X32_String(0,54,"32x32 font:字库测试",WHITE,GRAY);
	
	
	int i;
	for(i= 20; i< 220;i++) //  打点划线
	{
	   WS_TFT_Draw_Point(i,315,RED);
	}
	
	//  画矩形
	WS_TFT_Draw_Rectangle(10,290,230,310,RED);
}


//   选择需要更新的区域
void WS_TFT_Set_Windows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	WS_TFT_Write_Cmd(lcddev.setxcmd);	
	WS_TFT_Write_Dat(xStar>>8);
	WS_TFT_Write_Dat(0x00FF&xStar);		
	WS_TFT_Write_Dat(xEnd>>8);
	WS_TFT_Write_Dat(0x00FF&xEnd);

	WS_TFT_Write_Cmd(lcddev.setycmd);	
	WS_TFT_Write_Dat(yStar>>8);
	WS_TFT_Write_Dat(0x00FF&yStar);		
	WS_TFT_Write_Dat(yEnd>>8);
	WS_TFT_Write_Dat(0x00FF&yEnd);

	//   开始跟新选择区域内的GRAM
	WS_TFT_Write_Cmd(lcddev.wramcmd);	
}  


//   画点
void WS_TFT_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	//  设置区域
	WS_TFT_Set_Windows(x,y,x+1,y+1);
	//   写入数据
	WS_TFT_Write_Dat(color)         ; 
}

//   画矩形
void WS_TFT_Draw_Rectangle(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd,uint16_t color)
{
	uint16_t       i = 0;
	//  设置区域
	WS_TFT_Set_Windows(xStar,yStar,xEnd,yEnd);
	//   写入数据
	for(;yStar < yEnd ; yStar ++   )
	{
	  for(i = xStar; i <= xEnd; i ++ )
	     WS_TFT_Write_Dat(color)    ;  
	}
}

/**
 * @brief		带颜色画线函数(直线、斜线)
 * @param   x1,y1	起点坐标
 * @param   x2,y2	终点坐标
 * @return  none
 */
void WS_TFT_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
//  uint16_t	i = 0;
	int16_t		delta_x = 0, delta_y = 0;
	int8_t		incx = 0, incy = 0;
	uint16_t	distance = 0;
	uint16_t  t = 0;
	uint16_t	x = 0, y = 0;
	uint16_t 	x_temp = 0, y_temp = 0;
	
	if(y1 == y2)
	{
			/* 快速画水平线 */
			WS_TFT_Draw_Rectangle(x1,y1,x2,y2+1,color) ;
			return;
	}
	else
	{
		/* 画斜线（Bresenham算法） */
		/* 计算两点之间在x和y方向的间距，得到画笔在x和y方向的步进值 */
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if(delta_x > 0)
		{
			//斜线(从左到右)
			incx = 1;
		}
		else if(delta_x == 0)
		{
			//垂直斜线(竖线)
			incx = 0;
		}
		else
		{
			//斜线(从右到左)
			incx = -1;
			delta_x = -delta_x;
		}
		if(delta_y > 0)
		{
			//斜线(从左到右)
			incy = 1;
		}
		else if(delta_y == 0)
		{
			//水平斜线(水平线)
			incy = 0;
		}
		else
		{
			//斜线(从右到左)
			incy = -1;
			delta_y = -delta_y;
		}			
		
		/* 计算画笔打点距离(取两个间距中的最大值) */
		if(delta_x > delta_y)
		{
			distance = delta_x;
		}
		else
		{
			distance = delta_y;
		}
		
		/* 开始打点 */
		x = x1;
		y = y1;
		//第一个点无效，所以t的次数加一
		for(t = 0; t <= distance + 1;t++)
		{
			WS_TFT_Draw_Point(x, y, color);
		
			/* 判断离实际值最近的像素点 */
			x_temp += delta_x;	
			if(x_temp > distance)
			{
				//x方向越界，减去距离值，为下一次检测做准备
				x_temp -= distance;		
				//在x方向递增打点
				x += incx;
					
			}
			y_temp += delta_y;
			if(y_temp > distance)
			{
				//y方向越界，减去距离值，为下一次检测做准备
				y_temp -= distance;
				//在y方向递增打点
				y += incy;
			}
		}
	}
}


//   刷16*16点区域
void WS_TFT_Dis_16X16_Date (int x, int y, uint8_t  *buffer , uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	
	     uint16_t   dat = 0;
       WS_TFT_Set_Windows(x,y,x+15,y+15);	
			 for(i=0;i<32;i+=2)
			 {
					dat   = buffer[i] <<8 ;
				  dat  |= buffer[i+1] ;
					for(j=15;j>=0;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
					
			 } 
}

//   刷24*24点区域
void WS_TFT_Dis_24X24_Date (int x, int y, uint8_t  *buffer , uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	     uint8_t   dat = 0;
       WS_TFT_Set_Windows(x,y,x+23,y+23);	
			 for(i=0;i<72;i++)
			 {
					dat   = buffer[i]  ;
					for(j=7;j>=0;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}	
			 } 
}

//   刷32*32点区域
void WS_TFT_Dis_32X32_Date (int x, int y, uint8_t  *buffer , uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	     uint8_t   dat = 0;
       WS_TFT_Set_Windows(x,y,x+31,y+31);	
			 for(i=0;i<128;i++)
			 {
					dat   = buffer[i]  ;
					for(j=7;j>=0;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}	
			 } 
}


//   
void WS_TFT_Dis_5X7_String (int x,int y,char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	int  i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat ;
	for(idex=0;idex<len;idex++)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+5,y+7);	
       p_dat = (uint8_t *)(string8X8Code + (string[idex] - 0x20)*8);	
			 for(i=0;i<8;i++)
			 {
					dat = p_dat[i]  ;
					for(j=7;j>=2;j--)
					{
							if((dat & (1<<j))  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
      		
		}
	  else  //  
		{
		   idex ++;   
		}
    x +=  6;	
		if(x  >=  LCD9486_XMAX - 6) return;    //
	}
}


void WS_TFT_Dis_8X16_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	uint16_t i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat , buffer[32] ;
	
	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  // 判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+7,y+15);	
       p_dat = (uint8_t *)(string8X16Code + (string[idex] - 0x20)*16);	
			 for(i=0;i<16;i++)
			 {
					dat = p_dat[i]  ;
					for(j=0;j<8;j++)
					{
							if((dat & 1<<(7-j))  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  8;
       idex ++;			 
		}
	  else  // 
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font16_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_16X16_Date(x,y,buffer,backColor,forColor);
						  x +=  16;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  LCD9486_XMAX - 8) return;      
	}
}

void WS_TFT_Dis_12X24_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	uint16_t i , j, dat ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat ,  buffer[72] ;

	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+11,y+23);	
       p_dat = (uint8_t *)(ascii12X24Code + (string[idex] - 0x20)*48);	
			 for(i=0;i<48;i+=2)
			 {
					dat  = p_dat[i]<<8  ;
				  dat |= p_dat[i+1]   ;
					for(j=15;j>=4;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  12;
       idex ++;			 
		}
	  else  //  
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font24_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_24X24_Date(x,y,buffer,backColor	,forColor);
						  x +=  24;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  LCD9486_XMAX - 12) return;      //???????????????????????		
	}
}


void WS_TFT_Dis_16X32_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
  int	i , j ;
	uint16_t  dat ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat ,  buffer[128] ;

	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+15,y+31);	
       p_dat = (uint8_t *)(ascii16X32Code + (string[idex] - 0x20)*64);	
			 for(i=0;i<64;i+=2)
			 {
					dat  = p_dat[i]<<8  ;
				  dat |= p_dat[i+1]   ;
					for(j=15;j>=0;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  16;
       idex ++;			 
		}
	  else  // 
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font32_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_32X32_Date(x,y,buffer,backColor	,forColor);
						  x +=  32;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  LCD9486_XMAX - 12) return;      //???????????????????????		
	}
}



//void LCD9486_SetWindows_ForRead(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
//{	
//	WS_TFT_Write_Cmd(lcddev.setxcmd);	
//	WS_TFT_Write_Dat(xStar>>8);
//	WS_TFT_Write_Dat(0x00FF&xStar);		
//	WS_TFT_Write_Dat(xEnd>>8);
//	WS_TFT_Write_Dat(0x00FF&xEnd);

//	WS_TFT_Write_Cmd(lcddev.setycmd);	
//	WS_TFT_Write_Dat(yStar>>8);
//	WS_TFT_Write_Dat(0x00FF&yStar);		
//	WS_TFT_Write_Dat(yEnd>>8);
//	WS_TFT_Write_Dat(0x00FF&yEnd);

//	WS_TFT_Write_Cmd(0x2e);	
//	xStar = (*FSMC_TFT_Dat_Addr);	
//} 



