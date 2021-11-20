
/******************************************************************************
 * @file     				 step_input_mode.h
 * @brief    				 输入模式处理
 * @version  				 1.0.0
 * @creationTime     2020_04_29
 * @creator					 
 * @readme
 *        1.STM32F030系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _STEP_INPUT_MODE_H__
#define _STEP_INPUT_MODE_H__


#include "ws_main.h"
#include "ws_config.h"


#define WS_Step_Input_OK  0
#define WS_Step_Input_ERR 1
#define WS_Step_Input_EMPTY 2

int WS_Step_NOTInput_check(struct __WS_StepControlTypeDef  *hstepctl);
int WS_Step_Input_check(struct __WS_StepControlTypeDef  *hstepctl);


int WS_Step_Input_Mode_Blocking(struct __WS_StepControlTypeDef  *hstepct );


int WS_Step_Input_Mode1(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode2(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode3(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode4(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode5(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode6(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode7(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode8(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode9(struct __WS_StepControlTypeDef  *hstepctl );

int WS_Step_Input_Mode10(struct __WS_StepControlTypeDef  *hstepctl );



#endif

