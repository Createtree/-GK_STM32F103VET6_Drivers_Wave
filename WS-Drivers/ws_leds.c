/******************************************************************************
 * @file     				 ws_leds.c
 * @brief    				 驱动点阵屏幕
                     1、5*7 ， 7*8 ， 8*16 ASCII码取芯片内部数据，提高取数效率
										 2、系统内有字库芯片，汉字显示从字库芯片读取点阵信息
										 3、
 * @version  				 1.0.0
 * @creationTime     2020_03_05
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#include "ws_leds.h"
#include "ws_dzcode.h"         //  ASCII 8*8 8*16 点阵信息


#define  LEDS_EN 1

#ifdef LEDS_EN


//  显示使能脚
#define WS_Leds_define_OE_DI    Leds_OE_GPIO_Port->ODR |=   Leds_OE_Pin  ;
#define WS_Leds_define_OE_EN    Leds_OE_GPIO_Port->ODR &= ~ Leds_OE_Pin  ;

//  595输出脚
#define WS_Leds_define_LAT_RESET    Leds_LAT_GPIO_Port->ODR |=   Leds_LAT_Pin  ;
#define WS_Leds_define_LAT_SET    Leds_LAT_GPIO_Port->ODR &= ~ Leds_LAT_Pin  ;

//  595时钟脚
#define WS_Leds_define_CLK_SET    Leds_CLK_GPIO_Port->ODR |=   Leds_CLK_Pin  ;
#define WS_Leds_define_CLK_RESET    Leds_CLK_GPIO_Port->ODR   &= ~ Leds_CLK_Pin  ;


//   行控制脚
#define WS_Leds_defineSet_Line_IO(num)   GPIOB->ODR &= ~(0xf<<12);GPIOB->ODR |= num<<12;



//    点阵数据缓存
char  ledsDisBuffer[leds_Row][leds_Column];


//   行数据同步更新
void outLedsLineDate(void) 
{
    WS_Leds_define_LAT_SET  ;
    __NOP(); __NOP(); __NOP();__NOP(); __NOP();__NOP();__NOP(); 
    WS_Leds_define_LAT_RESET;
}

//  总线写一个字节数据出去
void writeLedsByte( char  *p) {
    unsigned char i, h, l;
    h = *p;
    l = *(p+(leds_Row/2) * leds_Column);
    for( i = 0 ; i < leds_Column; i ++) {

       WS_Leds_define_CLK_RESET;
        if(h&1) 
					{            
					  Leds_R1_GPIO_Port->ODR |=  Leds_R1_Pin;
          } 
					else   
					{
            Leds_R1_GPIO_Port->ODR &= ~Leds_R1_Pin;
          }

        if(l&1) 
					{
             Leds_R2_GPIO_Port->ODR |=  Leds_R2_Pin;
          } 
					else 
					{
             Leds_R2_GPIO_Port->ODR &= ~Leds_R2_Pin;
          }  
        WS_Leds_define_CLK_SET; 
        h >>=1;   l >>=1;      
	
    }
}

//   总线写一行数据出去
void writeLedsBytes(char *p,unsigned char n) {
    while(n --) {
        writeLedsByte(p++);
    }
    outLedsLineDate();
}




//   显示16*16点阵
void dis16X16Data(int x,int y,char *p) {
    char i;
    y *= 16;
    for(i=0; i<16; i++) {
        setLedsByte(x,i+y,*p++);
        setLedsByte(x+1,i+y,*p++);
    }
}


//   显示8*8点阵
void dis8X8Data(int x,int y,char *p) {
    char i;
    y *= 8;
    for(i=0; i<8; i++) {
        setLedsByte(x,i+y,*p);
        p++;
    }
}

//   显示5*7点阵
void dis5X7Data(int x,int y,char *p) {
    char i,j;
	  unsigned char num;
    y *= 8;
	  for(j=0;j<8;j++)
	  {
			 num = *p ++;
       for(i=7; i>2; i--) 
		   {
				 if((num & 1<<i) == 0)
				 {
				    WS_Leds_Set_Point(x++,y+j,0);
				 }
				 else
				 {
				    WS_Leds_Set_Point(x++,y+j,1);
				 }    
       }
			 x -= 5;	 
	  }
}

//   显示8*16点阵
void dis8X16Data(int x,int y,char *p) {
    char i;
    y *= 16;
    for(i=0; i<16; i++) {
        setLedsByte(x,i+y,*p);
        p++;
    }
}





////   关闭某个字符显示
//void close8Char(char x,char y) {
//    char i;
//    y *= 8;
//    for(i=0; i<8; i++) {
//        setLedsByte(x,i+y,0x00);
//    }
//}
////  显示8*8字符
//void dis8X8String(char x,char y,char *p) {
//    char *codeP;
//    while(*p >0 && *p  != '\n') {
//        codeP = (char *)string8X8Code +  + (*p - 0x20)*8;
//        dis8X8Data(x,y,codeP) ;
//        x+=1;
//        p ++;
//        if(x == 8) return     ;
//    }
//}
////  显示8*8数字
//void dis8NumberString(char x,char y,char *p) {
//    char *codeP;
//    while(*p >0 && *p  != '\n') {
//        codeP = (char *)number8Code + (*p - 0x30)*8;
//        dis8X8Data(x,y,codeP) ;
//        x+=1;
//        p ++;
//        if(x == 8) return ;
//    }
//}
////  显示8数字
//void dis8Number(char x,char y,int n) {
//    dis8X8String(x,y,WS_Format("%d",(int)n));
//}

////  显示秒函数
//void dis16Sec(int sec) {
//    static char buf[5] = {0};
//    char m, s;
//    m = sec /60;
//    s = sec %60;
//    if(m > 99) m = 99;
//    buf[0] = m / 10 + 0x30;
//    buf[1] = m % 10 + 0x30;
//    buf[2] = s / 10 + 0x30;
//    buf[3] = s % 10 + 0x30;
//    dis8X16String(0,1,buf)    ;
//    openDPToClose();
//}

////  显示小数点
//int openDp(void) {
//    WS_Leds_Set_Point(31,5,1);
//    WS_Leds_Set_Point(32,5,1);
//    WS_Leds_Set_Point(31,6,1);
//    WS_Leds_Set_Point(32,6,1);

//    WS_Leds_Set_Point(31,27,1);
//    WS_Leds_Set_Point(32,27,1);
//    WS_Leds_Set_Point(31,28,1);
//    WS_Leds_Set_Point(32,28,1);
//    return 0;
//}

////  关闭小数点
//int closeDp(void) {
//    WS_Leds_Set_Point(31,5,0);
//    WS_Leds_Set_Point(32,5,0);
//    WS_Leds_Set_Point(31,6,0);
//    WS_Leds_Set_Point(32,6,0);

//    WS_Leds_Set_Point(31,27,0);
//    WS_Leds_Set_Point(32,27,0);
//    WS_Leds_Set_Point(31,28,0);
//    WS_Leds_Set_Point(32,28,0);

//    return 0;
//}

////  小数点闪烁
//void openDPToClose(void) {

//    closeDp();
//    CreateTimer(openDp,50);
//}


/******************************************************************************
	*方法名称： void setLedsLine(char n)
	*功能：     选择对应的行显示
  *参数：     n	：对应行（0~15）						
	*返回:		  无
*******************************************************************************/

