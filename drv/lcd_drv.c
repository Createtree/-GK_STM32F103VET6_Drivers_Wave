

#include "lcd_drv.h"
#include <math.h>


#include "asc_font.h"
#include "song_16x16.h"





#define LCD_SetWindows	LCD9486_SetWindows
#define p_lcd_ram		p_LCD9486_Ram
#define p_lcd_reg		p_LCD9486_Reg

#define LcdData			p_LCD9486_Ram

void Lcd_Init(void)
{
	LCD9486_Init();
	
}

void Lcd_FillAll(u16 c)
{
	LCD9486_Clear(c);
}


void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	

	u32 i;				
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	u32 totalpoint=height*width;
	LCD_SetWindows(sx,sy,ex-1,ey-1);//设置显示窗口


	for(i=0;i<totalpoint;i++)
	{
		p_lcd_ram[0]=color;	//写入数据 	 
	}

	//LCD_SetWindows(0,0,LCD9486_XMAX,LCD9486_YMAX);//恢复窗口设置为全屏
}

void LCD_WinRead(u16 sx,u16 sy,u16 ex,u16 ey,u16 *buf)
{
	u32 i;				
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	u32 totalpoint=height*width;
	LCD9486_SetWindows_ForRead(sx,sy,ex-1,ey-1);//设置显示窗口


	for(i=0;i<totalpoint;i++)
	{
		buf[i] = p_lcd_ram[0];		//写入数据 	 
	}	
}

void LCD_WinWrite(u16 sx,u16 sy,u16 ex,u16 ey,u16 *buf)
{
	u32 i;				
	u16 width=ex-sx+1; 		//得到填充的宽度
	u16 height=ey-sy+1;		//高度
	u32 totalpoint=height*width;
	LCD_SetWindows(sx,sy,ex-1,ey-1);//设置显示窗口


	for(i=0;i<totalpoint;i++)
	{
		p_lcd_ram[0]=buf[i];	//写入数据 	 
	}	
}


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 c)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,c);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

static u16 tCour[]={
	0xf800,
	0x07e0,
	0x001f,
	0xffff,
	0x0000,
};

void Lcd_testColour(void)
{
	static u8 co = 0;
	u16 c;
	//u32 i;
	
	if(++co>=sizeof(tCour)/sizeof(tCour[0]))
		co = 0;
	c = tCour[co];
	

	LCD9486_Clear(c);
	
	
}

#define WriteData(d)	(*p_LCD9486_Ram) = (d)
#define BITMODE_8BIT 0
#define LcdSetArea		LCD9486_SetWindows

u16 RGB_Colour(u8 R,u8 G,u8 B)  //通过RGB返回一个565的颜色值
{
    u16 Rt;
    Rt = (((u16)R<<8)&0xf800)|(((u16)G<<3)&0x07e0)|(((u16)B>>3)&0x001f);
    return Rt;
}

void Lcd_testColour3(void)
{
	//register u32 size;
	u16 x1,y1,x2,y2,lx,ly,i,j,c;
	//u8 t;



	x1 = 30;
	lx = 30;

	y1 = 32;	//112;
	ly = 255;

	x2 = x1+lx-1;
	y2 = y1+ly-1;


	LCD9486_SetWindows(x1,y1,x2,y2);

	for(j = 0;j<ly;j++)
	{
		c = RGB_Colour(j,0,0);
		//t = Colour_R(j);
		//c = ((u16)t<<8)&0xf800;
		for(i = 0;i<lx;i++)
		{
#if BITMODE_8BIT
			WriteData(j);
			WriteData(0);
			WriteData(0);
#else
			WriteData(c);
#endif
		}
	}

	x1 += lx;
	x2 = x1+lx-1;
	y2 = y1+ly-1;

	LcdSetArea(x1,y1,x2,y2);
	//WriteCommand(0x2C);

	for(j = 0;j<ly;j++)
	{
		c = RGB_Colour(0,j,0);
		for(i = 0;i<lx;i++)
		{
#if BITMODE_8BIT
			WriteData(j);
			WriteData(0);
			WriteData(0);
#else
			WriteData(c);
#endif
		}
	}
	x1 += lx;
	x2 = x1+lx-1;
	y2 = y1+ly-1;

	LcdSetArea(x1,y1,x2,y2);
	//WriteCommand(0x2C);
	for(j = 0;j<ly;j++)
	{
		c = RGB_Colour(0,0,j);
		for(i = 0;i<lx;i++)
		{
#if BITMODE_8BIT
			WriteData(j);
			WriteData(0);
			WriteData(0);
#else
			WriteData(c);
#endif
		}
	}

	x1 += lx;
	x2 = x1+lx-1;
	y2 = y1+ly-1;

	LcdSetArea(x1,y1,x2,y2);
	//WriteCommand(0x2C);
	for(j = 0;j<ly;j++)
	{
		c = RGB_Colour(j,j,j);
		for(i = 0;i<lx;i++)
		{
#if BITMODE_8BIT
			WriteData(j);
			WriteData(0);
			WriteData(0);
#else
			WriteData(c);
#endif
		}
	}

}


