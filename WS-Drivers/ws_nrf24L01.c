#include "ws_nrf24L01.h"

extern  SPI_HandleTypeDef hspi1  ;

#ifdef NRF24L01_EN

//   通信参数配置
#define NRF_SELFADD   "00"
#define TX_ADR_WIDTH    2    // 2  uints TX address width
#define RX_ADR_WIDTH    2    // 2  uints RX address width

uint8_t nrfTxDateLen  = 1;   // 发送数据长度
uint8_t nrfRxDateLen  = 1;   // 接收数据长度

#define TX_PLOAD_WIDTH  nrfTxDateLen    
#define RX_PLOAD_WIDTH  nrfRxDateLen    

char  NRF_Tx_Buf[16]; //发送数据缓存
char  NRF_Rx_Buf[16]; //接收数据缓存

char  nrfSelfAddBuf[] = NRF_SELFADD ;




//******************************************************************************************
uchar sta;   //状态标志

//短延时
void delay_ms(unsigned int x)
{
	   HAL_Delay(x);
}
/************************************IO 口模拟SPI总线 代码************************************************/
uchar SPI_RW(uchar byte)
{
	  uchar i;
	for(i=0;i<8;i++)
	{   
		if(byte&0x80) 
			SET_NFR24L01_MOSI
		else          
			CLR_NFR24L01_MOSI
		
		SET_NFR_SCK; 
		//delayUs(1) ;
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		
		byte=(byte<<1);	
		if(GET_NFR_MISO)byte|=1;	 
		
		CLR_NFR_SCK;
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		//delayUs(1);
		
	}
	return(byte);
}
uchar SPI_RW_Reg(uchar reg,uchar value) // 向寄存器REG写一个字节，同时返回状态字节
{
	uchar status;
	CLR_NFR24L01_CSN;
	status=SPI_RW(reg);
	SPI_RW(value);
	SET_NFR24L01_CSN;
	return(status);
}
uchar SPI_Read (uchar  reg )
{
	uchar reg_val;
	CLR_NFR24L01_CSN;
	SPI_RW(reg);
	reg_val=SPI_RW(0);
	SET_NFR24L01_CSN;
	return(reg_val);
}

uchar SPI_Write_Buf(char reg, char *pBuf, char bytes)
{
	uchar status,byte_ctr;
	CLR_NFR24L01_CSN;                   // Set NFR_CSN low, init SPI tranaction
	status = SPI_RW(reg);      // 选择寄存器写入和读取状态字节
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // 然后写所有字节在缓冲区(* pBuf)
	SPI_RW(*pBuf++);
	SET_NFR24L01_CSN;;                   // Set NFR_CSN high again
	return(status);            // nRF24L01返回状态字节
}

//   设置本机地址
void setNrfSelfAdd(char *p)
{
     memcpy(nrfSelfAddBuf,p,RX_ADR_WIDTH);
}
//   得到本机地址
char *getNrfSelfAdd(void)
{
    return nrfSelfAddBuf;
}


char    nrfMode = 0;
#define nrf_T_mode 1
#define nrf_R_mode 2
#define nrf_DOWN_mode 3
#define nrf_IDLE_mode 4

/*******************************发*****送*****模*****式*****代*****码*************************************/
void nrfTxMode(void)
{
	CLR_NFR24L01_CE;		   //2.4G无线模块IO口初始化
	CLR_NFR_SCK;
	SET_NFR24L01_CSN;
	CLR_NFR24L01_CE;
	SPI_RW_Reg(FLUSH_TX,0x00);
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, getNrfSelfAdd(), TX_ADR_WIDTH);    // tx地址来nRF24L01写道
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); // RX_Addr0 tx adr一样为自动ack
	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x15); // 500us + 86us, 10 重发...1a
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0);         // 选择RF信道40 
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0e);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为2字节
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
	//SET_NFR24L01_CE;
	//delay_ms(100);
	nrfMode = nrf_T_mode;
}