void setLedsLine(char n) 
{
    if( n > 15) return ;
    n = 15-n;
    WS_Leds_defineSet_Line_IO (n);
}



/******************************************************************************
	*方法名称： void WS_Leds_Updata_Buffer(void)
	*功能：     用于定时中断函数刷新点阵显示
  *参数：     无							
	*返回:		  无
*******************************************************************************/

void WS_Leds_Updata_Buffer(void) {
	
    static  char line=0;

    WS_Leds_define_OE_DI  //  禁止显示

		writeLedsBytes(ledsDisBuffer[line],8);
		setLedsLine(line);
    if(++line == 16) line = 0;

    WS_Leds_define_OE_EN  // 开启显示
}

/******************************************************************************
	*方法名称： void WS_Leds_Dis_Clean(void)
	*功能：     清屏
  *参数：     无							
	*返回:		  无
*******************************************************************************/
void WS_Leds_Dis_Clean(void)
{
    char i,j;
    for(i=0; i<leds_Row; i++)
        for(j=0; j<leds_Column; j++)
            ledsDisBuffer[i][j] = 0x00;
}

/******************************************************************************
	*方法名称： void WS_Leds_Dis_Full(void)
	*功能：     全屏点亮
  *参数：     无							
	*返回:		  无
*******************************************************************************/  
void WS_Leds_Dis_Full(void) 
{
    char i,j;
    for(i=0; i<leds_Row; i++)
        for(j=0; j<leds_Column; j++)
            ledsDisBuffer[i][j] = 0xff;
}



