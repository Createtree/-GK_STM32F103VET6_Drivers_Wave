#include "ws_nrf24L01.h"

extern  SPI_HandleTypeDef hspi1  ;

#ifdef NRF24L01_EN

//   ͨ�Ų�������
#define NRF_SELFADD   "00"
#define TX_ADR_WIDTH    2    // 2  uints TX address width
#define RX_ADR_WIDTH    2    // 2  uints RX address width

uint8_t nrfTxDateLen  = 1;   // �������ݳ���
uint8_t nrfRxDateLen  = 1;   // �������ݳ���

#define TX_PLOAD_WIDTH  nrfTxDateLen    
#define RX_PLOAD_WIDTH  nrfRxDateLen    

char  NRF_Tx_Buf[16]; //�������ݻ���
char  NRF_Rx_Buf[16]; //�������ݻ���

char  nrfSelfAddBuf[] = NRF_SELFADD ;




//******************************************************************************************
uchar sta;   //״̬��־

//����ʱ
void delay_ms(unsigned int x)
{
	   HAL_Delay(x);
}
/************************************IO ��ģ��SPI���� ����************************************************/
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
uchar SPI_RW_Reg(uchar reg,uchar value) // ��Ĵ���REGдһ���ֽڣ�ͬʱ����״̬�ֽ�
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
	status = SPI_RW(reg);      // ѡ��Ĵ���д��Ͷ�ȡ״̬�ֽ�
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // Ȼ��д�����ֽ��ڻ�����(* pBuf)
	SPI_RW(*pBuf++);
	SET_NFR24L01_CSN;;                   // Set NFR_CSN high again
	return(status);            // nRF24L01����״̬�ֽ�
}

//   ���ñ�����ַ
void setNrfSelfAdd(char *p)
{
     memcpy(nrfSelfAddBuf,p,RX_ADR_WIDTH);
}
//   �õ�������ַ
char *getNrfSelfAdd(void)
{
    return nrfSelfAddBuf;
}


char    nrfMode = 0;
#define nrf_T_mode 1
#define nrf_R_mode 2
#define nrf_DOWN_mode 3
#define nrf_IDLE_mode 4

/*******************************��*****��*****ģ*****ʽ*****��*****��*************************************/
void nrfTxMode(void)
{
	CLR_NFR24L01_CE;		   //2.4G����ģ��IO�ڳ�ʼ��
	CLR_NFR_SCK;
	SET_NFR24L01_CSN;
	CLR_NFR24L01_CE;
	SPI_RW_Reg(FLUSH_TX,0x00);
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, getNrfSelfAdd(), TX_ADR_WIDTH);    // tx��ַ��nRF24L01д��
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); // RX_Addr0 tx adrһ��Ϊ�Զ�ack
	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x15); // 500us + 86us, 10 �ط�...1a
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0);         // ѡ��RF�ŵ�40 
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0e);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ2�ֽ�
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
	//SET_NFR24L01_CE;
	//delay_ms(100);
	nrfMode = nrf_T_mode;
}


/*******************************����*****ģ*****ʽ*****��*****��*************************************/
void powerDownModeNrf(void)
{
	CLR_NFR24L01_CE ;		   //2.4G����ģ��IO�ڳ�ʼ��
	CLR_NFR_SCK;
	SET_NFR24L01_CSN;
	CLR_NFR24L01_CE ;
	SPI_RW_Reg(FLUSH_TX,0x00);
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, nrfSelfAddBuf, TX_ADR_WIDTH);    // tx��ַ��nRF24L01д��
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); // RX_Addr0 tx adrһ��Ϊ�Զ�ack
	SPI_RW_Reg(NRF_WRITE_REG + EN_AA, 0x01);      // Enable Auto.Ack:Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	SPI_RW_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 �ط�...1a
	SPI_RW_Reg(NRF_WRITE_REG + RF_CH, 0);        // ѡ��RF�ŵ�40 
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0e);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ2�ֽ�
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0c);
	//SET_NFR24L01_CE;
	//delay_ms(100);
	nrfMode = nrf_DOWN_mode;
	
}


/*******************************��*****��*****ģ*****ʽ*****��*****��*************************************/
uchar SPI_Read_Buf(char reg, char *pBuf, char uchars)
{
	uchar status,i;								   // 
	CLR_NFR24L01_CSN;                      // Set NFR_CSN low, init SPI tranaction
	status = SPI_RW(reg);         // ѡ��Ĵ���д��Ͷ�ȡ��״̬ uchar
	for(i=0;i<uchars;i++)
		pBuf[i] = SPI_RW(0);      // 
	SET_NFR24L01_CSN;       
	return(status);                    // return nRF24L01 status uchar
}
/******************************************************************************************************/
/*������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)*/
/*���ܣ����ݶ�ȡ�����rx_buf���ջ�������*/
/*****************************************************************************************************/
unsigned char nRF24L01_RxPacket( char* rx_buf)
{
	unsigned char revale=0;
	sta=SPI_Read(STATUS)  ; // ��ȡ״̬�Ĵ������ж����ݽ���״��
	if((RX_DR & sta) && sta != 0xff)             // �ж��Ƿ���յ�����
	{
		//NFR_CE = 0;    //SPIʹ��
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;   //��ȡ������ɱ�־
		//Delay(100);
	}
	SPI_RW_Reg(NRF_WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}
/****************************************************************************************************/
/*������void RX_Mode(void)*/
/*���ܣ����ݽ������� */
/***************************************************************************************************/
void WS_NRF_Set_Rx_Mode(char *add, uint8_t  len)
{
  setNrfSelfAdd(add) ;
	nrfRxDateLen = len ;
	CLR_NFR24L01_CE;		   //2.4G����ģ��IO�ڳ�ʼ��
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
	SPI_RW_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(NRF_WRITE_REG + RF_SETUP, 0x0E);    // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
	SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0f);
	SET_NFR24L01_CE;

	nrfMode = nrf_R_mode;

}

