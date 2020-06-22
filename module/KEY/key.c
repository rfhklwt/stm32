#include "key.h"
#include "delay.h"
/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：按键驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/10
*  版本：V1.0
*  功能：把PA15,PA13,PA0与按键连上并初始化
***************************************************************/


/***************************************************************************/
/*              按键初始化函数（输入：PA15和PA13以及PA0和PC5）             */
/***************************************************************************/
void KEY_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);//使能PORTA, PORTC时钟

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;									//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA15

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;									//PA13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;									//PC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);										//初始化

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;									//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//PA0设置成输入，默认下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.0
}


/***************************************************************
* 名称：   按键处理函数（返回按键值）
* mode：	0：不支持连续按
*			1：支持连续按
*
* 返回值：	0：没有任何按键按下
*			KEY0_PRES，KEY0按下
*			KEY1_PRES，KEY1按下
*			WKUP_PRES，WK_UP按下
* 注意：	此函数有响应优先级,KEY0 > KEY1 > WK_UP!!
***************************************************************/

u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;					//按键按松开标志
	if (mode)	key_up = 1;					//按键松开标志
	if (key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1))
	{
		delay_ms(10);						//去抖动
		key_up = 0;
		if (KEY0 == 0)	return KEY0_PRES;
		else if (KEY1 == 0)	return KEY1_PRES;
		else if (WK_UP == 0)	return WKUP_PRES;
	}
	else if (KEY0 == 1 && KEY1 == 1 && WK_UP == 0)
	{
		key_up = 1;
	}
	return 0;								//无按键按下
}
