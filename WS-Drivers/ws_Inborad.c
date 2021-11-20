/******************************************************************************
 * @file     				 ws_Inborad.c
 * @brief    				 75hc595+CD4021扩展输入板驱动
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.75hc595+CD4021扩展输入板驱动   
 *				2. 
          3.
*******************************************************************************/

#include "ws_Inborad.h"

//      输出输出数据结构外部声明
extern  IOBoradBufferTypeDef    IOBoradBuffer ;


/******************************************************************************
	*函数名称： WS_Inborad_Get_Port_Down
	*功能：     得到产生下降沿的端口编号
	*参数：     
              无 ：           
	*返回:		  端口编号
*******************************************************************************/
uint16_t  WS_Inborad_Get_Port_Down(void)
{     
	  static  uint64_t swIfg = 0;
    uint64_t k  ;  //  
	  uint64_t num;  // 
	  uint16_t     rtn = 0;  //  
	  uint16_t     i   = 0;  // 
	
	  k    =  IOBoradBuffer.inputBorad.number        ; 	

	  /*******   输入逻辑处理  **********/
	  if(k != 0xFFFFFFFFFFFFFFFF)
		{  num =  swIfg | k;
			 if(num != 0xFFFFFFFFFFFFFFFF)
			 {
						for(i=0;i<64;i++)
						 {
							 if((num & (uint64_t)1<<i) == 0) // 
									 break;
						 }
						if(i < 64)      //
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


/******************************************************************************
	*函数名称： WS_Inborad_Get_Port_Up
	*功能：     得到产生上升沿的端口编号
	*参数：     
              无 ：          
	*返回:		  端口编号
*******************************************************************************/
uint16_t  WS_Inborad_Get_Port_Up(void)
{     
	  static  uint64_t swIfg = 0;
    uint64_t k  ;  //  
	  uint64_t num;  // 
	  uint16_t     rtn = 0;  //  
	  uint16_t     i   = 0;  // 
	
	  k    =  ~IOBoradBuffer.inputBorad.number ; 	

	  /*******   输入逻辑处理  **********/
	  if(k != 0xFFFFFFFFFFFFFFFF)
		{  num =  swIfg | k;
			 if(num != 0xFFFFFFFFFFFFFFFF)
			 {
						for(i=0;i<64;i++)
						 {
							 if((num & (uint64_t)1<<i) == 0) // 
									 break;
						 }
						if(i < 64)      //
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


/*
函数名称char WS_Borad_Get_Port_Stu_Pin(char n)
函数功能：得到指定输入端口的状态
输入参数: n:需要查询的按键编号(1~8)	
返回值  ：按键的状态  
*/
uint16_t  WS_Inborad_Get_Port_Pin_Stu(uint16_t n)
{
	   //uint16_t  f = 2;
     
	   if(n < 1 || n > 64)   return 2;
	
	   if((IOBoradBuffer.inputBorad.number & (uint32_t)1<<(n-1)) == 0)
		 {
		    return 0;
		 }
		 else
		 {
		    return 1;
		 }
	   //return f;	 
}

/*
函数名称char WS_Borad_Get_Port_Stu_All(char n)
函数功能：得到所有输入端口的状态
输入参数: 无	
返回值  ：所有端口状态  
*/
uint64_t   WS_Inborad_Get_Port_All_Stu(void)
{
      
	     return  IOBoradBuffer.inputBorad.number              ;
}




/*
函数名称char WS_Borad_Get_Port_All_NOTStu(char n)
函数功能：得到所有输入端口的反向状态
输入参数: 无	
返回值  ：所有端口反向状态  
*/
uint64_t   WS_Inborad_Get_Port_All_NOTStu(void)
{   
	     return  ~IOBoradBuffer.inputBorad.number;
}



/*
函数名称  WS_Inborad_Set_Port_Stu
函数功能：设置输入板端口的输出状态
输入参数: n： 端口号 ； stu: 状态（0：通电；1：断电；2：取反）
返回值  ：无  
*/
void   WS_Inborad_Set_Port_Stu(uint16_t n , uint16_t stu )
{   
	     if(n < 1 || n > 64)   return ;
	
	     n --;
	     n = 63-n;
	
	     switch(stu)
			 {
				 //   吸合
				 case 0:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  |=  (1<<(7-n%8))  ;
					       break;  
				 
				 //   释放
				 case 1:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  &= ~(1<<(7-n%8))  ;
					       break;
				 
				 //  取反
				 case 2:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  ^=  (1<<(7-n%8))  ;
					       break;
			 }				 
}



