#ifndef __LED_H
#define __LED_H
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：LED驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/09
*  版本：V1.0
*  功能：初始化PB5和PE5为输出口.并使能这两个口的时钟
***************************************************************/
#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2

void LED_Init(void);	//初始化


#endif
