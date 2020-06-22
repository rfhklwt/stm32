#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：PWM  驱动代码(头文件)
*  作者：rfhklwt
*  修改日期：2017/07/16
*  版本：V1.0
*  功能：
***************************************************************/

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init_hubu(u16 arr,u16 psc);

#endif