/*******************************掉电*****模*****式*****代*****码*************************************/
void powerDownModeNrf(void)
{
	CLR_NFR24L01_CE ;		   //2.4G无线模块IO口初始化
	CLR_NFR_SCK;
	SET_NFR24L01_CSN;
	CLR_NFR24L01_CE ;
	SPI_RW_Reg(FLUSH_TX,0x00);
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, nrfSelfAddBuf, TX_ADR_WIDTH);    // tx地址来nRF24L01写道
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); // RX_Addr0 tx adr一样为自动ack
	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 重发...1a
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0);        // 选择RF信道40 
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0e);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为2字节
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0c);
	//SET_NFR24L01_CE;
	//delay_ms(100);
	nrfMode = nrf_DOWN_mode;
	
}


/*******************************接*****收*****模*****式*****代*****码*************************************/
uchar SPI_Read_Buf(char reg, char *pBuf, char uchars)
{
	uchar status,i;								   // 
	CLR_NFR24L01_CSN;                      // Set NFR_CSN low, init SPI tranaction
	status = SPI_RW(reg);         // 选择寄存器写入和读取的状态 uchar
	for(i=0;i<uchars;i++)
		pBuf[i] = SPI_RW(0);      // 
	SET_NFR24L01_CSN;       
	return(status);                    // return nRF24L01 status uchar
}
/******************************************************************************************************/
/*函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)*/
/*功能：数据读取后放如rx_buf接收缓冲区中*/
/*****************************************************************************************************/
unsigned char nRF24L01_RxPacket( char* rx_buf)
{
	unsigned char revale=0;
	sta=SPI_Read(STATUS)  ; // 读取状态寄存其来判断数据接收状况
	if((RX_DR & sta) && sta != 0xff)             // 判断是否接收到数据
	{
		//NFR_CE = 0;    //SPI使能
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;   //读取数据完成标志
		//Delay(100);
	}
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return revale;
}
/****************************************************************************************************/
/*函数：void RX_Mode(void)*/
/*功能：数据接收配置 */
/***************************************************************************************************/
void WS_NRF_Set_Rx_Mode(char *add, uint8_t  len)
{
  setNrfSelfAdd(add) ;
	nrfRxDateLen = len ;
	CLR_NFR24L01_CE;		   //2.4G无线模块IO口初始化
	CLR_NFR_SCK;
	SET_NFR24L01_CSN;
	CLR_NFR24L01_CE;
	SPI_RW_Reg(FLUSH_RX,0x00);
	SPI_RW_Reg(NRF_WRITE_REG +SETUP_AW,0x01);
	//SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); // RX_Addr0 same as TX_Adr for Auto.Ack 
  //	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);       // Enable Auto.Ack:Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01) ;  // Enable Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x15);  // 500us + 86us, 10 retrans...1a
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0);          // Select RF channel 40
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0E);    // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0f);
	SET_NFR24L01_CE;

	nrfMode = nrf_R_mode;

}

#ifdef NRF24L01_SEND_EN  //  重复发送

//    重复发送标志
char  sendCmdNrfIfg = 0;
//    记次数
char  sendCmdNrfCount = 0;

//    目标地址缓存
char  targetAddBuf[TX_ADR_WIDTH+1] ;

char  sendNRFBuf(void)
{
    char rtn , len;
     //传输
	  CLR_NFR24L01_CE;   //StandBy I模式
	
	  // 设置发送地址
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAddBuf, TX_ADR_WIDTH)   ;    
      // 装载接收端地址
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAddBuf, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // 装载数据 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ收发完成中断响应，16位CRC，主发送
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //置高NFR_CE，激发数据发送
				
	  delay_ms(5); //  等待
		
		//  检测发送状态
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
//			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //当前STATUS状态  发送中断应使bit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //如果是发送超时 bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  等待
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  进入待机模式
		
		if(nrfMode == nrf_R_mode)  //  数据发送完成,设置接收模式
		{
		   SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); 
		   SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	     SET_NFR24L01_CE;
		}

		return rtn;
}

//  循环发送任务
void sendCmdNrfTask(void);

