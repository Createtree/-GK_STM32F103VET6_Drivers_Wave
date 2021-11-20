/******************************************************************************
 * @file     				 ws_Inborad.c
 * @brief    				 75hc595+CD4021��չ���������
 * @version  				 1.0.0
 * @creationTime     2020_07_16
 * @creator					 
 * @readme
 *        1.75hc595+CD4021��չ���������   
 *				2. 
          3.
*******************************************************************************/

#include "ws_Inborad.h"

//      ���������ݽṹ�ⲿ����
extern  IOBoradBufferTypeDef    IOBoradBuffer ;


/******************************************************************************
	*�������ƣ� WS_Inborad_Get_Port_Down
	*���ܣ�     �õ������½��صĶ˿ڱ��
	*������     
              �� ��           
	*����:		  �˿ڱ��
*******************************************************************************/
uint16_t  WS_Inborad_Get_Port_Down(void)
{     
	  static  uint64_t swIfg = 0;
    uint64_t k  ;  //  
	  uint64_t num;  // 
	  uint16_t     rtn = 0;  //  
	  uint16_t     i   = 0;  // 
	
	  k    =  IOBoradBuffer.inputBorad.number        ; 	

	  /*******   �����߼�����  **********/
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
	*�������ƣ� WS_Inborad_Get_Port_Up
	*���ܣ�     �õ����������صĶ˿ڱ��
	*������     
              �� ��          
	*����:		  �˿ڱ��
*******************************************************************************/
uint16_t  WS_Inborad_Get_Port_Up(void)
{     
	  static  uint64_t swIfg = 0;
    uint64_t k  ;  //  
	  uint64_t num;  // 
	  uint16_t     rtn = 0;  //  
	  uint16_t     i   = 0;  // 
	
	  k    =  ~IOBoradBuffer.inputBorad.number ; 	

	  /*******   �����߼�����  **********/
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
�������ƣvchar WS_Borad_Get_Port_Stu_Pin(char n)
�������ܣ��õ�ָ������˿ڵ�״̬
�������: n:��Ҫ��ѯ�İ������(1~8)	
����ֵ  ��������״̬  
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
�������ƣvchar WS_Borad_Get_Port_Stu_All(char n)
�������ܣ��õ���������˿ڵ�״̬
�������: ��	
����ֵ  �����ж˿�״̬  
*/
uint64_t   WS_Inborad_Get_Port_All_Stu(void)
{
      
	     return  IOBoradBuffer.inputBorad.number              ;
}




/*
�������ƣvchar WS_Borad_Get_Port_All_NOTStu(char n)
�������ܣ��õ���������˿ڵķ���״̬
�������: ��	
����ֵ  �����ж˿ڷ���״̬  
*/
uint64_t   WS_Inborad_Get_Port_All_NOTStu(void)
{   
	     return  ~IOBoradBuffer.inputBorad.number;
}



/*
��������  WS_Inborad_Set_Port_Stu
�������ܣ����������˿ڵ����״̬
�������: n�� �˿ں� �� stu: ״̬��0��ͨ�磻1���ϵ磻2��ȡ����
����ֵ  ����  
*/
void   WS_Inborad_Set_Port_Stu(uint16_t n , uint16_t stu )
{   
	     if(n < 1 || n > 64)   return ;
	
	     n --;
	     n = 63-n;
	
	     switch(stu)
			 {
				 //   ����
				 case 0:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  |=  (1<<(7-n%8))  ;
					       break;  
				 
				 //   �ͷ�
				 case 1:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  &= ~(1<<(7-n%8))  ;
					       break;
				 
				 //  ȡ��
				 case 2:
					       IOBoradBuffer.inputBoradTransmitBuffer[n/8]  ^=  (1<<(7-n%8))  ;
					       break;
			 }				 
}