#ifdef NRF24L01_SEND_EN  //  �ظ�����

//    �ظ����ͱ�־
char  sendCmdNrfIfg = 0;
//    �Ǵ���
char  sendCmdNrfCount = 0;

//    Ŀ���ַ����
char  targetAddBuf[TX_ADR_WIDTH+1] ;

char  sendNRFBuf(void)
{
    char rtn , len;
     //����
	  CLR_NFR24L01_CE;   //StandBy Iģʽ
	
	  // ���÷��͵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAddBuf, TX_ADR_WIDTH)   ;    
      // װ�ؽ��ն˵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAddBuf, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // װ������ 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //�ø�NFR_CE���������ݷ���
				
	  delay_ms(5); //  �ȴ�
		
		//  ��ⷢ��״̬
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
//			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //��ǰSTATUS״̬  �����ж�Ӧʹbit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //����Ƿ��ͳ�ʱ bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  �ȴ�
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  �������ģʽ
		
		if(nrfMode == nrf_R_mode)  //  ���ݷ������,���ý���ģʽ
		{
		   SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); 
		   SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	     SET_NFR24L01_CE;
		}

		return rtn;
}

//  ѭ����������
void sendCmdNrfTask(void);

#define NRF_SEND_DATA_BUF_LEN 10
char  NRFSendDataBuf[NRF_SEND_DATA_BUF_LEN][TX_PLOAD_WIDTH+1];
char  NRFSendAddBuf [NRF_SEND_DATA_BUF_LEN][TX_ADR_WIDTH+1]  ;
int  NRFSendBufCount = 0               ;
int  NRFSaveBufCount = 0               ;

//   ���ͺ���
//   0 : �ɹ��� 1ʧ��
char sendCommandRNF(char *targetAdd, char command, char *dat)
{   
    char len, r;
	
	  
	  //  װ�ص�ַ
	  strcpy(NRFSendAddBuf[NRFSaveBufCount],targetAdd);	
	
	  //  ��֯��������
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
		
//  r = 	sendNRFBuf();    //   �������� 
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

//  ѭ����������
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
				if(++sendCmdNrfCount >= 20)  //  ���50�δ���
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

//   ���ͺ���
//   0 : �ɹ��� 1ʧ��
char sendInfoRNF(char *targetAdd, char command, char *dat)
{   
    char len, rtn = 0;
	
	  //  ��֯��������
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
	
	  //����
	  CLR_NFR24L01_CE;   //StandBy Iģʽ
	
	  // ���÷��͵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAdd, TX_ADR_WIDTH)   ;    
      // װ�ؽ��ն˵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAdd, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // װ������ 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //�ø�NFR_CE���������ݷ���
				
	  delay_ms(5); //  �ȴ�
		
		//  ��ⷢ��״̬
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //��ǰSTATUS״̬  �����ж�Ӧʹbit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //����Ƿ��ͳ�ʱ bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  �ȴ�
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  �������ģʽ
		
		if(nrfMode == nrf_R_mode)  //  ���ݷ������,���ý���ģʽ
		{
		   SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, nrfSelfAddBuf, TX_ADR_WIDTH); 
		   SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0F);
	       SET_NFR24L01_CE;
		}
    return rtn;	
}
#endif



#else
//   ���ͺ���
//   0 : �ɹ��� 1ʧ��
char sendCommandRNF(char *targetAdd, char command, char *dat)
{   
    char len, rtn = 0;
	  rtn  = WS_ERR;	
	  //  ��֯��������
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
	
	  //����
	  CLR_NFR24L01_CE;   //StandBy Iģʽ
	
	  // ���÷��͵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, targetAdd, TX_ADR_WIDTH)   ;    
      // װ�ؽ��ն˵�ַ
	  SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, targetAdd, TX_ADR_WIDTH);

	  
	  SPI_RW_Reg(FLUSH_TX,0x00);
	
	 
		
     // װ������ 		
	  SPI_Write_Buf(WR_TX_PLOAD,NRF_Tx_Buf, TX_PLOAD_WIDTH); 
		
		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	  SPI_RW_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
		
	  SET_NFR24L01_CE;   //�ø�NFR_CE���������ݷ���
				
	  delay_ms(10);      //  �ȴ�
		  
		//  ��ⷢ��״̬
		len = 5;
		while(len --)
		{
			sta=SPI_Read(NRF_READ_REG +  STATUS);
			if(sta & TX_DS)	 //��ǰSTATUS״̬  �����ж�Ӧʹbit5 = 1 
			{ 
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_OK;
	            break;				
			}
			if(sta & MAX_RT)  //����Ƿ��ͳ�ʱ bit4 = 1;
			{
				SPI_RW_Reg(NRF_WRITE_REG + STATUS,sta);
				rtn  = WS_ERR;		
				break;
			}
			delay_ms(1); //  �ȴ�
	    }
		//sendString(WSFormatData("len =%d\n sta = %x\n",(int)len,(int)sta));
		
		CLR_NFR24L01_CE;  //  �������ģʽ
		
		if(nrfMode == nrf_R_mode)  //  ���ݷ������,���ý���ģʽ
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

//  NRF��Ⲣ��ȡ����
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
	  //   ѭ������
    //  sendCmdNrfTask();	   
#endif

		return 4;
}
#endif



#endif