#define NRF_SEND_DATA_BUF_LEN 10
char  NRFSendDataBuf[NRF_SEND_DATA_BUF_LEN][TX_PLOAD_WIDTH+1];
char  NRFSendAddBuf [NRF_SEND_DATA_BUF_LEN][TX_ADR_WIDTH+1]  ;
int  NRFSendBufCount = 0               ;
int  NRFSaveBufCount = 0               ;

//   发送函数
//   0 : 成功， 1失败
char sendCommandRNF(char *targetAdd, char command, char *dat)
{   
    char len, r;
	
	  
	  //  装载地址
	  strcpy(NRFSendAddBuf[NRFSaveBufCount],targetAdd);	
	
	  //  组织发送数据
	  NRFSendDataBuf[NRFSaveBufCount][5] = command;
	  memcpy(NRFSendDataBuf[NRFSaveBufCount],  nrfSelfAddBuf,TX_ADR_WIDTH);
	  if(dat > 0)
		{
	    len = strlen(dat);
			NRFSendDataBuf[NRFSaveBufCount][6] = 0x30+len/10;
			NRFSendDataBuf[NRFSaveBufCount][7] = len%10 + 0x30;
			NRFSendDataBuf[NRFSaveBufCount][8] = 'S';
		  memcpy(NRFSendDataBuf[NRFSaveBufCount]+9,dat,len);
		  NRFSendDataBuf[NRFSaveBufCount][9 + len] = '\n'  ;
			NRFSendDataBuf[NRFSaveBufCount][9 + len] = 0  ;
		}
		else
		{
      NRFSendDataBuf[NRFSaveBufCount][7] = 0x30;
			NRFSendDataBuf[NRFSaveBufCount][6] = 0x30;
			NRFSendDataBuf[NRFSaveBufCount][8] = 'S' ;
			NRFSendDataBuf[NRFSaveBufCount][9] = '\n';
			NRFSendDataBuf[NRFSaveBufCount][9] = 0;
    }
    
		if(++NRFSaveBufCount  == NRF_SEND_DATA_BUF_LEN ) 
			   NRFSaveBufCount = 0;
		
//  r = 	sendNRFBuf();    //   启动传输 
//	if(r == WS_OK)
//	 {
//	    return WS_OK;	
//	 }
//	else
//	 {	sendCmdNrfCount = 0;
//	 	  sendCmdNrfIfg = 1;
//		  return WS_ERR;	
//	 }
		 
    return WS_OK;	
}

//  循环发送任务
void sendCmdNrfTask(void)
{
   static int timeCount = 0;
	 char r = 0;
	 if(sendCmdNrfIfg != 0)
	  {
	    if(++timeCount >= 10)
		  {
		//  rs485SendStringToOut(NRF_Tx_Buf);
		//  rs485SendStringToOut("\r\n")    ;
		//		sendUsartStr(USART1,NRF_Tx_Buf);
		//		sendUsartStr(USART1,"\r\n");
				timeCount = 0    ;
				r =  sendNRFBuf();
				if(r == WS_OK)
				 {
					  sendCmdNrfIfg = 0;
					  
				 }
				if(++sendCmdNrfCount >= 20)  //  最大50次传输
				 {
						sendCmdNrfCount = 0;
						sendCmdNrfIfg   = 0;
				 }
			}
	  }
		else
		{
		   if(NRFSaveBufCount != NRFSendBufCount)
			 {
			     strcpy(NRF_Tx_Buf,  NRFSendDataBuf[NRFSendBufCount]);	
				   strcpy(targetAddBuf,NRFSendAddBuf[NRFSendBufCount]) ;	
				   if(++NRFSendBufCount == NRF_SEND_DATA_BUF_LEN)
						    NRFSendBufCount = 0;
					 
					 sendCmdNrfIfg = 1;
			 }
		}
		
}


#ifdef NRF24L01_SEND_ONE_EN

