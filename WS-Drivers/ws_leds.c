/******************************************************************************
 * @file     				 ws_leds.c
 * @brief    				 ����������Ļ
                     1��5*7 �� 7*8 �� 8*16 ASCII��ȡоƬ�ڲ����ݣ����ȡ��Ч��
										 2��ϵͳ�����ֿ�оƬ��������ʾ���ֿ�оƬ��ȡ������Ϣ
										 3��
 * @version  				 1.0.0
 * @creationTime     2020_03_05
 * @creator					 
 * @readme
 *        1.STM32F103ϵ��   
 *				2. 
          3.
*******************************************************************************/


#include "ws_leds.h"
#include "ws_dzcode.h"         //  ASCII 8*8 8*16 ������Ϣ


#define  LEDS_EN 1

#ifdef LEDS_EN


//  ��ʾʹ�ܽ�
#define WS_Leds_define_OE_DI    Leds_OE_GPIO_Port->ODR |=   Leds_OE_Pin  ;
#define WS_Leds_define_OE_EN    Leds_OE_GPIO_Port->ODR &= ~ Leds_OE_Pin  ;

//  595�����
#define WS_Leds_define_LAT_RESET    Leds_LAT_GPIO_Port->ODR |=   Leds_LAT_Pin  ;
#define WS_Leds_define_LAT_SET    Leds_LAT_GPIO_Port->ODR &= ~ Leds_LAT_Pin  ;

//  595ʱ�ӽ�
#define WS_Leds_define_CLK_SET    Leds_CLK_GPIO_Port->ODR |=   Leds_CLK_Pin  ;
#define WS_Leds_define_CLK_RESET    Leds_CLK_GPIO_Port->ODR   &= ~ Leds_CLK_Pin  ;


//   �п��ƽ�
#define WS_Leds_defineSet_Line_IO(num)   GPIOB->ODR &= ~(0xf<<12);GPIOB->ODR |= num<<12;



//    �������ݻ���
char  ledsDisBuffer[leds_Row][leds_Column];


//   ������ͬ������
void outLedsLineDate(void) 
{
    WS_Leds_define_LAT_SET  ;
    __NOP(); __NOP(); __NOP();__NOP(); __NOP();__NOP();__NOP(); 
    WS_Leds_define_LAT_RESET;
}

//  ����дһ���ֽ����ݳ�ȥ
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

//   ����дһ�����ݳ�ȥ
void writeLedsBytes(char *p,unsigned char n) {
    while(n --) {
        writeLedsByte(p++);
    }
    outLedsLineDate();
}




//   ��ʾ16*16����
void dis16X16Data(int x,int y,char *p) {
    char i;
    y *= 16;
    for(i=0; i<16; i++) {
        setLedsByte(x,i+y,*p++);
        setLedsByte(x+1,i+y,*p++);
    }
}


//   ��ʾ8*8����
void dis8X8Data(int x,int y,char *p) {
    char i;
    y *= 8;
    for(i=0; i<8; i++) {
        setLedsByte(x,i+y,*p);
        p++;
    }
}

//   ��ʾ5*7����
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

//   ��ʾ8*16����
void dis8X16Data(int x,int y,char *p) {
    char i;
    y *= 16;
    for(i=0; i<16; i++) {
        setLedsByte(x,i+y,*p);
        p++;
    }
}





////   �ر�ĳ���ַ���ʾ
//void close8Char(char x,char y) {
//    char i;
//    y *= 8;
//    for(i=0; i<8; i++) {
//        setLedsByte(x,i+y,0x00);
//    }
//}
////  ��ʾ8*8�ַ�
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
////  ��ʾ8*8����
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
////  ��ʾ8����
//void dis8Number(char x,char y,int n) {
//    dis8X8String(x,y,WS_Format("%d",(int)n));
//}

////  ��ʾ�뺯��
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

////  ��ʾС����
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

////  �ر�С����
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

////  С������˸
//void openDPToClose(void) {

//    closeDp();
//    CreateTimer(openDp,50);
//}


/******************************************************************************
	*�������ƣ� void setLedsLine(char n)
	*���ܣ�     ѡ���Ӧ������ʾ
  *������     n	����Ӧ�У�0~15��						
	*����:		  ��
*******************************************************************************/

void setLedsLine(char n) 
{
    if( n > 15) return ;
    n = 15-n;
    WS_Leds_defineSet_Line_IO (n);
}



/******************************************************************************
	*�������ƣ� void WS_Leds_Updata_Buffer(void)
	*���ܣ�     ���ڶ�ʱ�жϺ���ˢ�µ�����ʾ
  *������     ��							
	*����:		  ��
*******************************************************************************/

void WS_Leds_Updata_Buffer(void) {
	
    static  char line=0;

    WS_Leds_define_OE_DI  //  ��ֹ��ʾ

		writeLedsBytes(ledsDisBuffer[line],8);
		setLedsLine(line);
    if(++line == 16) line = 0;

    WS_Leds_define_OE_EN  // ������ʾ
}

/******************************************************************************
	*�������ƣ� void WS_Leds_Dis_Clean(void)
	*���ܣ�     ����
  *������     ��							
	*����:		  ��
*******************************************************************************/
void WS_Leds_Dis_Clean(void)
{
    char i,j;
    for(i=0; i<leds_Row; i++)
        for(j=0; j<leds_Column; j++)
            ledsDisBuffer[i][j] = 0x00;
}

