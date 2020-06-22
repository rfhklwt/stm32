#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：按键驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/10
*  版本：V1.0
***************************************************************/

/***************************************************************
* 名称：   按键读取
* KEY0 	-->		PA13
* KEY1 	-->		PA15
* WK_UP 	-->		PA0
***************************************************************/
#define KEY0  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13)		//读取按键0
#define KEY1  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)		//读取按键1
#define KEY2  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)			//读取按键2
#define WK_UP   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)		//读取按键3

#define KEY0_PRES	1											//KEY0
#define KEY1_PRES	2											//KEY1
#define WKUP_PRES	3											//WK_UP

//函数初始化
void KEY_Init(void);											//IO初始化
u8 KEY_Scan(u8 mode);  											//按键扫描函数

#endif