//   发送函数
//   0 : 成功， 1失败
char sendInfoRNF(char *targetAdd, char command, char *dat)
{   
    char len, rtn = 0;
	
	  //  组织发送数据
	  memcpy(NRF_Tx_Buf,nrfSelfAddBuf,TX_ADR_WIDTH);
	  NRF_Tx_Buf[5] = command;
	  if(dat > 0)
		{
	    len = strlen(dat);
			NRF_Tx_Buf[6] = 0x30+len/10;
			NRF_Tx_Buf[7] = len%10 + 0x30;
			NRF_Tx_Buf[8] = 'S';
		  memcpy(NRF_Tx_Buf+9,dat,len);
		  NRF_Tx_Buf[9 + len] = '\n'  ;
			NRF_Tx_Buf[9 + len] = 0  ;
		}
		else
		{
      NRF_Tx_Buf[7] = 0x30;
			NRF_Tx_Buf[6] = 0x30;
			NRF_Tx_Buf[8] = 'S';
			NRF_Tx_Buf[9] = '\n';
			NRF_Tx_Buf[9] = 0;
    }
	
	  //传输
	  CLR_NFR24L01_CE;   //StandBy I模式
	
	  // 设置发送地址
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAdd, TX_ADR_WIDTH)   ;    
      // 装载接收端地址
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAdd, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // 装载数据 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ收发完成中断响应，16位CRC，主发送
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //置高NFR_CE，激发数据发送
				
	  delay_ms(5); //  等待
		
		//  检测发送状态
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //当前STATUS状态  发送中断应使bit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //如果是发送超时 bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  等待
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  进入待机模式
		
		if(nrfMode == nrf_R_mode)  //  数据发送完成,设置接收模式
		{
		   SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); 
		   SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	       SET_NFR24L01_CE;
		}
    return rtn;	
}
#endif



#else
//   发送函数
//   0 : 成功， 1失败
char sendCommandRNF(char *targetAdd, char command, char *dat)
{   
    char len, rtn = 0;
	  rtn  = WS_ERR;	
	  //  组织发送数据
	  memcpy(NRF_Tx_Buf,nrfSelfAddBuf,TX_ADR_WIDTH);
	  NRF_Tx_Buf[5] = command;
	  if(dat > 0)
		{
	        len = strlen(dat);
			NRF_Tx_Buf[6] = 0x30+len/10;
			NRF_Tx_Buf[7] = len%10 + 0x30;
			NRF_Tx_Buf[8] = 'S';
		    memcpy(NRF_Tx_Buf+9,dat,len);
		    NRF_Tx_Buf[9 + len] = '\n'  ;
			NRF_Tx_Buf[9 + len] = 0  ;
		}
		else
		{
            NRF_Tx_Buf[7] = 0x30;
			NRF_Tx_Buf[6] = 0x30;
			NRF_Tx_Buf[8] = 'S';
			NRF_Tx_Buf[9] = '\n';
			NRF_Tx_Buf[9] = 0;
      }
	
	  //传输
	  CLR_NFR24L01_CE;   //StandBy I模式
	
	  // 设置发送地址
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAdd, TX_ADR_WIDTH)   ;    
      // 装载接收端地址
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAdd, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // 装载数据 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ收发完成中断响应，16位CRC，主发送
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //置高NFR_CE，激发数据发送
				
	  delay_ms(10);      //  等待
		  
		//  检测发送状态
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //当前STATUS状态  发送中断应使bit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //如果是发送超时 bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  等待
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  进入待机模式
		
		if(nrfMode == nrf_R_mode)  //  数据发送完成,设置接收模式
		{
		   SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); 
		   SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	     SET_NFR24L01_CE;
		}
    return rtn;	
}

#endif





#ifdef NRF24L01_RXEN

char nrfRxDate = 0;

char getNrfRxDate(void)
{
   char f = 0;
	 if(nrfRxDate > 0)
	 {
	    f = nrfRxDate;
		  nrfRxDate = 0;
	 }
   return  f;
}

//  NRF检测并读取数据
int WS_NRF_Receive_Task(void * date)
{
	  //char c;
	  date = date;
    if(nRF24L01_RxPacket(NRF_Rx_Buf))
		{   
			  WS_Borad_Open_Buzzer_Time(10)  ;
			  nrfRxDate = NRF_Rx_Buf[0]      ;
    }
		//rs484SendString("D1 open\n");

#ifdef NRF24L01_SEND_EN
	  //   循环发送
    //  sendCmdNrfTask();	   
#endif

		return 4;
}
#endif



#endif