void DisplayChar_8x16(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  //显示任意字符
{
	u16 i,j;
	u8 *Font_Addr ;//= (u8 *)(0x08002800+0x70000 + ((Char)<<4));
	u8 ft = 0;	
	LCD9486_SetWindows(xp,yp,xp+7,yp+15);
	if(Char>=0x20)
		Char-=0x20;
	Font_Addr = (u8 *)&song_8x16[Char*16];
	
	for(j=0; j<16; j++)
	{
		ft= *Font_Addr++;
		for(i=0; i<8; i++)
		{
			if(ft & 0x80)     //get the word dot,if true,display 
				//Draw_Pixel(xp+i, yp+j, FT_Color);
				LcdData[0] = FT_Color;
			else LcdData[0] = BK_Color;
			ft <<= 1;
		}
	}	
}


void DisplayChar_6x12(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  //显示任意字符
{
	u16 i,j;
	u8 *Font_Addr ;//= (u8 *)(0x08002800+0x70000 + ((Char)<<4));
	u8 ft = 0;	
	LCD9486_SetWindows(xp,yp,xp+5,yp+11);
	if(Char>=0x20)
		Char-=0x20;
	Font_Addr = (u8 *)&asc2_1206[Char];//*12];
	
	for(j=0; j<12; j++)
	{
		ft= *Font_Addr++;
		for(i=0; i<6; i++)
		{
			if(ft & 0x01)     //get the word dot,if true,display 
				//Draw_Pixel(xp+i, yp+j, FT_Color);
				LcdData[0] = FT_Color;
			else LcdData[0] = BK_Color;
			ft >>= 1;
		}
	}	
}


u8 *FU_Get16x16Dot(u8 *hz)
{
	u32 i;
	for(i = 0;i<sizeof(song_16x16_str)/3;i++)
	{
		if(MLib_memcmp(hz,(uint8_t *)song_16x16_str+(i*3),3) == 0)
		{
			return (u8 *)song_16x16_dot+(i*32);
		}
	}
	return 0;
}

u8 *FU_Get24x24Dot(u8 *hz)
{
	u32 i;
	for(i = 0;i<sizeof(song_16x16_str)/2;i++)
	{
		if(MLib_memcmp(hz,(uint8_t *)song_16x16_str+(i*2),2) == 0)
		{
			return (u8 *)song_24x24_dot+(i*(24*3));
		}
	}
	return 0;
}

u8 shang[]={
	0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x03,0xF8,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0xFF,0xFE,0x00,0x00
};

void DisplayChar_16x16(u8 *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u16 i,j;
	u8 *Font_Addr ;//= (u8 *)(0x08002800+0x70000 + ((Char)<<4));
	u8 ft = 0;	
	LCD9486_SetWindows(xp,yp,xp+15,yp+15);
	//Font_Addr = (u8 *)&song_8x16[Char*16];
	
	Font_Addr = FU_Get16x16Dot(hz);
	//memcpy(hztbuf,shang,32);
	
	for(j=0; j<32; j++)
	{
		ft = Font_Addr[j];
		for(i=0; i<8; i++)
		{
			if(ft & 0x80)
				LcdData[0] = FT_Color;
			else 
				LcdData[0] = BK_Color;
			ft <<= 1;
		}
	}	
}


u8 FU_spizk_Get16x16Dot(u8 *buf,u8 *phz)
{
	u32 t;
	u32 hz = U8ToU16_Big(phz);
	t = ((hz>>8)*0xa2) + 0x3bbf;
	if(hz >= t)
	{
		W25Q32_Data_Read(buf,32,0x30000+(hz-t)*32);
		return 1;
	}
	return 0;
}

void DisplayChar_spizk_16x16(uint8_t *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u8 i,j,s;
	u8 *p;
	static uint8_t buf[32];
	FU_spizk_Get16x16Dot(buf,hz);
	LCD9486_SetWindows(xp,yp,xp+15,yp+15);
	
	p = buf;
	for(s = 0;s <2;s++)
	{
		for(j=0x80; j; j>>=1)
		{
			for(i=0; i<16; i++)
			{
				if(p[i]&j)
					LcdData[0] = FT_Color;
				else 
					LcdData[0] = BK_Color;
			}
		}
		p+=16;
	}
}

void DisplayChar_12x24(char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  //显示任意字符
{
	u16 i,j;
	u8 *Font_Addr ;//= (u8 *)(0x08002800+0x70000 + ((Char)<<4));
	u8 ft = 0;	
	LCD9486_SetWindows(xp,yp,xp+11,yp+23);
	if(Char>=0x20)
		Char-=0x20;
	Font_Addr = (u8 *)&song_12x24[Char*(16*3)];
	
	for(j=0; j<24; j++)
	{
		ft= *Font_Addr++;
		for(i=0; i<8; i++)
		{
			if(ft & 0x80)     //get the word dot,if true,display 
				//Draw_Pixel(xp+i, yp+j, FT_Color);
				LcdData[0] = FT_Color;
			else LcdData[0] = BK_Color;
			ft <<= 1;
		}
		ft= *Font_Addr++;
		for(i=0; i<4; i++)
		{
			if(ft & 0x80)     //get the word dot,if true,display 
				//Draw_Pixel(xp+i, yp+j, FT_Color);
				LcdData[0] = FT_Color;
			else LcdData[0] = BK_Color;
			ft <<= 1;
		}
	}	
}

void DisplayChar_24x24(u8 *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u16 i,j;
	u8 *Font_Addr ;//= (u8 *)(0x08002800+0x70000 + ((Char)<<4));
	u8 ft = 0;	
	LCD9486_SetWindows(xp,yp,xp+23,yp+23);
	//Font_Addr = (u8 *)&song_8x16[Char*16];
	
	Font_Addr = FU_Get24x24Dot(hz);
	//memcpy(hztbuf,shang,32);
	
	for(j=0; j<(24*3); j++)
	{
		ft = Font_Addr[j];
		for(i=0; i<8; i++)
		{
			if(ft & 0x80)
				LcdData[0] = FT_Color;
			else 
				LcdData[0] = BK_Color;
			ft <<= 1;
		}
	}	
}

u8 FU_spizk_Get24x24Dot(u8 *buf,u8 *phz)
{
	u32 t;
	u32 hz = U8ToU16_Big(phz);
	t = ((hz>>8)*0xa2) + 0x3bbf;
	if(hz >= t)
	{
		W25Q32_Data_Read(buf,24*3,((uint32_t)519168)+(hz-t)*(24*3));
		return 1;
	}
	return 0;
}

void DisplayChar_spizk_24x24(uint8_t *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u8 i,j,ft;
	static uint8_t buf[24*3];
	FU_spizk_Get24x24Dot(buf,hz);
	LCD9486_SetWindows(xp,yp,xp+23,yp+23);
	
	for(j=0; j<(24*3); j++)
	{
		ft = buf[j];
		for(i=0; i<8; i++)
		{
			if(ft & 0x80)
				LcdData[0] = FT_Color;
			else 
				LcdData[0] = BK_Color;
			ft <<= 1;
		}
	}
}

static uint8_t sbuf[64*64/8];


u8 FU_spizk_Get_16x32(u8* buf,u8 c)
{
	u32 t = c;
	//u32 hz = U8ToU16_Big(phz);
	//t = ((hz>>8)*0xa2) + 0x3bbf;
	//if(hz >= t)
	{
		W25Q32_Data_Read(buf,32*2,((uint32_t)1107984+(32*2*0))+(t)*(32*2));
		return 1;
	}
	//return 0;
}

void DisplayChar_spizk_16x32(char c, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u8 i,j,ft;
	FU_spizk_Get_16x32(sbuf,c);
	LCD9486_SetWindows(xp,yp,xp+15,yp+31);
	
	for(j=0; j<(32*2); j++)
	{
		ft = sbuf[j];
		for(i=0x01; i; i <<= 1)
		{
			if(ft & i)//0x01)
				LcdData[0] = FT_Color;
			else 
				LcdData[0] = BK_Color;
			//ft <<= 1;
		}
	}
}

u8 FU_spizk_Get_32x32(u8 *buf,u8 *phz)
{
	u32 t;
	u32 hz = U8ToU16_Big(phz);
	t = ((hz>>8)*0xa2) + 0x3bbf;
	if(hz >= t)
	{
		W25Q32_Data_Read(buf,32*4,((uint32_t)1107984+8194)+(hz-t)*(32*4));
		return 1;
	}
	return 0;
}


void Displayhz_spizk_32x32(uint8_t *hz, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color)  
{
	u8 i,j,ft;
	FU_spizk_Get_32x32(sbuf,hz);
	LCD9486_SetWindows(xp,yp,xp+31,yp+31);
	
	for(j=0; j<(32*4); j++)
	{
		ft = sbuf[j];
		for(i=1; i; i<<=1)
		{
			if(ft & i)	//0x80)
				LcdData[0] = FT_Color;
			else 
				LcdData[0] = BK_Color;
			//ft <<= 1;
		}
	}
}


void DispStr_Allsize(uint16_t hsize,
							uint8_t *str,
							uint8_t zc,
							uint32_t ml,
							uint16_t x,
							uint16_t y,
							uint16_t bk,
							uint16_t ft)
{
	void (* p_disp_asc) (char Char, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
	void (* p_disp_hz)	(u8 *p, u16 xp, u16 yp, u16 BK_Color, u16 FT_Color);
	uint32_t i;

	switch(hsize)
	{
	case 12:
		p_disp_asc = DisplayChar_6x12;
		p_disp_hz  = _NULL;
		break;
	case 16:
		p_disp_asc = DisplayChar_8x16;
		p_disp_hz  = DisplayChar_16x16;
		break;
	case 24:
		p_disp_asc = DisplayChar_12x24;
		p_disp_hz  = DisplayChar_spizk_24x24;
		break;
	case 32:
		p_disp_asc = DisplayChar_spizk_16x32;
		p_disp_hz  = Displayhz_spizk_32x32;
		break;
	default:
		hsize = 0;
		break;
	}
	
	if(hsize)
	{
		for(i = 0;i<ml;i++)
		{
			if(zc)
			{
				if(str[i] == 0)
					break;
			}
			
			if((str[i]&0x80) == 0)
			{
				p_disp_asc(str[i],x,y,bk,ft);
				x += hsize/2;
			}
			else
			{
				if(p_disp_hz != _NULL)
				{
					p_disp_hz(&str[i],x,y,bk,ft);
					x += hsize;
				}
				i++;
			}
		}
	}
}


uint16_t Lcd_FastSqrt(uint16_t r,uint16_t x,uint16_t sy)
{
	/*
	uint32_t t2,ly2,ny2;
	t2 = r*r;
	t2 -= x*x;
	*/
	double fy;	
	fy = r*r;
	fy -= x*x;
	fy = sqrt(fy);
	return (uint16_t)(fy+0.5);
}

void LCD_DrawArc(uint16_t x0,uint16_t y0,uint16_t r,uint16_t c)
{
	uint16_t i,q,m;
	m = (r*7);
	m /= 10;

	for(i = 0;i<=m;i++)
	{
		q = Lcd_FastSqrt(r,i,0);
		LCD_DrawPoint(x0+i,y0+q,c);		
		LCD_DrawPoint(x0+i,y0-q,c);		
		LCD_DrawPoint(x0-i,y0+q,c);		
		LCD_DrawPoint(x0-i,y0-q,c);		

		LCD_DrawPoint(x0+q,y0+i,c);		
		LCD_DrawPoint(x0+q,y0-i,c);		
		LCD_DrawPoint(x0-q,y0+i,c);		
		LCD_DrawPoint(x0-q,y0-i,c);		
	}
	
}


uint8_t hz_size;
	
//uint8_t hz_testbuf[]={0xb0,0xa1};//{0xa1,0xa1};
uint8_t hz_testbuf[]={0xa1,0xa2};

void Lcd_Test(void)
{
	LCD9486_Clear(0);
	
#if 1
	DisplayChar_6x12('g',10,10,RED,BLUE);
#endif
	
#if 1//1
	DisplayChar_spizk_16x16(hz_testbuf, 5, 30, 0xffff,0);
	//DisplayChar_spizk_16x16("，上", 5, 30, 0xffff,0);

	//while(1)
	//	delay_ms(100);
#endif

#if 1
	//LCD9486_Clear(0);
	//Lcd_testColour3();
	
	//LCD_Fill(10,10,26,26,RED);
	DisplayChar_8x16('a',10,10,RED,BLUE);
	DisplayChar_8x16('A',18,10,RED,BLUE);
	hz_size = sizeof(song_16x16_str);
	DisplayChar_16x16("北",5,30,RED,BLUE);
#endif
	
#if 1//1	
	DisplayChar_12x24('a',10,10,RED,BLUE);
	DisplayChar_spizk_24x24("北",5,30,RED,BLUE);
	sleep(10);
	
	//while(1);
	
#endif

	
#if 1//1	
	LCD9486_Clear(0);
	DisplayChar_spizk_16x32('a',10,10,RED,BLUE);
	Displayhz_spizk_32x32("北",5,45,RED,BLUE);
	sleep(10);
	
	//while(1);
	
#endif
	
	
}






//end file