/********************************************************************************
	*方法名称： WS_Leds_Set_Line
	*功能：     根据坐标设置对应点的状态
  *参数：     x: x坐标（0~leds_Column-1） ；y: y坐标（0~leds_Row/16）
*						stu ： 1:显示 ， 0：关闭
	*返回：			无
*******************************************************************************/

void WS_Leds_Set_Line(int x,int y,char stu ) {
    char i;
    y *=16;
    for(i=0; i<16; i++) {
        WS_Leds_Set_Point(x,i+y,stu);
    }
}



/********************************************************************************
	*方法名称： WS_Leds_Set_Point
	*功能：     根据坐标设置对应点的状态
  *参数：     x: x坐标（0~leds_Column-1） ；y: y坐标（0~leds_Row）
*						stu ： 1:显示 ， 0：关闭
	*返回：			无
*******************************************************************************/

void WS_Leds_Set_Point(int x,int y,char stu) {
   
	  //   排除非法数据
	  if(x >= leds_Column*8 || y >= leds_Row)  return ;
	
	  x = leds_Column*8-1-x;
    if(stu == 0) {
        ledsDisBuffer[y][x/8] &= ~(1<<x%8);
    } else {
        ledsDisBuffer[y][x/8] |=  1<<x%8;
    }
}

/********************************************************************************
	*方法名称： setLedsByte
	*功能：     根据坐标设置缓存数据（单位：字节）
  *参数：     x: x坐标（0~leds_Column/8-1） ；y: y坐标（0~leds_Row）
	*						dat ： 设置的数据
	*返回：			无
*******************************************************************************/

void setLedsByte(int x,int y,char dat) 
{
    x = leds_Column-1-x;
    ledsDisBuffer[y][x] = dat;
}



/******************************************************************************
	*方法名称： WS_Leds_Dis_8X16_String(char x,char y,char  *p_string)
	*功能：     在LEDS上显示8X16字符，字母、汉字兼容
  *参数：     x: x坐标（0~7）； y :  y坐标（0~1）
							buff :   字符缓存 
	*返回:		  
*******************************************************************************/
void WS_Leds_Dis_8X16_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	   {
		   if((idex+x)>7) return;            //每列字符长度大于最大显示长度将不显示超出的部分
		   if((uint8_t)p_string[idex]<0x80)
		    { //判断当前字符是否是双字节汉字
			     // dis8X16Data(idex,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_8X16)); 
			     dis8X16Data(idex+x,y,(char *)(uint8_t *)(string8X16Code + (p_string[idex] - 0x20)*16));
			  }
		else
		 {
			 
//  有字库信息就读取字库信息显示汉字				
#if WS_GT21_EN			
			  dis16X16Data(idex+x,y,(char *)getGb16x16Code(p_string[idex],p_string[idex+1]));
#endif
			  idex++;
		 }
	}
}


/******************************************************************************
	*方法名称： void  WS_Leds_Dis_5X7_String(char x,char y,char  *p_string)
	*功能：     在LEDS上显示5X7字符 , 只能显示ASCII码
  *参数：     x: x坐标（0~63）； y :  y坐标（0~3）
							buff :   字符缓存 
	*返回:		  
*******************************************************************************/
void WS_Leds_Dis_5X7_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	{
		if((idex*6 + x) >= leds_Column*8 -4) return;      //每列字符长度大于最大显示长度将不显示超出的部分
		if((uint8_t)p_string[idex]<0x80)  //判断当前字符是否是双字节汉字
		{
			//dis5X7Data(idex*6+x,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_5X7)); 
      dis5X7Data(idex*6+x,y,(char *)(string5X7Code + (p_string[idex] - 0x20)*8));		
		}
	  else  //  跳过汉字
		{
		   idex ++;
		}		
	}
}



/******************************************************************************
	*方法名称： void  WS_Leds_Dis_7X8_String(char x,char y,char  *p_string)
	*功能：     在LEDS上显示7X8字符 , 只能显示ASCII码
  *参数：     x: x坐标（0~7）； y :  y坐标（0~3）
							buff :   字符缓存 
	*返回:		  
*******************************************************************************/
void WS_Leds_Dis_7X8_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	{
		if((idex+x)>7) return;      //每列字符长度大于最大显示长度将不显示超出的部分
		if((uint8_t)p_string[idex]<0x80)  //判断当前字符是否是双字节汉字
		{
			//dis8X8Data(idex+x,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_7X8)); 
       dis8X8Data(idex+x,y,(char *)(string7X8Code + (p_string[idex] - 0x20)*8));		
		}
	  else  //  跳过汉字
		{
		   idex ++;   
		}		
	}
}






#endif
