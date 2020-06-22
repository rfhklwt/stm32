#ifndef __ADC_H
#define __ADC_H
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：ADC配置(头文件)
*  作者：rfhklwt
*  修改日期：2017/07/15
*  版本：V1.0
*  功能：
***************************************************************/

void Adc_Init(void);
u16  Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif
