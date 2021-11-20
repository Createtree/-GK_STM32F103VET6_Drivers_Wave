
/******************************************************************************
 * @file     				 ws_task.c
 * @brief    				 ws程序框架下的任务管理
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/

#include "ws_task.h"
#include "stdlib.h "
#include "math.h"


//   外部变量声明
extern   UART_HandleTypeDef huart1;

extern   SPI_HandleTypeDef  hspi2 ;

extern   RTC_HandleTypeDef hrtc;

extern  ADC_HandleTypeDef hadc1;
extern  ADC_HandleTypeDef hadc2;

extern  TIM_HandleTypeDef htim3;


//宏定义
#define ScanSize 480
#define Relay1_State HAL_GPIO_ReadPin(Relay1_GPIO_Port,Relay1_Pin)
#define Relay2_State HAL_GPIO_ReadPin(Relay2_GPIO_Port,Relay2_Pin)
//全局变量
uint16_t ADC_channel=1;
double Vmax=0;
double Vmin=0;
double Vp=0;
double Vrms=0;

float Ky=47;
int Px=0;
int Py=0;
//int Flash=0;
float f=0;
float _f;
float Duty=0;
float Duty_P=0;
float Duty_M=0;
uint16_t Wave_Type=0;
char WaveName[6][9] = {"空","正弦波","矩形波","三角波","正锯齿波","反锯齿波"};
char ModeName[3][8] = {"10倍","直通","衰减"};
uint16_t p_adcBuffer[ScanSize];

uint16_t Mode_State = 0;//模式
uint16_t Mode_time=0;
uint16_t pause=0;
uint16_t ADCPeriod=0;//ADC采样频率
float Vup[3]={1.27,1.24,1.16};//抬升电压
//频率捕获
uint32_t ex1_high;//存捕获的值
uint32_t ex1_low;//存捕获的值
uint32_t ex1_flag;//中断计数标志位

//uint32_t dV_Buf[ScanSize/2];//斜率缓存
//uint16_t dV_Flag=0;

uint16_t adc_flag = 0;
uint16_t adcBuffer[ScanSize] ;
float Vpp_Buf[ScanSize];
//测量模式宏函数
#define Mode0() do{HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_RESET);HAL_GPIO_WritePin(Relay2_GPIO_Port,Relay2_Pin,GPIO_PIN_RESET);Mode_State=0;}while(0)//10倍
#define Mode1() do{HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(Relay2_GPIO_Port,Relay2_Pin,GPIO_PIN_RESET);Mode_State=1;}while(0)//直通
#define Mode2() do{HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_SET);HAL_GPIO_WritePin(Relay2_GPIO_Port,Relay2_Pin,GPIO_PIN_SET);Mode_State=2;}while(0)//衰减

/******************************************************************************
	*函数名称： WS_OS_Task_RunLed(void)
	*功能：     WS_OS任务 , 运行灯任务， 每秒一次 ，打印测试信息
	*参数：     无
	*返回:			执行时间间隔
*******************************************************************************/
int testIfg  = 0 ;
int  WS_OS_Task_RunLed(void * data)
{ 
    RTC_TimeTypeDef  sTime ;
	  RTC_DateTypeDef  sDate ;
	
	 
//	  float vol=0   ;
//	  int   temper  ;
	
//	  uint32_t adc1Number  = 0;
	
	

//	  int i;
//	  //   DMA转换
//		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcBuffer,20);
//		osDelay(30)    ;
//	  for(i=0;i<20;i++)
//	  {
//		  adc1Number += adcBuffer[i];
//		}
//		adc1Number /= 20;
		
  if(adc_flag == 0)
  {
		adc_flag = 1;
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adcBuffer,ScanSize);
		HAL_TIM_Base_Start_IT(&htim3);
  }

   WS_Borad_Open_RunLed_Time(10)   ;
	
	 //  日历更新
//	 HAL_RTC_GetTime(&hrtc  ,&sTime, RTC_FORMAT_BIN);
//	 HAL_RTC_GetDate(&hrtc  ,&sDate, RTC_FORMAT_BIN);
//	 WS_TFT_Dis_8X16_String(0,224,  WS_Format("20%02d:%02d:%02d / ",sDate.Year,sDate.Month,sDate.Date),WHITE, BLACK);
//	 WS_TFT_Dis_8X16_String(104,224,WS_Format("%02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds),WHITE, BLACK);



		

		
		//显示ADC1采集数据
		if(Mode_State==2)//衰减
		{
			 WS_TFT_Dis_8X16_String(0,150,WS_Format("ADC1:%.2fv  Vmax: %.2lfv  Vmin:%.2lfv    ",(float)*(p_adcBuffer+ScanSize-1)*2.5/4096,(Vmax-Vup[Mode_State])*5,(Vmin-Vup[Mode_State])*5),WHITE, BLACK);
			 WS_TFT_Dis_8X16_String(0,166,WS_Format("Vpp: %.2fv   Vrms: %.2fv   Mode:%s   ",Vp*5,Vrms*5,ModeName[Mode_State]),WHITE, BLACK);
		}
		if(Mode_State==1)//直通
		{
			 WS_TFT_Dis_8X16_String(0,150,WS_Format("ADC1:%.2fv  Vmax: %.2lfv  Vmin:%.2lfv    ",(float)*(p_adcBuffer+ScanSize-1)*2.5/4096,Vmax-Vup[Mode_State],Vmin-Vup[Mode_State]),WHITE, BLACK);
			 WS_TFT_Dis_8X16_String(0,166,WS_Format("Vpp: %.2fv   Vrms: %.2fv   Mode:%s   ",Vp,Vrms,ModeName[Mode_State]),WHITE, BLACK);
		}
		if(Mode_State==0)//放大
		{
			 WS_TFT_Dis_8X16_String(0,150,WS_Format("ADC1:%.2fv  Vmax: %.1lfmv  Vmin:%.1lfmv    ",(float)*(p_adcBuffer+ScanSize-1)*2.5/4096,(Vmax-Vup[Mode_State])*100,(Vmin-Vup[Mode_State])*100),WHITE, BLACK);
			 WS_TFT_Dis_8X16_String(0,166,WS_Format("Vpp: %.1fmv   Vrms: %.1fmv   Mode:%s   ",Vp*100,Vrms*100,ModeName[Mode_State]),WHITE, BLACK);
		}
		
		WS_TFT_Dis_8X16_String(0,198,WS_Format("WaveType: %s  +Wid%.2fms  -Wid%.2fms     ",&WaveName[Wave_Type],(float)ex1_high/1000,(float)ex1_low/1000),WHITE, BLACK);

		//	 WS_TFT_Dis_8X16_String(0,198,WS_Format("SC:%2d  Py:%d  Ky:%.0f ",ScanSize,Py,Ky),WHITE, BLACK);
		if(ex1_high > 0 || ex1_low > 0)
		{
			Duty = (float)ex1_high/(ex1_high+ex1_low);
			f = 1000000.0/(ex1_high+ex1_low);
			if(f>2E3)
				WS_TFT_Dis_8X16_String(0,182,WS_Format("F: %.1fkHz  Duty: %.1f%%   ADCPer=%d        ",f/1000,Duty*100,ADCPeriod),WHITE, BLACK);
			else
				WS_TFT_Dis_8X16_String(0,182,WS_Format("F: %.1fHz  Duty: %.1f%%   ADCPer=%d        ",f,Duty*100,ADCPeriod),WHITE, BLACK);
			ex1_high =0 ;ex1_low = 0;
		}

	


	 return 30;                          
}



char  WS_Borad_Get_Keyborad_Down(void)
{  
	  static  unsigned int swIfg = 0;
    unsigned int k  ;  //  
	  unsigned int num;  // 
	  char     rtn = 0;  //  
	  char     i   = 0;  // 
	  k    = 0xffE0                     ; 
	  k   |= GPIOC->IDR ; //  PB2,PB3,PB3,PB4
	  /*******   输入逻辑处理  **********/
	  if(k != 0xffff)
		{  num =  swIfg | k;
			 if(num != 0xffff)
			 {
						for(i=0;i<16;i++)
						 {
							 if((num & 1<<i) == 0) // 
									 break;
						 }
						if(i < 16)      //
						 {	 
							 rtn = i+1;
						 }
						swIfg |= 1<<i;  // 
			  }
		}
		num = ~(swIfg & k);  // 
		swIfg &= num      ;  // 
		return rtn        ; 		 
}


int WS_OS_Task_In_Out_Borad(void *date)
{
    
	  char   key ;

	  

	  key = WS_Borad_Get_Keyborad_Down();
	
	  if(key > 0)
		{
			WS_Borad_Open_Buzzer_Time(10)   ;
			WS_Borad_Open_DebugLed_Time(10) ;
		}
	  
	  if(key == 1)
		{
			Ky += 2;
		}
		
		if(key == 2)
		{
			Ky -= 2;
		}
		if(key == 3)
		{
			Px+=4;
		}
		if(key == 4)
		{
			static int p_mode=0;
			
			switch(++p_mode)
			{
				case 0:pause=0;break;
				case 1:pause=1;Mode0();break;
				case 2:pause=1;Mode1();break;
				case 3:pause=1;Mode2();break;
			}
			if(p_mode>3)p_mode=0;


		}
		if(key == 5)
		{
			
			pause=1-pause;
				
		}
	  return 4;
}


/******************************************************************************
	*函数名称： WS_OS_TASK_INIT(void)
	*功能：     WS_OS  任务创建  , 必须放在板载资源初始化之前
	*参数：     无
	*返回:			无
*******************************************************************************/
void WS_OS_Task_Create(void)
{
     WS_OS_Init_Task();
     WS_OS_Create_Task(WS_OS_ID_Runled_define,	    WS_OS_Task_RunLed,100) ; 
     WS_OS_Create_Task(WS_OS_ID_Keyborad_define,    WS_OS_Task_In_Out_Borad ,100) ;      
}


//   WSOS 依赖线程
void WS_FreeThread_WSOS(void const * argument)
{
  
  
	WS_OS_Task_Create();
	osDelay(1000)      ;
  /* Infinite loop */ 
  for(;;)
  {
    osDelay(10)     ;
		WS_OS_On_Task() ;
		WS_OS_Run_Task();		
  }
  /* USER CODE END WSOS_FUN */
}
//uint8_t is_Square_Wave(uint16_t *data)
//{
//	int i;
//	
//	for(i=0;i<239;i++)
//	if(*(data+i)-*(data+1+i))
//	   {
//		   
//	   }
//}
uint16_t Find_Max(const uint16_t *data,uint16_t len)
{
	uint16_t i=0;
	uint16_t temp=data[0];
	for(i=1;i<len-1;i++)
	{
		if(data[i]>temp)
			temp = data[i];
		
	}
	return temp;
}
uint16_t Find_Min(const uint16_t *data,uint16_t len)
{
	uint16_t i=0,temp=data[0];
	for(i=1;i<len-1;i++)
	{
		if(data[i]<temp)
			temp = data[i];
	}
	return temp;
}

double Avg(uint16_t *data,uint16_t len)
{
	uint16_t i;
	long temp=data[0];
	for(i=0;i<len;i++)
	{
		temp+=data[i];
	}
	return temp/len;
}
double LAvg(uint32_t *data,uint16_t len)
{
	uint16_t i;
	long temp=data[0];
	for(i=0;i<len;i++)
	{
		temp+=data[i];
	}
	return temp/len;
}

int cmp(const void *a,const void *b)
{
	//从大到小改成： return *(int *)b-*(int *)a;
    return *(int *)b-*(int *)a;
}

uint16_t* Dif(uint16_t *data,uint16_t len)//计算微分
{
	uint16_t i, *d = pvPortMalloc(len/2);
	for(i=0;i<len-1;i++)
	{
		d[i] = *data - *(data+1);
		
	}
	
	return d;
}

float calculateSD(float *data,uint16_t len)//计算标准差std
{
    float mean, standardDeviation = 0.0;
 
    int i;
 
    mean = Avg((uint16_t*)data,len);
 
    for(i=0; i<len-1; ++i)
        standardDeviation += pow(data[i] - mean, 2);
 
    return sqrt(standardDeviation/10);
}

/*交换两个整数的值*/
void swap(uint16_t *p, uint16_t *q)
{
    uint16_t buf;
	
    buf = *p;
    *p = *q;
    *q = buf;
	
    return;
}
//查找众数
uint16_t find_mode_number(uint16_t *arr,uint16_t len)
{
	qsort(arr,len,sizeof(uint16_t),cmp);
	uint16_t many = 1,less = 1;
	uint16_t value = 0;
 	for(uint16_t i = 0;i < len;i++){
		for(uint16_t j = i;j < len;j++)
		{
			if(arr[j] == arr[j+1]){
				less++;
			} else {
				if(many < less){
					swap(&many,&less);
					value = arr[j];
			 	}
				less = 1;
				break;
			}		
		}
	}
	 
	 return value;
}


uint16_t Vmax_Buf[20];
uint16_t Vmin_Buf[20];


//   用户任务线程
/**
* @brief Function implementing the WS_FreeTask_use thread.
* @param argument: Not used
* @retval None
*/
void WS_FreeTask_userTask_Fun(void const * argument)
{
//	long temp1,temp2;
	uint16_t  i ,c=0;
	osDelay(3000);
//	p_adcBuffer = pvPortMalloc(ScanSize * 2);

	WS_TFT_Draw_Rectangle(0,0,319,150,BLACK);
  /* Infinite loop */
  for(;;)
	{
		
	  if(adc_flag == 2)
	  {
		  memcpy(p_adcBuffer,adcBuffer,ScanSize*2);
		  adc_flag=0;
	  }
//		HAL_ADC_Start_DMA(&hadc1,(uint32_t  *)p_adcBuffer,ScanSize);
		Wave_Type = Wareform(p_adcBuffer,Vmax*(4096/2.5),Vmin*(4096/2.5),Duty);
		Vmax_Buf[c]=Find_Max(p_adcBuffer,ScanSize);
		Vmin_Buf[c]=Find_Min(p_adcBuffer,ScanSize);
		++c>19?c=0:0;
		
		WS_TFT_Draw_Rectangle(0,0,ScanSize-1,150,BLACK);
		WS_TFT_Draw_Line(0,150-Vp*(4096/5)/Ky,320,150-Vp*(4096/5)/Ky,GRAY);//draw Vpp/2

		for(i=0;i<ScanSize-1;i++)
		{

//			WS_TFT_Draw_Line(i,150 - *(p_adcBuffer+i)/41,i+1,150 - *(p_adcBuffer+i+1)/41,YELLOW);	
//			WS_TFT_Draw_Point(i,150 - *(p_adcBuffer+i)/Ky,BLUE);

			if(abs(*(p_adcBuffer+i)-*(p_adcBuffer+i-1))>1)//斜率变化大
			{
				
				WS_TFT_Draw_Line(i-1,150-Px - *(p_adcBuffer+i)/Ky,i,150-Px - *(p_adcBuffer+i+1)/Ky,YELLOW);	

//				if(abs(*(p_adcBuffer+i)-*(p_adcBuffer+i-1))>Vp*(4096/2.5)*0.95)Skip++;
				
			}
			else 
			{
//				WS_TFT_Draw_Point(i,150 - *(p_adcBuffer+i)/Ky,YELLOW);
//				FewDif++;
			}
			
			
			
		}
//		HAL_ADC_Start_DMA(&hadc1,(uint32_t  *)p_adcBuffer,32);

		osDelay(100);
	}

}


void WS_Timer_10MS_Callback(void)
{
	
//	Vmax = Avg(Vmax_Buf,10)*2.459/4096;
//	Vmin = Avg(Vmin_Buf,10)*2.459/4096;

	
//	Vmax = find_mode_number(Vmax_Buf,19)*2.50/4096;
//	Vmin = find_mode_number(Vmin_Buf,19)*2.50/4096;
	Vmax = Find_Max(Vmax_Buf,19)*2.50/4096;
	Vmin = Find_Min(Vmin_Buf,19)*2.50/4096;			

	
	Vp = Vmax-Vmin;
	Vrms = Vmax*0.70710678;

	
	if(f<68*5000)
	{
		ADCPeriod=72E6/((48)*f*9);
		__HAL_TIM_SET_AUTORELOAD(&htim3,ADCPeriod);
	}
	else
	{
		ADCPeriod=8;
		__HAL_TIM_SET_AUTORELOAD(&htim3,10);
	}
	
	if(pause==0)
	{
		if(Relay1_State==GPIO_PIN_RESET)	Mode_State=0;//10倍
		else if(Relay1_State==GPIO_PIN_SET && Relay2_State==GPIO_PIN_RESET)	Mode_State=1;//直通
		else if(Relay1_State==GPIO_PIN_SET && Relay2_State==GPIO_PIN_SET)	Mode_State=2;//衰减5倍
		
		if(Mode_State==1)//直通模式[0.2,2.3]，ADC采集值[0.2,2.3]
		{
			if(Vp<0.2)
			{
				Mode0();			//放大					
			}
			if(Vp>2.3)
			{
				Mode2();			//衰减					
			}
		}
		if(Mode_State==0)//放大模式[0,0.23],ADC采集值[0,2.3]
		{
			if(Vp>2.3)
			{
				Mode1();			//直通				
			}

		}
		if(Mode_State==2)//衰减模式[2.3,10],ADC采集值[0.46,2]
		{

			if(Vp<0.4)
			{
				Mode1();			//直通					
			}
		}	
	}

}
/*
	5个周期，采样数为480，每个周期采样480/Tc=480/5=96,将缓存分成Tc份，每个周期要有96份
	则采样速度为频率的96倍,
	采样频率 = 96/T = 96*f
	采样频率公式 fc = (480/Tc)*f=96*f
	72 000 000/(9*Period) = fc
	ADCPeriod = 72 000 000/(fc*9)=72E6/((480/Tc)*f*9)
*/


//uint16_t tim3_cnt = 0;
//void WS_TIM3_IT_Callback(void)//定时器采样频率：1us一次
//{
//	tim3_cnt++;
//	if(tim3_cnt>=ScanSize)
//	{
//		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)p_adcBuffer,ScanSize);
//		tim3_cnt=0;
//	}
//	
//}

//void Wave_Analysis(void)
//{
//	int c=0;
//	uint16_t i=0,j=ScanSize-1;
//	uint16_t *data = Dif(data,ScanSize);			//积分数组
//	qsort(data,ScanSize,sizeof(uint16_t),cmp);		//排序
//	for(i=0;i<ScanSize-1;i++)
//	{
//		if(data[0]>=data[i]+5||data[0]<=data[i]-5)	//模糊范围匹配频数
//			c--;
//		else c++;
//		
//	}
//	if(c<2)//不匹配数为1
//	{
//		Wave_Type = 1;//方波
//	}
//	else if(c<=5)//不匹配数为2
//	{
//		Wave_Type = 2;//三角波
//	}
//	else 
//	{
//		Wave_Type = 3;//正弦波
//	}
//}


//void is_rectangle(uint16_t i)
//{

//	if(fabs((*(p_adcBuffer+i)-*(p_adcBuffer+i-1))*2.5/4096)>=Vp/3)
//	{

//		Rectangle++;
//	}
//	if(Rectangle>100)
//	{
//		
//	}
//	
//}



/*******************************************************************|
*@detile 			快速傅里叶FFT 									|
*@param  [in1]pr:	存放n个采样输入的实部，返回离散傅里叶变换的模  		|
*@param  [in2]pi:	存放n个采样输入的虚部								|
*@param  [in3]n :  	返回离散傅里叶变换的n个实部						|
*@param  [in4]k :	返回离散傅里叶变换的n个虚部						|
*@param  [in5]fr:   采样点数											|
*@param  [in6]fi:	满足n=2^k										|
*@return [out]  :	无												|
********************************************************************/
//void kfft(uintpr,pi,n,k,fr,fi)
//{


//	int it,m,is,i,j,nv,l0;
//	double p,q,s,vr,vi,poddr,poddi;
//	for (it=0; it<=n-1; it++)  //将pr[0]和pi[0]循环赋值给fr[]和fi[]
//	{ 
//		m=it; 
//		is=0;
//		for(i=0; i<=k-1; i++)
//		{ 
//			j=m/2; 
//			is=2*is+(m-2*j); 
//			m=j;
//		}
//		fr[it]=pr[is]; 
//		fi[it]=pi[is];
//	}
//	pr[0]=1.0; 
//	pi[0]=0.0;
//	p=6.283185306/(1.0*n);
//	pr[1]=cos(p); //将w=e^-j2pi/n用欧拉公式表示
//	pi[1]=-sin(p);

//	for (i=2; i<=n-1; i++)  //计算pr[]
//	{ 
//		p=pr[i-1]*pr[1]; 
//		q=pi[i-1]*pi[1];
//		s=(pr[i-1]+pi[i-1])*(pr[1]+pi[1]);
//		pr[i]=p-q; pi[i]=s-p-q;
//	}
//	for (it=0; it<=n-2; it=it+2)  
//	{ 
//		vr=fr[it]; 
//		vi=fi[it];
//		fr[it]=vr+fr[it+1]; 
//		fi[it]=vi+fi[it+1];
//		fr[it+1]=vr-fr[it+1]; 
//		fi[it+1]=vi-fi[it+1];
//	}
//	m=n/2; 
//	nv=2;
//	for (l0=k-2; l0>=0; l0--) //蝴蝶操作
//	{ 
//		m=m/2; 
//		nv=2*nv;
//		for (it=0; it<=(m-1)*nv; it=it+nv)
//		  for (j=0; j<=(nv/2)-1; j++)
//			{ 
//				p=pr[m*j]*fr[it+j+nv/2];
//				q=pi[m*j]*fi[it+j+nv/2];
//				s=pr[m*j]+pi[m*j];
//				s=s*(fr[it+j+nv/2]+fi[it+j+nv/2]);
//				poddr=p-q; 
//				poddi=s-p-q;
//				fr[it+j+nv/2]=fr[it+j]-poddr;
//				fi[it+j+nv/2]=fi[it+j]-poddi;
//				fr[it+j]=fr[it+j]+poddr;
//				fi[it+j]=fi[it+j]+poddi;
//			}
//	}
//	for (i=0; i<=n-1; i++)
//	   { 
//		  pr[i]=sqrt(fr[i]*fr[i]+fi[i]*fi[i]);  //幅值计算
//	   }
//	return;
//}



//void Wave_Dif(void)
//{
//	static uint32_t Vn=0,_Vn=0;
//	uint16_t i,time=0,_time=0;

//	if(ex1_flag == 1)
//	{
//		ex1_flag = 0;
//		_Vn = HAL_ADC_GetValue(&hadc2);
//		
//		while(time<=(500000.0/f)+10)
//		{
//			WS_CCP_Measure_Time(1,0);
//			Vn = HAL_ADC_GetValue(&hadc2);						//跟新数据
//			time += WS_CCP_Measure_Time(1,1);
//			dV_Buf[i++] = (Vn - _Vn)/(time-_time);	//求微分
//			_Vn = Vn;
//			_time = time;
//			if(i>99)
//			{
//				i=0;
//				dV_Flag = 1;
//			}
//			
//			
//		}
//		
//		ex1_flag = 1;
//	}

//}

//void Wave_Find_Vm(void)
//{
////	uint32_t max,min;
//	uint32_t *ADC_Buf;
//	uint16_t i;

//	if(ex1_flag == 1)
//	{
//		ADC_Buf = pvPortMalloc(ScanSize);
//		HAL_ADC_Start_DMA(&hadc1,(uint32_t  *)ADC_Buf,ScanSize/2);//大约6us
//		ex1_flag = 0;
//		for(i=0;ex1_flag == 0||i>=ScanSize/2;i++)
//		{
//			if(*(ADC_Buf+i)>Vmax)Vmax=*(ADC_Buf+i);
//			if(*(ADC_Buf+i)<Vmin)Vmin=*(ADC_Buf+i);

//		}
//		ex1_flag = 0;
//		
//	}

//}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvHalfCpltCallback must be implemented in the user file.
  */
	if(hadc == &hadc1)
	{
	   HAL_ADC_Stop_DMA(hadc);
	   HAL_TIM_Base_Stop_IT(&htim3);
	   adc_flag = 2;
	}
}





