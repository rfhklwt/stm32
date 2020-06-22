#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：通用定时器3配置头文件
*  作者：rfhklwt
*  修改日期：2017/07/14
*  版本：V1.0
*  功能：
***************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);

#endif
