#ifndef _LCD5110_H__
#define _LCD5110_H__
#include "sys.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：LCD5110驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/25
*  版本：V2.0
*  功能：LCD函数头文件（函数声明）
***************************************************************/

void LCD5110_Init(void);
void LCD_Show_Char(u8 x, u8 y, u8 ASCII);
void LCD_Show_String(u8 x, u8 y, u8 *str);
void LCD_Show_BigChar(u8, u8, u8, u8, u8);
void LCD_Show_BigString(u8, u8, u8, u8, u8 *);
void LCD_Show_Chinese(u8 x, u8 y, u8 width, u8 index);


#endif





