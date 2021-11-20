
#include "lcd9486_drv.h"
#include "drive_dzcode.h"

typedef struct
{
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD9486_TypeDef ;


#define LCD9486_BASE        ((uint32_t)(0x60000000 | 0x00FFFFFE))
#define LCD9486             ((LCD9486_TypeDef *) LCD9486_BASE)


__IO uint16_t *p_LCD9486_Ram;
__IO uint16_t *p_LCD9486_Reg;


#define LCD_WR_REG(d)	      (*p_LCD9486_Reg) = (d)		//LCD9486->LCD_REG = (d)
#define LCD_WR_DATA(d)	    (*p_LCD9486_Ram) = (d)		//LCD9486->LCD_RAM = (d)
#define LCD_WriteReg(r,d)		(*p_LCD9486_Reg) = (r),(*p_LCD9486_Ram) = (d)
	


void LCD9486_Addr_Init(void)
{
	  p_LCD9486_Ram = (uint16_t *)(((uint32_t)(0x60000000 | 0x00FFFFFE))+2);
	  p_LCD9486_Reg = (uint16_t *)(((uint32_t)(0x60000000 | 0x00FFFFFE)));
}

void LCD9486_RESET(void)
{
//	LCD9486_RST_CLR  ;  
	WS_Delay_MS(100) ;	
//	LCD9486_RST_SET  ;
	WS_Delay_MS(50)  ;
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

void LCD9486_SetParam(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;			
	LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;	
	LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
#endif
}

void LCD9486_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 




//void LCD9486_SetWindows_ForRead(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
//{	
//	LCD_WR_REG(lcddev.setxcmd);	
//	LCD_WR_DATA(xStar>>8);
//	LCD_WR_DATA(0x00FF&xStar);		
//	LCD_WR_DATA(xEnd>>8);
//	LCD_WR_DATA(0x00FF&xEnd);

//	LCD_WR_REG(lcddev.setycmd);	
//	LCD_WR_DATA(yStar>>8);
//	LCD_WR_DATA(0x00FF&yStar);		
//	LCD_WR_DATA(yEnd>>8);
//	LCD_WR_DATA(0x00FF&yEnd);

//	LCD_WR_REG(0x2e);	
//	xStar = (*p_LCD9486_Ram);	
//} 


//   按照指定颜色清屏
void LCD9486_Clear(uint16_t Color)
{	
	uint32_t index=0; 
	     
	LCD9486_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	

	for(index=0;index<LCD9486_XMAX * LCD9486_YMAX ;index++)
	{
		LCD9486->LCD_RAM=Color;		  
	}
} 


void LCD9486_Init(void) 
{  
     										 
	LCD9486_Addr_Init();
	
 	//LCD9486_RESET()    ;

//*************2.8inch ILI9341初始化**********//	
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC9); //C1 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x00);   //SAP[2:0];BT[3:0] 01 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x08); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	WS_Delay_MS(120);
	LCD_WR_REG(0x29); //display on		
	LCD9486_SetParam();//设置LCD参数	 
	
	
	//    点亮背光	
	LCD9486_LIGHT_OPEN ;
	//  测试
	LCD9486_Clear(RED);
	WS_Delay_MS(500);
	LCD9486_Clear(WHITE);
	WS_TFT_Dis_8X8_String(100,100,"12345678",RED);   
  WS_TFT_Dis_8X16_String(100,120,"12345678",GREEN);	
}


//  开始跟新选择区域内的GRAM		 
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 


//   设置点坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos)  ;		
	LCD_WR_DATA((Xpos+1)>>8)  ;
	LCD_WR_DATA((Xpos+1))     ;
	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		
	LCD_WR_DATA((Ypos+1)>>8);
	LCD_WR_DATA((Ypos+1));
	LCD_WriteRAM_Prepare();	//开始写入GRAM		
} 

//   选择需要更新的区域
void LCD9486_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);

	LCD9486_WriteRAM_Prepare();	//开始写入GRAM			
}  


//   画点
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t c)
{
	LCD_SetCursor(x,y);
	LCD_WR_DATA(c)    ; 
}


//   
void WS_TFT_Dis_8X8_String (int x,int y,char  *string , uint16_t color)
{ 
	uint16_t i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat ;
	for(idex=0;idex<len;idex++)
	{
		if((uint8_t)string[idex]<0x80)  //判断当前字符是否是双字节汉字
		{
			 LCD9486_SetWindows(x,y,x+7,y+7);	
       p_dat = (uint8_t *)(string8X8Code + (string[idex] - 0x20)*8);	
			 for(i=0;i<8;i++)
			 {
					dat = p_dat[i]  ;
					for(j=0;j<8;j++)
					{
							if((dat & 1<<(7-j))  != 0 )
							{
							    LCD_WR_DATA(color);
                  //osDelay(100);								
							}
							else
							{
							    LCD_WR_DATA(WHITE); 
							} 
					}
			 }
      		
		}
	  else  //  跳过汉字
		{
		   idex ++;   
		}
    x +=  8;	
		if(x  >=  LCD9486_XMAX - 8) return;      //每列字符长度大于最大显示长度将不显示超出的部分		
	}
}


void WS_TFT_Dis_8X16_String (int x,int y,char  *string , uint16_t color)
{ 
	uint16_t i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat ;
	for(idex=0;idex<len;idex++)
	{
		if((uint8_t)string[idex]<0x80)  //判断当前字符是否是双字节汉字
		{
			 LCD9486_SetWindows(x,y,x+7,y+15);	
       p_dat = (uint8_t *)(string8X16Code + (string[idex] - 0x20)*16);	
			 for(i=0;i<16;i++)
			 {
					dat = p_dat[i]  ;
					for(j=0;j<8;j++)
					{
							if((dat & 1<<(7-j))  != 0 )
							{
							    LCD_WR_DATA(color);
                  //osDelay(100);								
							}
							else
							{
							    LCD_WR_DATA(WHITE); 
							} 
					}
			 }
      		
		}
	  else  //  跳过汉字
		{
		   idex ++;   
		}
    x +=  8;	
		if(x  >=  LCD9486_XMAX - 8) return;      //每列字符长度大于最大显示长度将不显示超出的部分		
	}
}