/******************************************************************************
	*�������ƣ� void WS_Leds_Dis_Full(void)
	*���ܣ�     ȫ������
  *������     ��							
	*����:		  ��
*******************************************************************************/  
void WS_Leds_Dis_Full(void) 
{
    char i,j;
    for(i=0; i<leds_Row; i++)
        for(j=0; j<leds_Column; j++)
            ledsDisBuffer[i][j] = 0xff;
}



/********************************************************************************
	*�������ƣ� WS_Leds_Set_Line
	*���ܣ�     �����������ö�Ӧ���״̬
  *������     x: x���꣨0~leds_Column-1�� ��y: y���꣨0~leds_Row/16��
*						stu �� 1:��ʾ �� 0���ر�
	*���أ�			��
*******************************************************************************/

void WS_Leds_Set_Line(int x,int y,char stu ) {
    char i;
    y *=16;
    for(i=0; i<16; i++) {
        WS_Leds_Set_Point(x,i+y,stu);
    }
}



/********************************************************************************
	*�������ƣ� WS_Leds_Set_Point
	*���ܣ�     �����������ö�Ӧ���״̬
  *������     x: x���꣨0~leds_Column-1�� ��y: y���꣨0~leds_Row��
*						stu �� 1:��ʾ �� 0���ر�
	*���أ�			��
*******************************************************************************/

void WS_Leds_Set_Point(int x,int y,char stu) {
   
	  //   �ų��Ƿ�����
	  if(x >= leds_Column*8 || y >= leds_Row)  return ;
	
	  x = leds_Column*8-1-x;
    if(stu == 0) {
        ledsDisBuffer[y][x/8] &= ~(1<<x%8);
    } else {
        ledsDisBuffer[y][x/8] |=  1<<x%8;
    }
}

/********************************************************************************
	*�������ƣ� setLedsByte
	*���ܣ�     �����������û������ݣ���λ���ֽڣ�
  *������     x: x���꣨0~leds_Column/8-1�� ��y: y���꣨0~leds_Row��
	*						dat �� ���õ�����
	*���أ�			��
*******************************************************************************/

void setLedsByte(int x,int y,char dat) 
{
    x = leds_Column-1-x;
    ledsDisBuffer[y][x] = dat;
}



/******************************************************************************
	*�������ƣ� WS_Leds_Dis_8X16_String(char x,char y,char  *p_string)
	*���ܣ�     ��LEDS����ʾ8X16�ַ�����ĸ�����ּ���
  *������     x: x���꣨0~7���� y :  y���꣨0~1��
							buff :   �ַ����� 
	*����:		  
*******************************************************************************/
void WS_Leds_Dis_8X16_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	   {
		   if((idex+x)>7) return;            //ÿ���ַ����ȴ��������ʾ���Ƚ�����ʾ�����Ĳ���
		   if((uint8_t)p_string[idex]<0x80)
		    { //�жϵ�ǰ�ַ��Ƿ���˫�ֽں���
			     // dis8X16Data(idex,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_8X16)); 
			     dis8X16Data(idex+x,y,(char *)(uint8_t *)(string8X16Code + (p_string[idex] - 0x20)*16));
			  }
		else
		 {
			 
//  ���ֿ���Ϣ�Ͷ�ȡ�ֿ���Ϣ��ʾ����				
#if WS_GT21_EN			
			  dis16X16Data(idex+x,y,(char *)getGb16x16Code(p_string[idex],p_string[idex+1]));
#endif
			  idex++;
		 }
	}
}


/******************************************************************************
	*�������ƣ� void  WS_Leds_Dis_5X7_String(char x,char y,char  *p_string)
	*���ܣ�     ��LEDS����ʾ5X7�ַ� , ֻ����ʾASCII��
  *������     x: x���꣨0~63���� y :  y���꣨0~3��
							buff :   �ַ����� 
	*����:		  
*******************************************************************************/
void WS_Leds_Dis_5X7_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	{
		if((idex*6 + x) >= leds_Column*8 -4) return;      //ÿ���ַ����ȴ��������ʾ���Ƚ�����ʾ�����Ĳ���
		if((uint8_t)p_string[idex]<0x80)  //�жϵ�ǰ�ַ��Ƿ���˫�ֽں���
		{
			//dis5X7Data(idex*6+x,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_5X7)); 
      dis5X7Data(idex*6+x,y,(char *)(string5X7Code + (p_string[idex] - 0x20)*8));		
		}
	  else  //  ��������
		{
		   idex ++;
		}		
	}
}



/******************************************************************************
	*�������ƣ� void  WS_Leds_Dis_7X8_String(char x,char y,char  *p_string)
	*���ܣ�     ��LEDS����ʾ7X8�ַ� , ֻ����ʾASCII��
  *������     x: x���꣨0~7���� y :  y���꣨0~3��
							buff :   �ַ����� 
	*����:		  
*******************************************************************************/
void WS_Leds_Dis_7X8_String (int x,int y,char  *p_string)
{
	uint8_t len=strlen((char *)p_string);
	uint8_t idex=0;
	for(idex=0;idex<len;idex++)
	{
		if((idex+x)>7) return;      //ÿ���ַ����ȴ��������ʾ���Ƚ�����ʾ�����Ĳ���
		if((uint8_t)p_string[idex]<0x80)  //�жϵ�ǰ�ַ��Ƿ���˫�ֽں���
		{
			//dis8X8Data(idex+x,y,(char *)getAsciiCode(p_string[idex],ASCLL_TYPE_7X8)); 
       dis8X8Data(idex+x,y,(char *)(string7X8Code + (p_string[idex] - 0x20)*8));		
		}
	  else  //  ��������
		{
		   idex ++;   
		}		
	}
}






#endif
