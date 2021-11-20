



#ifndef _WS_CONFIG_H__
#define _WS_CONFIG_H__


#include "ws_main.h"



//    ���ò������ƽṹ��
typedef struct  __WS_ConfigInfoTypeDef
{
	uint32_t           powerOnCount  ;            //  ��¼��������    	
	uint16_t           initFlag      ;            //  ��ʼ����־

} WS_ConfigInfoTypeDef;

//      ϵͳ������Ϣ   
extern  WS_ConfigInfoTypeDef   systemConfigInfo    ;


void WS_System_Config_Init(void);
void WS_Config_System_Save(void);	

//  �洢����ȡ���麯��
__weak  void  WS_Config_Read_Struct_Callback(uint8_t *  date  ,  uint32_t len) ;
__weak  void  WS_Config_Write_Struct_Callback(uint8_t *  date  ,  uint32_t len);


#endif


